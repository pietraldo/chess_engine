#include "MoveGeneration.h"



Board* MoveGeneration::boardFromFEN(string fen)
{
	// example string: r4rk1/1bpq1ppp/pp2p3/3nP3/PbBP4/2N3B1/1P2QPPP/R2R2K1 w - - 1 18

	// substracting first part from string
	size_t pos = fen.find(' ');
	string position = fen.substr(0, pos);

	fen = fen.substr(pos + 1);
	pos = fen.find(' ');
	string move = fen.substr(0, pos);

	fen = fen.substr(pos + 1);
	pos = fen.find(' ');
	string castle = fen.substr(0, fen.find(' '));

	fen = fen.substr(pos + 1);
	pos = fen.find(' ');
	string onPassantField = fen.substr(0, fen.find(' '));

	Board* board = new Board();
	board->whoToMove = move == "w" ? WHITE : BLACK;
	board->onPassantField = onPassantField == "-" ? 64 : int(onPassantField[0] - 97) + (int)(onPassantField[1] - '1') * 8;
	board->castleRights[WHITE][SHORTCASTLE] = castle.find("K") != string::npos;
	board->castleRights[WHITE][LONGCASTLE] = castle.find("Q") != string::npos;
	board->castleRights[BLACK][SHORTCASTLE] = castle.find("k") != string::npos;
	board->castleRights[BLACK][LONGCASTLE] = castle.find("q") != string::npos;

	int row = 7;
	int column = 0;
	for (int i = 0; i < position.size(); i++)
	{
		switch (position[i])
		{
		case 'r':
			board->figure[BLACK][ROOK] |= fields[row * N + column];
			break;
		case 'R':
			board->figure[WHITE][ROOK] |= fields[row * N + column];
			break;
		case 'q':
			board->figure[BLACK][QUEEN] |= fields[row * N + column];
			break;
		case 'Q':
			board->figure[WHITE][QUEEN] |= fields[row * N + column];
			break;
		case 'k':
			board->figure[BLACK][KING] |= fields[row * N + column];
			break;
		case 'K':
			board->figure[WHITE][KING] |= fields[row * N + column];
			break;
		case 'p':
			board->figure[BLACK][PAWN] |= fields[row * N + column];
			break;
		case 'P':
			board->figure[WHITE][PAWN] |= fields[row * N + column];
			break;
		case 'N':
			board->figure[WHITE][KNIGHT] |= fields[row * N + column];
			break;
		case 'n':
			board->figure[BLACK][KNIGHT] |= fields[row * N + column];
			break;
		case 'B':
			board->figure[WHITE][BISHOP] |= fields[row * N + column];
			break;
		case 'b':
			board->figure[BLACK][BISHOP] |= fields[row * N + column];
			break;
		case '/':
			column = 0;
			row--;
			continue;
			break;
		default:
			column += (int)(position[i] - '1');
			break;
		}
		column++;
	}
	for (int i = 0; i < 6; i++)
	{
		board->occupancy[WHITE] |= board->figure[WHITE][i];
		board->occupancy[BLACK] |= board->figure[BLACK][i];
	}
	return board;
}


bool MoveGeneration::isCheck(Board board, int kingField, Color kingColor)
{
	Color oppositeColor = toggleColor(kingColor);

	// rook
	Bitboard attackRook = attackRookf(board, kingField, kingColor);
	if ((attackRook & board.figure[oppositeColor][ROOK]) != 0 || (attackRook & board.figure[oppositeColor][QUEEN]) != 0) return true;
	
	// bishop
	Bitboard attackBishop = attackBishopf(board, kingField, kingColor);
	if (((attackBishop & board.figure[oppositeColor][BISHOP]) != 0) || ((attackBishop & board.figure[oppositeColor][QUEEN]) != 0)) return true;

	// knight
	if ((GamePrepare::attackKnight[kingField] & board.figure[oppositeColor][KNIGHT]) != 0) return true;

	// pawn
	if ((GamePrepare::attackPawn[kingColor][kingField] & board.figure[oppositeColor][PAWN]) != 0) return true;

	// king
	if ((GamePrepare::attackKing[kingField] & board.figure[oppositeColor][KING]) != 0) return true;

	return false;
}

