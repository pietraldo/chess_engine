#include "Game.h"


bool Game::isCheck(Board board, int kingField, Color kingColor)
{
	Bitboard occ = board.figure[blackOccupancy] | board.figure[whiteOccupancy];
	if (kingColor == WHITE)
	{
		// rook
		Bitboard mask = occ&rookMask[kingField];
		Bitboard attackRook=GamePrepare::attackRook[kingField][(mask*GamePrepare::magicRook[kingField]) >> (M - GamePrepare::rookShifts[kingField])];
		if ((attackRook & board.figure[blackRook]) != 0|| (attackRook & board.figure[blackQueen]) != 0) return true;
		
		// bishop
		mask = occ & bishopMask[kingField];
		Bitboard attackBishop = GamePrepare::attacBishop[kingField][(mask * GamePrepare::magicBishop[kingField]) >> (M - GamePrepare::bishopShifts[kingField])];
		if (((attackBishop & board.figure[blackBishop ]) != 0) || ((attackBishop & board.figure[blackQueen]) != 0)) return true;
		
		// knight
		if ((GamePrepare::attackKnight[kingField] & board.figure[blackKnight]) != 0) return true;

		// pawn
		if ((GamePrepare::attackPawnW[kingField] & board.figure[blackPawn]) != 0) return true;

		// king
		if ((GamePrepare::attackKing[kingField] & board.figure[blackKing]) != 0) return true;

		return false;
	}

	// rook
	Bitboard mask = occ & rookMask[kingField];
	Bitboard attackRook = GamePrepare::attackRook[kingField][(mask * GamePrepare::magicRook[kingField]) >> (M - GamePrepare::rookShifts[kingField])];
	if ((attackRook & board.figure[whiteRook]) != 0 || (attackRook & board.figure[whiteQueen]) != 0) return true;

	// bishop
	mask = occ & bishopMask[kingField];
	Bitboard attackBishop = GamePrepare::attacBishop[kingField][(mask * GamePrepare::magicBishop[kingField]) >> (M - GamePrepare::bishopShifts[kingField])];
	if (((attackBishop & board.figure[whiteBishop]) != 0) || ((attackBishop & board.figure[whiteQueen]) != 0)) return true;

	// knight
	if ((GamePrepare::attackKnight[kingField] & board.figure[whiteKnight]) != 0) return true;

	// pawn
	if ((GamePrepare::attackPawnB[kingField] & board.figure[whitePawn]) != 0) return true;

	// king
	if ((GamePrepare::attackKing[kingField] & board.figure[whiteKing]) != 0) return true;
	
	return false;
}

