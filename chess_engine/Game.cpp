#include "Game.h"

int bitScanForward(Bitboard bb)
{
	unsigned long index;
	_BitScanForward64(&index, bb);
	return index;
}

bool Game::isCheck(Board board, int kingField, Color kingColor)
{
	Color oppositeColor = toggleColor(kingColor);
	
	// rook
	Bitboard attackRook = attackRookf(board, kingField, kingColor);
	if ((attackRook & board.figure[oppositeColor][ROOK]) != 0|| (attackRook & board.figure[oppositeColor][QUEEN]) != 0) return true;
		
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

Bitboard Game::attackRookf(Board& board, int index, Color color)
{
	Bitboard occ = board.occupancy[WHITE] | board.occupancy[BLACK];
	// part of key for hashing
	Bitboard mask = occ & rookMask[index];
	// getting attacks
	Bitboard attackRook = GamePrepare::attackRook[index][(mask * GamePrepare::magicRook[index]) >> (M - GamePrepare::rookShifts[index])];
	
	// substracting attack on own piece
	attackRook -= attackRook & board.occupancy[color];
	return attackRook;
}

Bitboard Game::attackBishopf(Board& board, int index, Color color)
{
	Bitboard occ = board.occupancy[WHITE] | board.occupancy[BLACK];
	// part of key for hashing
	Bitboard mask = occ & bishopMask[index];
	// getting attacks
	Bitboard attackBishop = GamePrepare::attacBishop[index][(mask * GamePrepare::magicBishop[index]) >> (M - GamePrepare::bishopShifts[index])];
	// substracting attack on own piece
	attackBishop -= attackBishop & board.occupancy[color];
	return attackBishop;
}

Bitboard Game::attackQueenf(Board& board, int index, Color color)
{
	return attackBishopf(board, index, color)| attackRookf(board, index, color);
}

Bitboard Game::attackKnightf(Board& board, int index, Color color)
{
	Bitboard attackKnight = GamePrepare::attackKnight[index];
	// substracting attack on own piece
	attackKnight -= attackKnight & board.occupancy[color];
	return attackKnight;
}

Bitboard Game::attackKingf(Board& board, int index, Color color)
{
	Bitboard attackKing = GamePrepare::attackKing[index];
	// substracting attack on own piece
	attackKing -= attackKing & board.occupancy[color];
	return attackKing;
}

Bitboard Game::attackPawnf(Board& board, int index, Color color)
{
	Bitboard attackPawn_v = GamePrepare::attackPawn[color][index];
	attackPawn_v &= board.occupancy[toggleColor(color)];
	return attackPawn_v;
}


void Game::addPawnMoves(Board& board, Color color, list<Move>& moveList)
{
	Bitboard figures = board.figure[color][PAWN];
	Color opponent_color = toggleColor(color);

	while (figures)
	{
		int index_figure = bitScanForward(figures);
		Bitboard movePawn = GamePrepare::pawnMoves[color][index_figure];
		Bitboard attacksPawn = attackPawnf(board, index_figure,color);


		// delete if is obsticle before pawn
		movePawn -= movePawn & (board.occupancy[WHITE] | board.occupancy[BLACK]);
		// delete move of two fields because on first is obsticle
		if (abs(index_figure - log2(movePawn)) == 16)
			movePawn = 0;


		//enPassant capture
		if ((fields[board.onPassantField] & GamePrepare::attackPawn[color][index_figure]) != 0)
		{
			Move m;
			m.move = fields[index_figure] | fields[board.onPassantField];
			m.type_piece = color * P + PAWN;
			m.type_piece2 = opponent_color * P + PAWN;
			if (color == WHITE)
			{
				m.move2 = fields[board.onPassantField - 8];
				m.longCastle = board.WLC;
				m.shortCastle = board.WSC;
			}
			else
			{
				m.move2 = fields[board.onPassantField + 8];
				m.longCastle = board.BLC;
				m.shortCastle = board.BSC;
			}
			m.enPassant = board.onPassantField;
			moveList.push_back(m);
		}

		if (color == WHITE)
		{
			while (attacksPawn)
			{
				// geting index of field that rook is attacking
				int index = bitScanForward(attacksPawn);
				
				Move m;
				m.move = fields[index_figure] | fields[index];
				m.type_piece = WHITE*P+PAWN;
				m.longCastle = board.WLC;
				m.shortCastle = board.WSC;
				m.enPassant = board.onPassantField;
				
				
				for (int i = 0; i < 6; i++)
				{
					if (fields[index] & board.figure[BLACK][i])
					{
						m.type_piece2 = BLACK*P+i;
						m.move2 = fields[index];
						break;
					}
				}
				if (index >= 56)
				{
					
					m.move = fields[index_figure];
					m.move3 = fields[index];
					m.type_piece3 = WHITE*P+QUEEN;
					moveList.push_back(m);
					m.type_piece3 = WHITE * P + ROOK;
					moveList.push_back(m);
					m.type_piece3 = WHITE * P + KNIGHT;
					moveList.push_back(m);
					m.type_piece3 = WHITE * P + BISHOP;
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
				m.type_piece = WHITE * P + PAWN;
				m.longCastle = board.WLC;
				m.shortCastle = board.WSC;
				m.enPassant = board.onPassantField;
				if (index >= 56)
				{
					m.move = fields[index_figure];
					m.move3 = fields[index];
					m.type_piece3 = WHITE * P + QUEEN;
					moveList.push_back(m);
					m.type_piece3 = WHITE * P + ROOK;
					moveList.push_back(m);
					m.type_piece3 = WHITE * P + KNIGHT;
					moveList.push_back(m);
					m.type_piece3 = WHITE * P + BISHOP;
					moveList.push_back(m);
				}
				else
				{
					moveList.push_back(m);
				}
				
				// substracting added move
				movePawn &= movePawn - 1;
			}
		}
		else
		{
			while (attacksPawn)
			{
				// geting index of field that rook is attacking
				int index = bitScanForward(attacksPawn);

				Move m;
				m.move = fields[index_figure] | fields[index];
				m.type_piece = BLACK*P+PAWN;
				m.longCastle = board.BLC;
				m.shortCastle = board.BSC;
				m.enPassant = board.onPassantField;


				for (int i = 0; i < 6; i++)
				{
					if (fields[index] & board.figure[WHITE][i])
					{
						m.type_piece2 = WHITE*P+i;
						m.move2 = fields[index];
						break;
					}
				}
				if (index <= 7)
				{
					m.move = fields[index_figure];
					m.move3 = fields[index];
					m.type_piece3 = BLACK*P+QUEEN;
					moveList.push_back(m);
					m.type_piece3 = BLACK * P + ROOK;
					moveList.push_back(m);
					m.type_piece3 = BLACK * P + KNIGHT;
					moveList.push_back(m);
					m.type_piece3 = BLACK * P + BISHOP;
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
				m.move = fields[index_figure] | fields[index];
				m.type_piece = BLACK * P + PAWN;
				m.longCastle = board.BLC;
				m.shortCastle = board.BSC;
				m.enPassant = board.onPassantField;
				if (index <= 7)
				{
					m.move = fields[index_figure];
					m.move3 = fields[index];
					m.type_piece3 = BLACK * P + QUEEN;
					moveList.push_back(m);
					m.type_piece3 = BLACK * P + ROOK;
					moveList.push_back(m);
					m.type_piece3 = BLACK * P + KNIGHT;
					moveList.push_back(m);
					m.type_piece3 = BLACK * P + BISHOP;
					moveList.push_back(m);
				}
				else
				{
					moveList.push_back(m);
				}



				// substracting added move
				movePawn &= movePawn - 1;
			}
		}
		figures &= figures - 1;
	}
}

void Game::generateMoves(Board& board, Color color, Figure figure_type, list<Move>& moveList)
{
	Bitboard figures = board.figure[color][figure_type];
	while (figures)
	{
		int index_figure = bitScanForward(figures);
		Bitboard attacks = attacks_funcitons[figure_type](board, index_figure, color);

		while (attacks)
		{
			int index_attack = bitScanForward(attacks);

			Move m;

			m.move = fields[index_attack] | fields[index_figure];
			m.type_piece = color * P + figure_type;
			// TODO: change this below because it might be wrong
			m.longCastle = board.WLC;
			m.shortCastle = board.WSC;
			m.enPassant = board.onPassantField;

			Color opposit_color = toggleColor(color);
			if (fields[index_attack] & board.occupancy[opposit_color]) //capture move
			{
				m.move2 = fields[index_attack];
				for (int i = 0; i < P; i++)
				{
					if (fields[index_attack] & board.figure[opposit_color][i])
					{
						m.type_piece2 = opposit_color*P+ i;
						break;
					}
				}
			}

			moveList.push_back(m);

			attacks &= attacks - 1;
		}
		figures &= figures - 1;
	}
}

bool Game::isLegalCastle(Board& board, Color color, Castle castle)
{
	int start, end;
	Bitboard fieldsBetween, rookField, kingField;

	Bitboard occ = board.occupancy[WHITE] | board.occupancy[BLACK];
	
	// checking rights for castle and settings variables
	if (color == WHITE)
	{
		kingField = E1;
		if (castle == SHORTCASTLE)
		{
			if (!board.WSC) return false;
			start = 4;
			end = 7;
			rookField = H1;
			fieldsBetween = shortWhiteCastle;
		}
		else
		{
			if (!board.WLC) return false;
			start = 2;
			end = 5;
			rookField = A1;
			fieldsBetween = longWhiteCastle;
		}
	}
	else
	{
		kingField = E8;
		if (castle == SHORTCASTLE)
		{
			if (!board.BSC) return false;
			start = 60;
			end = 63;
			rookField = H8;
			fieldsBetween = shortBlackCastle;
		}
		else
		{
			if (!board.BLC) return false;
			start = 58;
			end = 61;
			rookField = A8;
			fieldsBetween = longBlackCastle;

		}
	}

	// checking if there isn't pieces betewen king and rook
	if ((fieldsBetween & occ)) return false;

	// checking if this field is under attack
	for (int i = start; i < end; i++)
		if (isCheck(board, i, color)) return false;

	// checking if pieces are on correct fields
	if ((board.figure[color][ROOK] & rookField) == 0 || (board.figure[color][KING] & kingField) == 0) return false;
	
	return true;
}

Board* Game::boardFromFEN(string fen)
{
	// example string: r4rk1/1bpq1ppp/pp2p3/3nP3/PbBP4/2N3B1/1P2QPPP/R2R2K1 w - - 1 18

	// substracting first part from string
	size_t pos=fen.find(' ');
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
	board->onPassantField = onPassantField == "-" ? 64 : int(onPassantField[0] - 97) + (int)(onPassantField[1]-'1') * 8;
	board->WSC = castle.find("K")!=string::npos;
	board->WLC = castle.find("Q")!=string::npos;
	board->BSC = castle.find("k")!=string::npos;
	board->BLC = castle.find("q")!=string::npos;

	int row=7;
	int column = 0;
	for (int i = 0; i < position.size(); i++)
	{
		switch (position[i])
		{
		case 'r':
			board->figure[BLACK][ROOK] |= fields[row*N+column];
			break;
		case 'R':
			board->figure[WHITE][ROOK] |= fields[row*N+column];
			break;
		case 'q':
			board->figure[BLACK][QUEEN] |= fields[row*N+column];
			break;
		case 'Q':
			board->figure[WHITE][QUEEN] |= fields[row*N+column];
			break;
		case 'k':
			board->figure[BLACK][KING] |= fields[row*N+column];
			break;
		case 'K':
			board->figure[WHITE][KING] |= fields[row*N+column];
			break;
		case 'p':
			board->figure[BLACK][PAWN] |= fields[row*N+column];
			break;
		case 'P':
			board->figure[WHITE][PAWN] |= fields[row*N+column];
			break;
		case 'N':
			board->figure[WHITE][KNIGHT] |= fields[row*N+column];
			break;
		case 'n':
			board->figure[BLACK][KNIGHT] |= fields[row*N+column];
			break;
		case 'B':
			board->figure[WHITE][BISHOP] |= fields[row*N+column];
			break;
		case 'b':
			board->figure[BLACK][BISHOP] |= fields[row*N+column];
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



int Game::count = 0;
string Game::output = "";

int Game::generation(Board* board, Color color,int max_depth, int depth)
{
	if (depth-1 == max_depth) return 1;
	
	list<Move> legalMoves = list<Move>();

	moveGeneration2(*board, color, legalMoves);
	
	int num = 0;
	for (auto m : legalMoves)
	{
		Board* boardNew= new Board();
		boardNew->BLC = board->BLC;
		boardNew->BSC = board->BSC;
		boardNew->WSC = board->WSC;
		boardNew->WLC = board->WLC;
		boardNew->onPassantField=board->onPassantField;
		
		for (int i = 0; i < 6; i++)
		{
			boardNew->figure[WHITE][i] = board->figure[WHITE][i];
			boardNew->figure[BLACK][i] = board->figure[BLACK][i];
		}
		boardNew->occupancy[WHITE] = board->occupancy[WHITE];
		boardNew->occupancy[BLACK] = board->occupancy[BLACK];
		
		makeMove(*boardNew, m);
		int num2=generation(boardNew, (color == WHITE) ? BLACK : WHITE, max_depth, depth + 1);

		if (depth == 1)
		{
			int one = bitScanForward(m.move);
			int two = bitScanForward(m.move^fields[one]);
			Bitboard aa = ((board->whoToMove == WHITE) ? board->occupancy[WHITE] : board->occupancy[BLACK]);
			
			if (fields[one] & aa)
				swap(two, one);
			//printBitboard(m.move);
			char a= (char)(two % 8 + 97);
			int b = (int)(two / 8) + 1;
			char c = (char)(one % 8 + 97);
			int d = (int)(one / 8) + 1;
			int e = num2;
			output+=a+to_string(b)+c+to_string(d)+": "+to_string(e)+"\n";
			//cout << output <<" "<<m.type_piece << endl;
			//cout<<(char)(m.from % 8 + 97) << (int)(m.from / 8) + 1  << (char)(m.to % 8 + 97) << (int)(m.to / 8) + 1<<": "<<num2 << endl;
		}
		num += num2;

		delete boardNew;
	}
	return num;
}


// returns list of legal moves in given position for the color
void Game::moveGeneration2(Board board, Color color, list<Move>& legalMoves)
{
	list<Move> moves = list<Move>();

	// generating moves (not checking if move is legal)
	vector<Figure> figures = { ROOK, BISHOP, KNIGHT, QUEEN, KING };
	for (auto figure : figures) {
		generateMoves(board, color,figure,moves);
	}
	
	addCastleMove(board, color, moves);
	addPawnMoves(board,color,moves);

	// checking is move is legal 
	// (makes move, checks if there is a check, unmakes move, adds or not to legal moves)
	while (!moves.empty())
	{
		Move mm = moves.front();
		moves.pop_front();

		makeMove(board, mm);

		if (!isCheck(board, log2(board.figure[color][KING]), color))
			legalMoves.push_back(mm);

		unmakeMove(board, mm);
	}
}

void Game::addCastleMove(Board& board, Color color, list<Move>& moveList)
{
	if (color == WHITE)
	{
		if (isLegalCastle(board, color, SHORTCASTLE))
		{
			Move m;
			m.move = E1 | G1;
			m.type_piece = color * P + KING;
			m.move2 = H1 | F1;
			m.type_piece2 = color * P + ROOK;
			m.longCastle = board.WLC;
			m.enPassant = board.onPassantField;
			m.shortCastle = board.WSC;

			moveList.push_back(m);
			
		}
		if (isLegalCastle(board, color, LONGCASTLE))
		{
			Move m;
			m.move = E1 | C1;
			m.type_piece = color * P + KING;
			m.move2 = A1 | D1;
			m.type_piece2 = color * P + ROOK;
			m.longCastle = board.WLC;
			m.shortCastle = board.WSC;
			m.enPassant = board.onPassantField;

			moveList.push_back(m);
		}
	}
	else
	{
		if (isLegalCastle(board, color, SHORTCASTLE))
		{
			Move m;
			m.move = E8 | G8;
			m.type_piece = color * P + KING;
			m.move2 = H8 | F8;
			m.type_piece2 = color * P + ROOK;
			m.longCastle = board.BLC;
			m.enPassant = board.onPassantField;
			m.shortCastle = board.BSC;

			moveList.push_back(m);
		}
		if (isLegalCastle(board, color, LONGCASTLE))
		{
			Move m;
			m.move = E8 | C8;
			m.type_piece = color * P + KING;
			m.move2 = A8 | D8;
			m.type_piece2 = color * P + ROOK;

			m.longCastle = board.BLC;
			m.shortCastle = board.BSC;
			m.enPassant = board.onPassantField;

			moveList.push_back(m);
		}
	}

}

void Game::makeMove(Board& board, Move& move)
{
	board.figure[move.type_piece/P][move.type_piece%P] ^= move.move;
	board.figure[move.type_piece2 / P][move.type_piece2 % P] ^= move.move2;
	board.figure[move.type_piece3 / P][move.type_piece3 % P] ^= move.move3;

	changeOnPassantMove(board, move);
	changeCastleRights(board, move);

	// TODO: make more efficient generating occupancy
	board.occupancy[WHITE] = 0;
	board.occupancy[BLACK] = 0;
	for (int i = 0; i < 6; i++)
	{
		board.occupancy[WHITE] |= board.figure[WHITE][i];
		board.occupancy[BLACK] |= board.figure[BLACK][i];
	}
}

void Game::unmakeMove(Board& board, Move& move)
{
	if (move.type_piece < 6) // white move
	{
		board.WLC = move.longCastle;
		board.WSC = move.shortCastle;
	}
	else
	{
		board.BLC = move.longCastle;
		board.BSC = move.shortCastle;
	}
	board.figure[move.type_piece / P][move.type_piece % P] ^= move.move;
	board.figure[move.type_piece2 / P][move.type_piece2 % P] ^= move.move2;
	board.figure[move.type_piece3 / P][move.type_piece3 % P] ^= move.move3;

	board.onPassantField = move.enPassant;

	// TODO: make more efficient generating occupancy
	board.occupancy[WHITE] = 0;
	board.occupancy[BLACK] = 0;
	for (int i = 0; i < 6; i++)
	{
		board.occupancy[WHITE] |= board.figure[WHITE][i];
		board.occupancy[BLACK] |= board.figure[BLACK][i];
	}
}

void Game::changeCastleRights(Board& board, Move& move)
{
	if (!move.longCastle && !move.shortCastle) return;
	
	// checking rights to castle
	if (((int)(move.type_piece / P)) == WHITE)
	{
		
		if ((move.type_piece % P) == KING)
		{
			board.WLC = false;
			board.WSC = false;
		}
		else if ((move.type_piece % P) == ROOK)
		{
			if (move.move & A1)
				board.WLC = false;
			else if (move.move & H1)
				board.WSC = false;
		}
	}
	else
	{
		if ((move.type_piece % P) == KING)
		{
			board.BLC = false;
			board.BSC = false;
		}
		else if ((move.type_piece % P) == ROOK)
		{
			if (move.move & A8)
				board.BLC = false;
			else if (move.move & H8)
				board.BSC = false;
		}
	}
}

void Game::changeOnPassantMove(Board& board, Move& move)
{
	board.onPassantField = 64;
	if (move.type_piece == WHITE*P+PAWN || move.type_piece == BLACK*P+PAWN)
	{
		int one = bitScanForward(move.move);
		int two = bitScanForward(move.move^fields[one]);
		//cout << one << " " << two << endl;
		if (abs(one - two) == 16)
		{
			board.onPassantField = (one+two)/2;
			
		}
		
	}	
}