bool MoveGeneration::isLegalCastle(Board& board, Color color, Castle castle)
{
	if (!board.castleRights[color][castle]) return false;

	Bitboard occ = board.occupancy[WHITE] | board.occupancy[BLACK];

	auto check = [&](int start, int end, Bitboard fieldsBetween, Bitboard rookField, Bitboard kingField) -> bool {

		// checking if there isn't pieces betewen king and rook
		if ((fieldsBetween & occ)) return false;

		// checking if this field is under attack
		for (int i = start; i < end; i++)
			if (isCheck(board, i, color)) return false;

		// checking if pieces are on correct fields
		if ((board.figure[color][ROOK] & rookField) == 0 || (board.figure[color][KING] & kingField) == 0) return false;

		return true;
	};

	if (color == WHITE)
	{
		if (castle == SHORTCASTLE)
			return check(4, 7, shortWhiteCastle, H1, E1);
		else
			return check(2, 5, longWhiteCastle, A1, E1);
	}
	else
	{
		if (castle == SHORTCASTLE)
			return check(60, 63, shortBlackCastle, H8, E8);
		else
			return check(58, 61, longBlackCastle, A8, E8);
	}
}

Bitboard MoveGeneration::attackRookf(Board& board, int index, Color color)
{
	Bitboard occ = board.occupancy[WHITE] | board.occupancy[BLACK];
	// part of key for hashing
	Bitboard mask = occ & rookMask[index];
	// getting attacks
	Bitboard attackRook = GamePrepare::attackRook[index][(mask * GamePrepare::magicRook[index]) >> (M - GamePrepare::rookShifts[index])];

	// substracting attack on own piece
	//attackRook -= attackRook & board.occupancy[color];
	return attackRook;
}

Bitboard MoveGeneration::attackBishopf(Board& board, int index, Color color)
{
	Bitboard occ = board.occupancy[WHITE] | board.occupancy[BLACK];
	// part of key for hashing
	Bitboard mask = occ & bishopMask[index];
	// getting attacks
	Bitboard attackBishop = GamePrepare::attacBishop[index][(mask * GamePrepare::magicBishop[index]) >> (M - GamePrepare::bishopShifts[index])];
	// substracting attack on own piece
	//attackBishop -= attackBishop & board.occupancy[color];
	return attackBishop;
}

Bitboard MoveGeneration::attackQueenf(Board& board, int index, Color color)
{
	return attackBishopf(board, index, color) | attackRookf(board, index, color);
}

Bitboard MoveGeneration::attackKnightf(Board& board, int index, Color color)
{
	Bitboard attackKnight = GamePrepare::attackKnight[index];
	// substracting attack on own piece
	/*attackKnight -= attackKnight & board.occupancy[color];*/
	return attackKnight;
}

Bitboard MoveGeneration::attackKingf(Board& board, int index, Color color)
{
	Bitboard attackKing = GamePrepare::attackKing[index];
	// substracting attack on own piece
	//attackKing -= attackKing & board.occupancy[color];
	return attackKing;
}

Bitboard MoveGeneration::attackPawnf(Board& board, int index, Color color)
{
	Bitboard attackPawn_v = GamePrepare::attackPawn[color][index];
	return attackPawn_v;
}