bool Game::isLegalCastleWhiteShort(Board b)
{
	// checking if rook and king are on correct fields
	if ((b.figure[whiteRook] & H1) == 0 || (b.figure[whiteKing] & E1) == 0) return false;
	
	// checking if field F1 and G1 are under attack
	if (isCheck(b, 5, WHITE) || isCheck(b, 6, WHITE)) return false;

	return true;
}
bool Game::isLegalCastleWhiteLong(Board b)
{
	// checking if rook and king are on correct fields
	if ((b.figure[whiteRook] & A1) == 0 || (b.figure[whiteKing] & E1) == 0) return false;

	// checking if field F1 and G1 are under attack
	if (isCheck(b, 2, WHITE) || isCheck(b, 3, WHITE)) return false;

	return true;
}
bool Game::isLegalCastleBlackShort(Board b)
{
	// checking if rook and king are on correct fields
	if ((b.figure[blackRook] & H8) == 0 || (b.figure[blackKing] & E8) == 0) return false;

	// checking if field F1 and G1 are under attack
	if (isCheck(b, 61, BLACK) || isCheck(b, 62, BLACK)) return false;

	return true;
}
bool Game::isLegalCastleBlackLong(Board b)
{
	if ((b.figure[blackRook] & A8) == 0 || (b.figure[blackKing] & E8) == 0) return false;

	// checking if field F1 and G1 are under attack
	if (isCheck(b, 58, BLACK) || isCheck(b, 59, BLACK)) return false;

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
			board->figure[blackRook] |= fields[row*N+column];
			break;
		case 'R':
			board->figure[whiteRook] |= fields[row*N+column];
			break;
		case 'q':
			board->figure[blackQueen] |= fields[row*N+column];
			break;
		case 'Q':
			board->figure[whiteQueen] |= fields[row*N+column];
			break;
		case 'k':
			board->figure[blackKing] |= fields[row*N+column];
			break;
		case 'K':
			board->figure[whiteKing] |= fields[row*N+column];
			break;
		case 'p':
			board->figure[blackPawn] |= fields[row*N+column];
			break;
		case 'P':
			board->figure[whitePawn] |= fields[row*N+column];
			break;
		case 'N':
			board->figure[whiteKnight] |= fields[row*N+column];
			break;
		case 'n':
			board->figure[blackKnight] |= fields[row*N+column];
			break;
		case 'B':
			board->figure[whiteBishop] |= fields[row*N+column];
			break;
		case 'b':
			board->figure[blackBishop] |= fields[row*N+column];
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
		board->figure[whiteOccupancy] |= board->figure[i];
		board->figure[blackOccupancy] |= board->figure[6 + i];
	}
	return board;
}

int bitScanForward(Bitboard bb)
{
	unsigned long index;
	_BitScanForward64(&index, bb);
	return index;
}
 

int Game::count = 0;
string Game::output = "";

int Game::generation(Board* board, Color color,int max_depth, int depth)
{
	if (depth-1 == max_depth) return 1;
	
	list<Move> legalMoves = list<Move>();

	moveGeneration(*board, color, legalMoves);
	
	int num = 0;
	for (auto m : legalMoves)
	{
		Board* boardNew= new Board();
		boardNew->BLC = board->BLC;
		boardNew->BSC = board->BSC;
		boardNew->WSC = board->WSC;
		boardNew->WLC = board->WLC;
		boardNew->onPassantField=board->onPassantField;
		
		for (int i = 0; i < 14; i++)
			boardNew->figure[i] = board->figure[i];

		makeMove(*boardNew,m);
		int num2=generation(boardNew, (color == WHITE) ? BLACK : WHITE, max_depth, depth + 1);

		if (depth == 1)
		{
			int one = bitScanForward(m.move);
			int two = bitScanForward(m.move^fields[one]);
			if (fields[one] & ((board->whoToMove == WHITE) ? board->figure[whiteOccupancy] : board->figure[blackOccupancy]))
				swap(two, one);
			//printBitboard(m.move);
			char a= (char)(two % 8 + 97);
			int b = (int)(two / 8) + 1;
			char c = (char)(one % 8 + 97);
			int d = (int)(one / 8) + 1;
			int e = num2;
			output+=a+to_string(b)+c+to_string(d)+": "+to_string(e)+"\n";
			//cout<<(char)(m.from % 8 + 97) << (int)(m.from / 8) + 1  << (char)(m.to % 8 + 97) << (int)(m.to / 8) + 1<<": "<<num2 << endl;
		}
		num += num2;

		delete boardNew;
	}
	return num;
}

