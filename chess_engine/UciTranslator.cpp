#include "UciTranslator.h"


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
	
	Game::maxTime = maxTime;
	Game::start = start;
	Move bestMove=Game::PickBestMove(board, board.whoToMove,depth);

	auto stop = std::chrono::high_resolution_clock::now();
	double seconds = (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000000;

	cout << " " << Game::num << endl;
	cout << "Time: " << seconds << endl << endl;

	cout << "bestmove: " << TranslateMove(board,bestMove)<<endl;
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
				cin >> command;
				Board* b = MoveGeneration::boardFromFEN(command);
				tr.readBoard(*b);
			}
		}
		else if (command == "d")
		{
			tr.showBoard();
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
				tr.getBestMove(depth,30);
			}
		}

	} while (true);

	
}