void MoveGeneration::addPawnMoves(Board& board, Color color, list<Move>& moveList, Bitboard mask, Bitboard pinned)
{
	Bitboard figures = board.figure[color][PAWN];
	Color opponent_color = toggleColor(color);
	figures &= ~pinned;
	
	while (figures)
	{
		int index_figure = bitScanForward(figures);
		Bitboard movePawn = GamePrepare::pawnMoves[color][index_figure];
		Bitboard attacksPawn = attackPawnf(board, index_figure, color) & board.occupancy[toggleColor(color)] & mask;
		
		// delete if is obsticle before pawn
		movePawn -= movePawn & (board.occupancy[WHITE] | board.occupancy[BLACK]);
		
		// delete move of two fields because on first is obsticle
		if (abs(index_figure - log2(movePawn)) == 16)
			movePawn = 0;
		movePawn &= mask;

		// enPassant capture
		if ((fields[board.onPassantField] & GamePrepare::attackPawn[color][index_figure]) != 0)
		{
			Move m;
			m.move = fields[index_figure] | fields[board.onPassantField];
			m.type_piece = color * P + PAWN;
			m.type_piece2 = opponent_color * P + PAWN;
			m.castleRights[SHORTCASTLE] = board.castleRights[color][SHORTCASTLE];
			m.castleRights[LONGCASTLE] = board.castleRights[color][LONGCASTLE];
			if (color == WHITE)
				m.move2 = fields[board.onPassantField - 8];
			else
				m.move2 = fields[board.onPassantField + 8];
			m.enPassant = board.onPassantField;
			
			//printBitboard(board.occupancy[color]);
			makeMove(board, m);
			//printBitboard(board.occupancy[color]);
			if(!isCheck(board, bitScanForward(board.figure[color][KING]), color))
				moveList.push_back(m);
			unmakeMove(board, m);
		}


		while (attacksPawn)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attacksPawn);

			Move m;
			m.move = fields[index_figure] | fields[index];
			m.type_piece = color * P + PAWN;
			m.castleRights[SHORTCASTLE] = board.castleRights[color][SHORTCASTLE];
			m.castleRights[LONGCASTLE] = board.castleRights[color][LONGCASTLE];
			m.enPassant = board.onPassantField;


			for (int i = 0; i < 6; i++)
			{
				if (fields[index] & board.figure[opponent_color][i])
				{
					m.type_piece2 = opponent_color * P + i;
					m.move2 = fields[index];
					break;
				}
			}
			if (index >= 56 || index <= 7)
			{

				m.move = fields[index_figure];
				m.move3 = fields[index];
				m.type_piece3 = color * P + QUEEN;
				moveList.push_back(m);
				m.type_piece3 = color * P + ROOK;
				moveList.push_back(m);
				m.type_piece3 = color * P + KNIGHT;
				moveList.push_back(m);
				m.type_piece3 = color * P + BISHOP;
				moveList.push_back(m);
			}
			else
			{
				moveList.push_back(m);
			}

			// substracting added move
			attacksPawn &= attacksPawn - 1;
		}
		
		while (movePawn)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(movePawn);

			Move m;
			m.move = fields[index] | fields[index_figure];
			m.type_piece = color * P + PAWN;
			m.castleRights[SHORTCASTLE] = board.castleRights[color][SHORTCASTLE];
			m.castleRights[LONGCASTLE] = board.castleRights[color][LONGCASTLE];
			m.enPassant = board.onPassantField;
			if (index >= 56 || index <= 7)
			{
				m.move = fields[index_figure];
				m.move3 = fields[index];
				m.type_piece3 = color * P + QUEEN;
				moveList.push_back(m);
				m.type_piece3 = color * P + ROOK;
				moveList.push_back(m);
				m.type_piece3 = color * P + KNIGHT;
				moveList.push_back(m);
				m.type_piece3 = color * P + BISHOP;
				moveList.push_back(m);
			}
			else
			{
				moveList.push_back(m);
			}

			// substracting added move
			movePawn &= movePawn - 1;
		}


		figures &= figures - 1;
	}
}





int MoveGeneration::count = 0;
string MoveGeneration::output = "";

void MoveGeneration::generateMoves(Board& board, Color color, Figure figure_type, list<Move>& moveList, Bitboard stopCheckFields, Bitboard pinned)
{
	Bitboard figures = board.figure[color][figure_type];
	 
	figures &= ~pinned; // not pinned figures
	
	while (figures)
	{
		int index_figure = bitScanForward(figures);
		Bitboard attacks = attacks_funcitons[figure_type](board, index_figure, color) & stopCheckFields;
		attacks -= attacks & board.occupancy[color];

		addMovesToList(board,color, figure_type, index_figure, attacks, moveList);
		
		figures &= figures - 1;
	}
}