void Game::moveGeneration(Board board, Color color, list<Move>& legalMoves)
{
	
	list<Move> moves = list<Move>();
	
	if (color==WHITE)
	{
		moveGenerationKingWhite(board, moves, board.figure[whiteKing]);
		moveGenerationRookWhite(board, moves, board.figure[whiteRook]);
		moveGenerationBishopWhite(board, moves, board.figure[whiteBishop]);
		moveGenerationPawnWhite(board, moves, board.figure[whitePawn]);
		moveGenerationKnightWhite(board, moves, board.figure[whiteKnight]);
		moveGenerationRookWhite(board, moves, board.figure[whiteQueen]);
		moveGenerationBishopWhite(board, moves, board.figure[whiteQueen]);
	}
	else
	{
		moveGenerationKingBlack(board, moves, board.figure[blackKing]);
		moveGenerationRookBlack(board, moves, board.figure[blackRook]);
		moveGenerationBishopBlack(board, moves, board.figure[blackBishop]);
		moveGenerationPawnBlack(board, moves, board.figure[blackPawn]);
		moveGenerationKnightBlack(board, moves, board.figure[blackKnight]);
		moveGenerationRookBlack(board, moves, board.figure[blackQueen]);
		moveGenerationBishopBlack(board, moves, board.figure[blackQueen]);
	}

	if (color==WHITE)
	{
		while (!moves.empty())
		{
			Move mm = moves.front();
			moves.pop_front();

			makeMove(board, mm);
			
			
			if (!isCheck(board, log2(board.figure[whiteKing]), WHITE))
				legalMoves.push_back(mm);
			
			unmakeMove(board, mm);
		}
	}
	else
	{
		while (!moves.empty())
		{
			Move mm = moves.front();
			moves.pop_front();
			
			//printBoard(board);cout << "a";
			//cout  << (char)(mm.from % 8 + 65) << (int)(mm.from / 8) + 1 << " -> " << (char)(mm.to % 8 + 65) << (int)(mm.to / 8) + 1 << endl;
			makeMove(board, mm);
			
			
			if (!isCheck(board, log2(board.figure[blackKing]), BLACK))
				legalMoves.push_back(mm);
			unmakeMove(board, mm);
		}
	}	
	
}


void Game::test(Board& board)
{
	list<Move> history = list<Move>();
	while (1)
	{
		list<Move> moves = list<Move>();

		moveGenerationKingWhite(board, moves, board.figure[whiteKing]);
		moveGenerationRookWhite(board, moves, board.figure[whiteRook]);
		moveGenerationBishopWhite(board, moves, board.figure[whiteBishop]);
		moveGenerationPawnWhite(board, moves, board.figure[whitePawn]);
		moveGenerationKnightWhite(board, moves, board.figure[whiteKnight]);
		moveGenerationRookWhite(board, moves, board.figure[whiteQueen]);
		moveGenerationBishopWhite(board, moves, board.figure[whiteQueen]);

		int i = 0;
		for (const auto& m : moves)
		{
			//cout << setw(3) << ++i << " " << "Piece: " << m.type_piece << ", move type: " << m.moveType << ", " << (char)(m.from % 8 + 65) << (int)(m.from / 8) + 1 << " -> " << (char)(m.to % 8 + 65) << (int)(m.to / 8) + 1 << endl;
		}

		cout << "Ruchow lacznie: " << moves.size() << endl;
		cout << "Castle rights white: " << board.WSC << " " << board.WLC << endl;
		printBoard(board);

		int number;

		cin >>number;
		if (number != 0)
		{
			for (auto& m : moves)
			{
				--number;
				if (number == 0)
				{
					makeMove(board, m);
					history.push_back(m);
				}

			}
		}
		else
		{
			if (history.empty()) continue;
			Move m = history.back();
			unmakeMove(board, m);
			history.pop_back();
		}

	}

}

void Game::moveGenerationRookWhite(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	while (figure)
	{
		// geting one rook
		int rookIndex = bitScanForward(figure);
		// part of key for hashing
		Bitboard mask = occ & rookMask[rookIndex];
		// getting attacks
		Bitboard attackRook = GamePrepare::attackRook[rookIndex][(mask * GamePrepare::magicRook[rookIndex]) >> (M - GamePrepare::rookShifts[rookIndex])];
		// substracting attack on own piece
		attackRook -= attackRook & board.figure[whiteOccupancy];

		// adding to list all moves for this rook
		while (attackRook)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attackRook);

			Move m;
			
			m.move = fields[rookIndex]|fields[index];
			m.type_piece = (board.figure[whiteRook] & figure) ? whiteRook : whiteQueen;
			m.longCastle = board.WLC;
			m.shortCastle = board.WSC;
			m.enPassant = board.onPassantField;

			
			if (fields[index] & board.figure[blackOccupancy]) //capture move
			{
				m.move2 = fields[index];
				for (int i = 0; i < 6; i++)
				{
					if (fields[index] & board.figure[i + 6])
					{
						m.type_piece2 = i + 6;
						break;
					}
				}
			}
			
			moves.push_back(m);
			// substracting added move
			attackRook &= attackRook - 1;
		}
		// substracting rook which moves was generatend
		figure &= figure - 1;
	}
}

