#include "UciTranslator.h"

int MAX_TIME = 10;

string UciTranslator::TranslateMove(Board& board, Move& m)
{
	int one = bitScanForward(m.move);
	int two = bitScanForward(m.move ^ fields[one]);
	Bitboard aa = ((board.whoToMove == WHITE) ? board.occupancy[WHITE] : board.occupancy[BLACK]);

	if (fields[one] & aa)
		swap(two, one);
	
	char a = (char)(two % 8 + 97);
	int b = (int)(two / 8) + 1;
	char c = (char)(one % 8 + 97);
	int d = (int)(one / 8) + 1;

	string promotion = "";
	if (m.move3 != 0)
	{
		switch (m.type_piece3)
		{
			case 0:
				promotion = "r";
				break;
			case 1:
				promotion = "b";
				break;
			case 2:
				promotion = "b";
				break;
			case 4:
				promotion = "q";
				break;
		}
			
	}
	
	return a + to_string(b) + c + to_string(d)+promotion;
}

Board* UciTranslator::boardFromFEN(string fen)
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

int UciTranslator::goPerft(int depth)
{
	int nodes = MoveGeneration::goPerft(board, board.whoToMove, depth);
	cout << "Number of nodes: " << nodes << endl;
	return nodes;
}

void UciTranslator::showBoard()
{
	printBoard(board);
}

void UciTranslator::readBoard(Board& b)
{
	board = Board(b);
}

void UciTranslator::getBestMove(int depth, int maxTime)
{
	auto start = std::chrono::high_resolution_clock::now();

	Move bestMove=Game::PickBestMove(board, board.whoToMove,depth, maxTime);
	
	auto stop = std::chrono::high_resolution_clock::now();
	double seconds = (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000000;

	
	cout << "Time: " << seconds << endl << endl;

	cout << "bestmove " << TranslateMove(board,bestMove)<<endl;
}

void UciTranslator::startComunication()
{
	cout << "Kuba Engine :)" << endl;
	string command;
	
	UciTranslator tr=UciTranslator();
	
	do
	{
		cin >> command;

		if (command == "position")
		{
			cin >> command;
			if (command == "fen")
			{
				string fen = "";
				for (int i = 0; i < 6; i++)
				{
					cin >> command;
					fen += command+" ";
				}
				
				Board* b = UciTranslator::boardFromFEN(fen);
				tr.readBoard(*b);
			}
			else if (command == "startpos")
			{
				string startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
				Board* b = UciTranslator::boardFromFEN(startpos);
				tr.readBoard(*b);
			}
		}
		else if (command == "d")
		{
			tr.showBoard();
		}
		else if (command == "uci")
		{
			cout << "uciok" << endl;
		}
		else if (command == "isready")
		{
			cout << "readyok" << endl;
		}
		else if (command == "go")
		{
			cin >> command;
			if (command == "perft")
			{
				cin >> command;
				int depth=stoi(command);
				tr.goPerft(depth);
			}
			else if (command == "depth")
			{
				cin >> command;
				int depth = stoi(command);
				cout << depth << endl;
				tr.getBestMove(depth,MAX_TIME);
				tr.quit();
			}
		}
		else if (command == "quit")
		{
			tr.quit();
		}

	} while (true);

	
}
void UciTranslator::quit()
{
	exit(0);
}