void MoveGeneration::addMovesToList(Board& board, Color color, int figure_type, int index_figure, Bitboard attacks, list<Move>& moveList)
{
	while (attacks)
	{
		int index_attack = bitScanForward(attacks);

		Move m;

		m.move = fields[index_attack] | fields[index_figure];
		m.type_piece = color * P + figure_type;
		m.castleRights[SHORTCASTLE] = board.castleRights[color][SHORTCASTLE];
		m.castleRights[LONGCASTLE] = board.castleRights[color][LONGCASTLE];
		m.enPassant = board.onPassantField;

		Color opposit_color = toggleColor(color);
		if (fields[index_attack] & board.occupancy[opposit_color]) //capture move
		{
			m.move2 = fields[index_attack];
			for (int i = 0; i < P; i++)
			{
				if (fields[index_attack] & board.figure[opposit_color][i])
				{
					m.type_piece2 = opposit_color * P + i;
					break;
				}
			}
		}

		moveList.push_back(m);

		attacks &= attacks - 1;
	}
}


int MoveGeneration::generation(Board* board, Color color, int max_depth, int depth)
{
	list<Move> legalMoves = list<Move>();
	if (depth == max_depth)
	{
		generateMovesNew(*board, color, legalMoves);
		return legalMoves.size();
	}



	generateMovesNew(*board, color, legalMoves);

	int num = 0;
	for (auto m : legalMoves)
	{
		// copying board
		Board* boardNew = new Board();
		boardNew->castleRights[0][0] = board->castleRights[0][0];
		boardNew->castleRights[0][1] = board->castleRights[0][1];
		boardNew->castleRights[1][0] = board->castleRights[1][0];
		boardNew->castleRights[1][1] = board->castleRights[1][1];

		boardNew->onPassantField = board->onPassantField;

		for (int i = 0; i < 6; i++)
		{
			boardNew->figure[WHITE][i] = board->figure[WHITE][i];
			boardNew->figure[BLACK][i] = board->figure[BLACK][i];
		}
		boardNew->occupancy[WHITE] = board->occupancy[WHITE];
		boardNew->occupancy[BLACK] = board->occupancy[BLACK];

		makeMove(*boardNew, m);
		int num2 = generation(boardNew, (color == WHITE) ? BLACK : WHITE, max_depth, depth + 1);

		if (depth == 1)
		{
			int one = bitScanForward(m.move);
			int two = bitScanForward(m.move ^ fields[one]);
			Bitboard aa = ((board->whoToMove == WHITE) ? board->occupancy[WHITE] : board->occupancy[BLACK]);

			if (fields[one] & aa)
				swap(two, one);
			//printBitboard(m.move);
			char a = (char)(two % 8 + 97);
			int b = (int)(two / 8) + 1;
			char c = (char)(one % 8 + 97);
			int d = (int)(one / 8) + 1;
			int e = num2;
			output += a + to_string(b) + c + to_string(d) + ": " + to_string(e) + "\n";
			//cout << output <<" "<<m.type_piece << endl;
			//cout<<(char)(m.from % 8 + 97) << (int)(m.from / 8) + 1  << (char)(m.to % 8 + 97) << (int)(m.to / 8) + 1<<": "<<num2 << endl;
		}
		num += num2;

		delete boardNew;
	}
	return num;
}

// TODO: except checking all checks in castle rights do attack board