void Game::moveGenerationBishopWhite(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	while (figure)
	{
		// geting one rook
		int bishopIndex = bitScanForward(figure);
		// part of key for hashing
		Bitboard mask = occ & bishopMask[bishopIndex];
		// getting attacks
		Bitboard attackBishop = GamePrepare::attacBishop[bishopIndex][(mask * GamePrepare::magicBishop[bishopIndex]) >> (M - GamePrepare::bishopShifts[bishopIndex])];
		// substracting attack on own piece
		attackBishop -= attackBishop & board.figure[whiteOccupancy];

		// adding to list all moves for this rook
		while (attackBishop)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attackBishop);

			Move m;

			m.move = fields[bishopIndex] | fields[index];
			m.type_piece = (board.figure[whiteBishop] & figure) ? whiteBishop : whiteQueen;
			m.longCastle = board.WLC;
			m.shortCastle = board.WSC;
			m.enPassant = board.onPassantField;

			if (fields[index] & board.figure[blackOccupancy]) //capture move
			{
				m.move2 = fields[index];
				bool was = false;
				for (int i = 0; i < 6; i++)
				{
					if (fields[index] & board.figure[i + 6])
					{
						m.type_piece2 = i + 6;
						break;
					}
					
				}
			}
			
			moves.push_back(m);
			// substracting added move
			attackBishop &= attackBishop - 1;
		}
		// substracting rook which moves was generatend
		figure &= figure - 1;
	}
}

void Game::moveGenerationPawnWhite(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	while (figure)
	{
		// geting one rook
		int pawnIndex = bitScanForward(figure);
		
		// getting attacks
		Bitboard attackPawn = GamePrepare::attackPawnW[pawnIndex];
		Bitboard movePawn = GamePrepare::pawnMovesW[pawnIndex];

		if ((log2(movePawn & occ) - pawnIndex) == 16)
			movePawn^= movePawn & occ;
		else if ((log2(movePawn & occ) - pawnIndex) >= 8)
			movePawn = 0;
		
		// enPassant capture
		if (fields[board.onPassantField] & attackPawn)
		{
			Move m;
			m.move = fields[pawnIndex] | fields[board.onPassantField];
			m.type_piece = whitePawn;
			m.move2 = fields[board.onPassantField - 8];
			m.type_piece2 = blackPawn;
			m.longCastle = board.WLC;
			m.shortCastle = board.WSC;
			m.enPassant = board.onPassantField;
			moves.push_back(m);
		}



		// substracting attack on own piece
		attackPawn  &= board.figure[blackOccupancy];

		// adding to list all moves for this rook
		while (attackPawn)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attackPawn);

			Move m;
			m.move = fields[pawnIndex] | fields[index];
			m.type_piece = whitePawn;
			m.longCastle = board.WLC;
			m.shortCastle = board.WSC;
			m.enPassant = board.onPassantField;
			
			
			for (int i = 0; i < 6; i++)
			{
				if (fields[index] & board.figure[i + 6])
				{
					m.type_piece2 = i + 6;
					m.move2 = fields[index];
					break;
				}
			}
			if (index >= 56)
			{
				m.move = fields[pawnIndex];
				m.move3 = fields[index];
				m.type_piece3 = whiteQueen;
				moves.push_back(m);
				m.type_piece3 = whiteRook;
				moves.push_back(m);
				m.type_piece3 = whiteBishop;
				moves.push_back(m);
				m.type_piece3 = whiteKnight;
				moves.push_back(m);
			}
			else
			{
				moves.push_back(m);
			}
				
			
			
			// substracting added move
			attackPawn &= attackPawn - 1;
		}
		while (movePawn)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(movePawn);

			Move m;
			m.move = fields[index] | fields[pawnIndex];
			m.type_piece = whitePawn;
			m.longCastle = board.WLC;
			m.shortCastle = board.WSC;
			m.enPassant = board.onPassantField;
			if (index >= 56)
			{
				m.move = fields[pawnIndex];
				m.move3 = fields[index];
				m.type_piece3 = whiteQueen;
				moves.push_back(m);
				m.type_piece3 = whiteRook;
				moves.push_back(m);
				m.type_piece3 = whiteBishop;
				moves.push_back(m);
				m.type_piece3 = whiteKnight;
				moves.push_back(m);
			}
			else
			{
				moves.push_back(m);
			}
						
			// substracting added move
			movePawn &= movePawn - 1;
		}	

		// substracting rook which moves was generatend
		figure &= figure - 1;
	}
}

void Game::moveGenerationKnightWhite(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	while (figure)
	{
		// geting one rook
		int knightIndex = bitScanForward(figure);
		
		// getting attacks
		Bitboard attackKnight = GamePrepare::attackKnight[knightIndex];
		// substracting attack on own piece
		attackKnight -= attackKnight & board.figure[whiteOccupancy];

		// adding to list all moves for this rook
		while (attackKnight)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attackKnight);

			Move m;
			m.move = fields[index] | fields[knightIndex];
			m.type_piece = whiteKnight;
			m.longCastle = board.WLC;
			m.shortCastle = board.WSC;
			m.enPassant = board.onPassantField;
			if (fields[index] & board.figure[blackOccupancy]) //capture move
			{
				m.move2 = fields[index];
				for (int i = 0; i < 6; i++)
				{
					if (fields[index] & board.figure[i + 6])
					{
						m.type_piece2 = i + 6;
						break;
					}
				}
			}
			
			moves.push_back(m);
			// substracting added move
			attackKnight &= attackKnight - 1;
		}
		// substracting rook which moves was generatend
		figure &= figure - 1;
	}
}

void Game::moveGenerationKingWhite(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	
	// geting one rook
	int kingIndex = bitScanForward(figure);

	// getting attacks
	Bitboard attackKing = GamePrepare::attackKing[kingIndex];
	// substracting attack on own piece
	attackKing -= attackKing & board.figure[whiteOccupancy];

	// adding to list all moves for this rook
	while (attackKing)
	{
		// geting index of field that rook is attacking
		int index = bitScanForward(attackKing);

		Move m;
		m.move = fields[index] | fields[kingIndex];
		m.type_piece = whiteKing;
		m.longCastle = board.WLC;
		m.shortCastle = board.WSC;
		m.enPassant = board.onPassantField;
		if (fields[index] & board.figure[blackOccupancy]) //capture move
		{
			m.move2 = fields[index];
			for (int i = 0; i < 6; i++)
			{
				if (fields[index] & board.figure[i + 6])
				{
					m.type_piece2 = i + 6;
					break;
				}
			}
		}
		
		moves.push_back(m);
		// substracting added move
		attackKing &= attackKing - 1;
	}
	
	if (board.WLC && isLegalCastleWhiteLong(board) && !((B1|C1|D1)&occ) && (board.figure[whiteRook] & A1) && !isCheck(board, 4, WHITE))
	{
		Move m;
		m.move = fields[kingIndex] | C1;
		m.type_piece = whiteKing;
		m.move2 = A1 | D1;
		m.type_piece2 = whiteRook;
		m.longCastle = board.WLC;
		m.shortCastle = board.WSC;
		m.enPassant = board.onPassantField;

		moves.push_back(m);
	}
	if (board.WSC && isLegalCastleWhiteShort(board) && !((G1 | F1) & occ) && (board.figure[whiteRook] & H1) && !isCheck(board, 4, WHITE))
	{
		Move m;
		m.move = fields[kingIndex] | G1;
		m.type_piece = whiteKing;
		m.move2 = H1 | F1;
		m.type_piece2 = whiteRook;
		m.longCastle = board.WLC;
		m.enPassant = board.onPassantField;
		m.shortCastle = board.WSC;
		
		moves.push_back(m);
	}
}