void MoveGeneration::generateMovesNew(Board& board, Color color, list<Move>& moveList)
{
	// There are three options
	// 1. double (or more) checks -> king move
	// 2. check -> king move, capture, cover
	// 3. no check
	
	int num_checks = 0;
	Bitboard attackMap = 0;
	Bitboard stopCheckFields = universe;
	Color oponent = toggleColor(color);
	Bitboard kingField = board.figure[color][KING];
	int king_index = bitScanForward(kingField);


	// checking how many checks are there and creating attack map
	auto addAttack = [&](Bitboard figures, int figure_type)
	{
		while (figures)
		{
			int index = bitScanForward(figures);
			Bitboard attackFigure = attacks_funcitons[figure_type](board, index, oponent);
			board.occupancy[color] ^= board.figure[color][KING];
			Bitboard attackFigure2 = attacks_funcitons[figure_type](board, index, oponent);
			attackMap |= attackFigure2;
			board.occupancy[color] ^= board.figure[color][KING];
			
			// if there is check
			if (attackFigure & kingField)
			{
				num_checks++;
				stopCheckFields = attackFigure & attacks_funcitons[figure_type](board, king_index, color); 
				stopCheckFields |=fields[index]; 
			}

			figures &= figures - 1;
		}
	};
	for (int i = 0; i < P; i++)
	{
		if (i == QUEEN)
		{
			Bitboard figures = board.figure[oponent][QUEEN];
			addAttack(figures, ROOK);
			addAttack(figures, BISHOP);
			continue;
		}

		Bitboard figures = board.figure[oponent][i];
		addAttack(figures,i);
	}
	
	
	// first case -> only king moves
	if (num_checks >= 2)
	{
		Bitboard kingMoves = attackKingf(board, king_index, color);
		kingMoves -= kingMoves & attackMap;
		kingMoves-= kingMoves & board.occupancy[color];
		addMovesToList(board, color, KING,king_index,kingMoves,moveList);
	}
	else
	{
		// looking for pinned figures
		// generating moves for pinned figures
		

		Bitboard pinned = 0; // figures that are pinned
		// figures can be pinned by rook, bishop or queen
		// 1. place rook or bishop in king's square and detect which figures are protecting king
		// 2. delete this figure and now check if there is a bishop or rook attacking if so this deleted figure is pinned
		for (int i = 0; i < 2; i++)
		{
			Bitboard attackFields = attacks_funcitons[i](board, king_index, oponent);
			Bitboard covering = attackFields & board.occupancy[color];
			Bitboard previous = (attacks_funcitons[i](board, king_index, color)) & (board.figure[oponent][i] | board.figure[oponent][QUEEN]);
			
			Bitboard pinningFigures = board.figure[oponent][i] | board.figure[oponent][QUEEN];

			while (covering)
			{
				int pinned_index = bitScanForward(covering);
				Bitboard figure_field = fields[pinned_index];
				// remove from occupancy
				board.occupancy[color] ^= figure_field;
				// check if it is a check 
				Bitboard pinned_figure_legal_moves = attacks_funcitons[i](board, king_index, color);
				if ((pinned_figure_legal_moves & pinningFigures) > previous)
				{
					int pinningFigureIndex = bitScanForward((pinned_figure_legal_moves&pinningFigures) - previous);
					pinned_figure_legal_moves&= attacks_funcitons[i](board, pinningFigureIndex, color);
					pinned_figure_legal_moves |= fields[pinningFigureIndex];
					// without this figure it is check so...
					// add to pinned 
					pinned |= fields[pinned_index];
					board.occupancy[color] ^= figure_field;
					for (int j = 0; j < P - 1; j++)
					{
						if (figure_field & board.figure[color][j])
						{
							if (j != PAWN)
							{
								Bitboard legalMovesFigure = attacks_funcitons[j](board, pinned_index, color) & pinned_figure_legal_moves & stopCheckFields;
								legalMovesFigure -= legalMovesFigure & board.occupancy[color];
								addMovesToList(board, color, j, pinned_index, legalMovesFigure, moveList);
							}
							else
							{
								addPawnMoves(board, color, moveList, stopCheckFields&pinned_figure_legal_moves, ~pinned);
							}
								
						}
					}

				}
				else
				{
					board.occupancy[color] ^= figure_field;
				}
				
				
				covering &= covering - 1;
			}
			
		}
		
		// generating moves of figures that are not pinned and this figures can go only for fields that are in stop check fields
		vector<Figure> figures = { ROOK, BISHOP, KNIGHT, QUEEN};
		for (auto figure : figures) {
			generateMoves(board, color, figure, moveList, stopCheckFields, pinned);
		}
		generateMoves(board, color, KING, moveList, ~attackMap, pinned);
		
		addCastleMove(board, color, moveList);
		addPawnMoves(board, color, moveList, stopCheckFields, pinned);
	}
	
}

// returns list of legal moves in given position for the color