void Game::moveGenerationRookBlack(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	while (figure)
	{
		// geting one rook
		int rookIndex = bitScanForward(figure);
		// part of key for hashing
		Bitboard mask = occ & rookMask[rookIndex];
		// getting attacks
		Bitboard attackRook = GamePrepare::attackRook[rookIndex][(mask * GamePrepare::magicRook[rookIndex]) >> (M - GamePrepare::rookShifts[rookIndex])];
		// substracting attack on own piece
		attackRook -= attackRook & board.figure[blackOccupancy];

		// adding to list all moves for this rook
		while (attackRook)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attackRook);

			Move m;
			m.move = fields[rookIndex] | fields[index];
			m.type_piece = (board.figure[blackRook] & figure) ? blackRook : blackQueen;
			m.longCastle = board.BLC;
			m.shortCastle = board.BSC;
			m.enPassant = board.onPassantField;
			
			if (fields[index] & board.figure[whiteOccupancy]) //capture move
			{
				m.move2 = fields[index];
				for (int i = 0; i < 6; i++)
				{
					if (fields[index] & board.figure[i ])
					{
						m.type_piece2 = i ;
						break;
					}
				}
			}
			
			moves.push_back(m);
			// substracting added move
			attackRook &= attackRook - 1;
		}
		// substracting rook which moves was generatend
		figure &= figure - 1;
	}
}

void Game::moveGenerationBishopBlack(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	while (figure)
	{
		// geting one rook
		int bishopIndex = bitScanForward(figure);
		// part of key for hashing
		Bitboard mask = occ & bishopMask[bishopIndex];
		// getting attacks
		Bitboard attackBishop = GamePrepare::attacBishop[bishopIndex][(mask * GamePrepare::magicBishop[bishopIndex]) >> (M - GamePrepare::bishopShifts[bishopIndex])];
		// substracting attack on own piece
		attackBishop -= attackBishop & board.figure[blackOccupancy];

		// adding to list all moves for this rook
		while (attackBishop)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attackBishop);

			Move m;
			m.move = fields[bishopIndex] | fields[index];
			m.type_piece = (board.figure[blackBishop] & figure) ? blackBishop : blackQueen;
			m.longCastle = board.BLC;
			m.shortCastle = board.BSC;
			m.enPassant = board.onPassantField;
			
			if (fields[index] & board.figure[whiteOccupancy]) //capture move
			{
				m.move2 = fields[index];
				for (int i = 0; i < 6; i++)
				{
					if (fields[index] & board.figure[i ])
					{
						m.type_piece2 = i ;
						break;
					}
				}
			}
			moves.push_back(m);
			// substracting added move
			attackBishop &= attackBishop - 1;
		}
		// substracting rook which moves was generatend
		figure &= figure - 1;
	}
}

void Game::moveGenerationPawnBlack(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	while (figure)
	{
		// geting one rook
		int pawnIndex = bitScanForward(figure);

		// getting attacks
		Bitboard attackPawn = GamePrepare::attackPawnB[pawnIndex];
		Bitboard movePawn = GamePrepare::pawnMovesB[pawnIndex];

		
		// delete if is obsticle before pawn
		movePawn -= movePawn & occ;
		// delete  move of two fields because on first is obsticle
		if ((pawnIndex-log2(movePawn)) == 16)
			movePawn =0;
		
		

		if (fields[board.onPassantField] & attackPawn)
		{
			Move m;
			m.move = fields[pawnIndex] | fields[board.onPassantField];
			m.type_piece = blackPawn;
			m.move2 = fields[board.onPassantField + 8];
			m.type_piece2 = whitePawn;
			m.longCastle = board.BLC;
			m.shortCastle = board.BSC;
			m.enPassant = board.onPassantField;
			moves.push_back(m);
		}



		// substracting attack on own piece
		attackPawn &= board.figure[whiteOccupancy];

		// adding to list all moves for this rook
		while (attackPawn)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attackPawn);

			Move m;
			m.move = fields[pawnIndex] | fields[index];
			m.type_piece = blackPawn;
			m.longCastle = board.BLC;
			m.shortCastle = board.BSC;
			m.enPassant = board.onPassantField;


			for (int i = 0; i < 6; i++)
			{
				if (fields[index] & board.figure[i])
				{
					m.type_piece2 = i ;
					m.move2 = fields[index];
					break;
				}
			}
			if (index <=7)
			{
				m.move = fields[pawnIndex];
				m.move3 = fields[index];
				m.type_piece3 = blackQueen;
				moves.push_back(m);
				m.type_piece3 = blackRook;
				moves.push_back(m);
				m.type_piece3 = blackBishop;
				moves.push_back(m);
				m.type_piece3 = blackKnight;
				moves.push_back(m);
			}
			else
			{
				moves.push_back(m);
			}



			// substracting added move
			attackPawn &= attackPawn - 1;
		}
		while (movePawn)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(movePawn);

			Move m;
			m.move = fields[pawnIndex] | fields[index];
			m.type_piece = blackPawn;
			m.longCastle = board.BLC;
			m.shortCastle = board.BSC;
			m.enPassant = board.onPassantField;
			if (index <=7)
			{
				m.move = fields[pawnIndex];
				m.move3 = fields[index];
				m.type_piece3 = blackQueen;
				moves.push_back(m);
				m.type_piece3 = blackRook;
				moves.push_back(m);
				m.type_piece3 = blackBishop;
				moves.push_back(m);
				m.type_piece3 = blackKnight;
				moves.push_back(m);
			}
			else
			{
				moves.push_back(m);
			}



			// substracting added move
			movePawn &= movePawn - 1;
		}


		// substracting rook which moves was generatend
		figure &= figure - 1;
	}
}

void Game::moveGenerationKnightBlack(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];

	while (figure)
	{
		// geting one rook
		int knightIndex = bitScanForward(figure);

		// getting attacks
		Bitboard attackKnight = GamePrepare::attackKnight[knightIndex];
		// substracting attack on own piece
		attackKnight -= attackKnight & board.figure[blackOccupancy];

		// adding to list all moves for this rook
		while (attackKnight)
		{
			// geting index of field that rook is attacking
			int index = bitScanForward(attackKnight);

			Move m;
			m.move = fields[index] | fields[knightIndex];
			m.type_piece = blackKnight;
			m.longCastle = board.BLC;
			m.shortCastle = board.BSC;
			m.enPassant = board.onPassantField;
			
			if (fields[index] & board.figure[whiteOccupancy]) //capture move
			{
				m.move2 = fields[index];
				for (int i = 0; i < 6; i++)
				{
					if (fields[index] & board.figure[i])
					{
						m.type_piece2 = i;
						break;
					}
				}
			}
			
			moves.push_back(m);
			// substracting added move
			attackKnight &= attackKnight - 1;
		}
		// substracting rook which moves was generatend
		figure &= figure - 1;
	}
}