void MoveGeneration::addCastleMove(Board& board, Color color, list<Move>& moveList)
{
	auto addMove = [&](Bitboard fields_move_king, Bitboard fields_move_rook) {
		Move m;
		m.move = fields_move_king;
		m.type_piece = color * P + KING;
		m.move2 = fields_move_rook;
		m.type_piece2 = color * P + ROOK;
		m.enPassant = board.onPassantField;

		m.castleRights[SHORTCASTLE] = board.castleRights[color][SHORTCASTLE];
		m.castleRights[LONGCASTLE] = board.castleRights[color][LONGCASTLE];

		moveList.push_back(m);
	};

	if (color == WHITE)
	{
		if (isLegalCastle(board, color, SHORTCASTLE))
			addMove(E1 | G1, H1 | F1);

		if (isLegalCastle(board, color, LONGCASTLE))
			addMove(E1 | C1, A1 | D1);
	}
	else
	{
		if (isLegalCastle(board, color, SHORTCASTLE))
			addMove(E8 | G8, H8 | F8);

		if (isLegalCastle(board, color, LONGCASTLE))
			addMove(E8 | C8, A8 | D8);
	}

}

void MoveGeneration::makeMove(Board& board, Move& move)
{
	board.figure[move.type_piece / P][move.type_piece % P] ^= move.move;
	board.figure[move.type_piece2 / P][move.type_piece2 % P] ^= move.move2;
	board.figure[move.type_piece3 / P][move.type_piece3 % P] ^= move.move3;

	changeOnPassantMove(board, move);
	changeCastleRights(board, move);

	// changing board occupancy
	board.occupancy[move.type_piece / P] ^= move.move;
	board.occupancy[move.type_piece2 / P] ^= move.move2;
	board.occupancy[move.type_piece3 / P] ^= move.move3;
}

void MoveGeneration::unmakeMove(Board& board, Move& move)
{
	if (move.type_piece < 6) // white move
	{
		board.castleRights[WHITE][SHORTCASTLE] = move.castleRights[SHORTCASTLE];
		board.castleRights[WHITE][LONGCASTLE] = move.castleRights[LONGCASTLE];
	}
	else
	{
		board.castleRights[BLACK][SHORTCASTLE] = move.castleRights[SHORTCASTLE];
		board.castleRights[BLACK][LONGCASTLE] = move.castleRights[LONGCASTLE];
	}
	board.figure[move.type_piece / P][move.type_piece % P] ^= move.move;
	board.figure[move.type_piece2 / P][move.type_piece2 % P] ^= move.move2;
	board.figure[move.type_piece3 / P][move.type_piece3 % P] ^= move.move3;

	board.onPassantField = move.enPassant;

	board.occupancy[move.type_piece / P] ^= move.move;
	board.occupancy[move.type_piece2 / P] ^= move.move2;
	board.occupancy[move.type_piece3 / P] ^= move.move3;
}

void MoveGeneration::changeCastleRights(Board& board, Move& move)
{
	// if they have no castle rights return because nothing can change
	if (!move.castleRights[0] && !move.castleRights[1]) return;

	int color = (move.type_piece / P);
	Bitboard rook1, rook2;

	if (color == WHITE)
	{
		rook1 = A1;
		rook2 = H1;
	}
	else
	{
		rook1 = A8;
		rook2 = H8;
	}

	// changing rights to castle
	if ((move.type_piece % P) == KING)
	{
		board.castleRights[color][SHORTCASTLE] = false;
		board.castleRights[color][LONGCASTLE] = false;
	}
	else if ((move.type_piece % P) == ROOK)
	{
		if (move.move & rook1)
			board.castleRights[color][LONGCASTLE] = false;
		else if (move.move & rook2)
			board.castleRights[color][SHORTCASTLE] = false;
	}
}

void MoveGeneration::changeOnPassantMove(Board& board, Move& move)
{
	// if piece is pawn and it moved two squares => set onPassant field between start field and end field
	board.onPassantField = 64;
	if (move.type_piece == WHITE * P + PAWN || move.type_piece == BLACK * P + PAWN)
	{
		int one = bitScanForward(move.move);
		int two = bitScanForward(move.move ^ fields[one]);

		if (abs(one - two) == 16)
		{
			board.onPassantField = (one + two) / 2;

		}
	}
}