void Game::moveGenerationKingBlack(Board& board, list<Move>& moves, Bitboard figure)
{
	Bitboard occ = board.figure[whiteOccupancy] | board.figure[blackOccupancy];


	// geting one rook
	int kingIndex = bitScanForward(figure);

	// getting attacks
	Bitboard attackKing = GamePrepare::attackKing[kingIndex];
	// substracting attack on own piece
	attackKing -= attackKing & board.figure[blackOccupancy];

	// adding to list all moves for this rook
	while (attackKing)
	{
		// geting index of field that rook is attacking
		int index = bitScanForward(attackKing);

		Move m;
		m.move = fields[index] | fields[kingIndex];
		m.type_piece = blackKing;
		m.longCastle = board.BLC;
		m.shortCastle = board.BSC;
		m.enPassant = board.onPassantField;
		
		if (fields[index] & board.figure[whiteOccupancy]) //capture move
		{
			m.move2 = fields[index];
			for (int i = 0; i < 6; i++)
			{
				if (fields[index] & board.figure[i])
				{
					m.type_piece2 = i;
					break;
				}
			}
		}
		
		moves.push_back(m);
		// substracting added move
		attackKing &= attackKing - 1;
	}

	if (board.BLC && isLegalCastleBlackLong(board) && !((B8 | C8 | D8) & occ) && (board.figure[blackRook]&A8) && !isCheck(board, 60, BLACK))
	{
		Move m;
		m.move = fields[kingIndex] | C8;
		m.type_piece = blackKing;
		m.move2 = A8 | D8;
		m.type_piece2 = blackRook;

		m.longCastle = board.BLC;
		m.shortCastle = board.BSC;
		m.enPassant = board.onPassantField;
	
		moves.push_back(m);
	}
	if (board.BSC && isLegalCastleBlackShort(board) && !((G8 | F8) & occ) && (board.figure[blackRook] & H8) && !isCheck(board,60,BLACK))
	{
		Move m;
		m.move = fields[kingIndex] | G8;
		m.type_piece = blackKing;
		m.move2 = H8 | F8;
		m.type_piece2 = blackRook;
		m.longCastle = board.BLC;
		m.enPassant = board.onPassantField;
		m.shortCastle = board.BSC;
		
		moves.push_back(m);
	}
}


void Game::makeMove(Board& board, Move& move)
{
	board.figure[move.type_piece] ^= move.move;
	board.figure[move.type_piece2] ^= move.move2;
	board.figure[move.type_piece3] ^= move.move3;

	changeOnPassantMove(board, move);

	// TODO: make more efficient generating occupancy
	board.figure[whiteOccupancy] = 0;
	board.figure[blackOccupancy] = 0;
	for (int i = 0; i < 6; i++)
	{
		board.figure[whiteOccupancy] |= board.figure[i];
		board.figure[blackOccupancy] |= board.figure[6 + i];
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
	board.figure[move.type_piece] ^= move.move;
	board.figure[move.type_piece2] ^= move.move2;
	board.figure[move.type_piece3] ^= move.move3;

	board.onPassantField = move.enPassant;

	// TODO: make more efficient generating occupancy
	board.figure[whiteOccupancy] = 0;
	board.figure[blackOccupancy] = 0;
	for (int i = 0; i < 6; i++)
	{
		board.figure[whiteOccupancy] |= board.figure[i];
		board.figure[blackOccupancy] |= board.figure[6 + i];
	}
}

void Game::changeCastleRights(Board& board, Move& move)
{
	if (!move.longCastle && !move.shortCastle) return;

	// checking rights to castle
	if (move.type_piece == whiteKing)
	{
		board.WLC = false;
		board.WSC = false;
	}
	else if (move.type_piece == blackKing)
	{
		board.BLC = false;
		board.BSC = false;
	}
	else if(move.type_piece==blackRook)
	{
		if (move.move&A8)
			board.BLC = false;
		else if (move.move&H8)
			board.BSC = false;
	}
	else if (move.type_piece == whiteRook)
	{
		if (move.move&A1)
			board.WLC = false;
		else if (move.move&H8)
			board.WSC = false;
	}
}

void Game::changeOnPassantMove(Board& board, Move& move)
{
	board.onPassantField = 64;
	if (move.type_piece == 0 || move.type_piece == 6)
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

