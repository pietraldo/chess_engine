#include "Test.h"
#include <sstream>
#include <queue>
#include <string>
#include <algorithm>
#include <chrono>


using namespace std;
using namespace std::chrono;

string formatWithDots(int number) {
	std::string result;
	std::string numStr = std::to_string(number);
	int count = 0;

	for (int i = numStr.length() - 1; i >= 0; i--) {
		result = numStr[i] + result;
		count++;

		if (count == 3 && i > 0) {
			result = "." + result;
			count = 0;
		}
	}

	return result;
}

void printColoredResult(const std::string& result) {
	if (result == "OK") {
		// Print in green
		cout << "\033[32m" << result << "\033[0m" << std::endl;
	}
	else {
		// Print in red
		cout << "\033[31m" << result << "\033[0m" << std::endl;
	}
}

bool Test::test1()
{

	bool show=false;
	int depth = 4;
	/*string fen[] = { 
		"2k5/7R/8/8/1Q6/8/8/3K4 b - - 0 1",
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" ,
		"rnbqkbnr/p3ppp1/1p5p/3p4/2BP4/5Q2/PPP2PPP/RNB1K1NR w KQkq - 0 6",
		"rnbqkbnr/pPpppp1p/8/8/8/8/P1pPP2P/RNBQKBNR w KQkq - 0 1",
		"rnbqkbnr/pppppppp/8/3P4/8/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1",
		"rnb1kbnr/pppppppp/4q3/3P4/7P/3QK3/PPP2PPP/RNB2BNR w kq - 0 1",
		"rn2kbnr/pppppppp/8/3P2q1/1b5P/3Q4/PPPK1PPP/RNB2BNR w HAkq - 0 1",
		"rn2kbnr/pppppppp/5b2/3P4/3q3P/1N1Q4/PPPK1PPP/R1B2BNR b kq - 0 1",
		"r3k2r/p1pppppp/2np1bn1/3P4/1Q1q3P/1NB2N2/PPP2BPP/R3K2R b KQkq - 0 1",
		"r3k2r/p1pppppp/2np1bn1/3P4/1Q1q3P/1NB2N2/PPP2BPP/R3K2R w KQkq - 0 1",
		"8/1P1k4/8/3K4/8/7p/8/8 w - - 0 1",
		"8/8/8/8/R2pk3/8/2P5/2K5 w - - 0 1",
	};*/
	string fen[] = { 
		"7r/1p4pp/3kpn2/pR6/4p3/P7/1P4PP/2R3K1 w - - 0 26" 
		
	};
	
	long sumN = 0;
	double sumT = 0;
	for (int i = 0; i < size(fen); i++)
	{
		cout << "Position: " << i + 1 << endl;

		// stockfish response
		string stockfish = ReadyChessEngine::GiveAnswer(("position fen " + fen[i] + "\ngo perft " + to_string(depth) + "\n\quit\n").c_str());

		Board* board = MoveGeneration::boardFromFEN(fen[i]);
		
		MoveGeneration::output = "";
		auto start = high_resolution_clock::now();

		// my response
		int nodes = MoveGeneration::generation(board, board->whoToMove, depth);

		auto stop = high_resolution_clock::now();
		double seconds = (double)duration_cast<microseconds>(stop - start).count()/1000000;

		string myEngine = MoveGeneration::output;
		string stockfish_nodes=compareMoveGeneration(stockfish, myEngine, true);

		cout << "Nodes: " << formatWithDots(nodes) << endl;
		cout << "Seconds: " << seconds << endl;
		cout << "Nodes per second: " << formatWithDots(nodes / seconds) << endl<<endl;
		
		string result = (to_string(nodes) == stockfish_nodes) ? "OK" : "FAILED";
		cout << "Test: ";
		printColoredResult(result);
		cout << endl;

		sumN += nodes;
		sumT += seconds;
	}
	cout << endl << "Avarage nodes per second: "<<sumN / sumT << endl; // 498.000 nodes/s
	return true;
}

Test::Test()
{
	GamePrepare();
	//test3();
	//test1();
	//test4();
	//test5();
	//test2();
	//test7();
}


int bitScanForward2(Bitboard bb)
{
	unsigned long index;
	_BitScanForward64(&index, bb);
	return index;
}
bool Test::test3()
{
	Board* b = MoveGeneration::boardFromFEN("r1b1k1nr/1ppp1pp1/p1n1p1qp/8/3PPB2/P1PB1N2/2P2PPP/R2QK2R b KQkq - 1 9");
	cout << Game::Evaluate2(*b);
	return true;
}


bool Test::test4()
{
	GamePrepare();
	Board* board = MoveGeneration::boardFromFEN("rnb1k3/pppppppp/1r1q4/4b3/1B3N2/3Q2B1/PPPPPPPP/RN2KB1R w KQq - 0 1");
	printBoard(*board);

	/*list<Move> moveList = list<Move>();
	MoveGeneration::generateMovesNew(*board,BLACK, moveList);*/
	auto start = high_resolution_clock::now();
	Game::maxTime = 30;
	Game::start = start;
	
	cout << "Best move: " << Game::PickBestMove(*board, board->whoToMove, 5);
	auto stop = high_resolution_clock::now();
	double seconds = (double)duration_cast<microseconds>(stop - start).count() / 1000000;
	
	cout << " " << Game::num << endl;
	cout << "Time: " << seconds << endl << endl;

	
	return true;
}
bool Test::test7()
{
	Board* board = MoveGeneration::boardFromFEN("rnb1k3/pppppppp/1r1q4/4b3/1B3N2/3Q2B1/PPPPPPPP/RN2KB1R w KQq - 0 1");
	printBoard(*board);
	list<Move> moveList = list<Move>();
	Game::quisanceSearch(*board, board->whoToMove, -9999, 9999, 100, 0, moveList);
	for (auto a : moveList)
	{
		cout << a << " ";
	}
	return true;
}

bool Test::test5()
{
	/*GamePrepare();
	Board* board = MoveGeneration::boardFromFEN("r2q1rk1/pb1nppbp/1p1pPnp1/5P2/2PpP3/2NBB3/PP2N1PP/R2Q1RK1 w - - 0 11");
	printBoard(*board);

	upCutoff = Game::Evaluate(*board)+10;
	downCutoff = upCutoff-10;

	auto start = high_resolution_clock::now();
	cout<<gg(*board, board->whoToMove,-9999999, 99999999, 100,0);
	auto stop = high_resolution_clock::now();
	auto seconds = (double)duration_cast<microseconds>(stop - start).count() / 1000000;
	cout << "Time: " << seconds << endl << endl;*/

	return true;
}

bool Test::test6()
{
	GamePrepare();
	Board* board = MoveGeneration::boardFromFEN("rn1qk2r/5ppp/p3pn2/1p4B1/3P4/P1N2Q2/B4PPP/R4RK1 b kq - 0 13");
	//printBoard(*board);

	UciTranslator tr = UciTranslator();
	tr.readBoard(*board);

	
	/*auto start = high_resolution_clock::now();
	int nodes=tr.goPerft(4);
	auto stop = high_resolution_clock::now();
	auto seconds = (double)duration_cast<microseconds>(stop - start).count() / 1000000;
	cout << "Time: " << seconds << endl << endl;
	cout << "Nodes: " << nodes << endl << endl;
	cout << "Nodes/sec: " << nodes/seconds << endl << endl;*/

	tr.getBestMove(3,3);




	return true;
}

bool Test::test2()
{
	return false;
	//Color color = WHITE;
	//Board* board = MoveGeneration::boardFromFEN("r3k3/1p6/8/8/8/8/1P6/4K2R w KAhq - 0 1");
	//list<Move> history = list<Move>();
	//while (1)
	//{
	//	list<Move> moves = list<Move>();

	//	
	//	MoveGeneration::moveGeneration2(*board,color, moves);
	//	color = toggleColor(color);
	//	
	//	int i = 0;
	//	for (const auto& m : moves)
	//	{
	//		int one = bitScanForward2(m.move);
	//		int two = bitScanForward2(m.move ^ fields[one]);
	//		Bitboard aa = ((board->whoToMove == WHITE) ? board->occupancy[WHITE] : board->occupancy[BLACK]);

	//		if (fields[one] & aa)
	//			swap(two, one);
	//		//printBitboard(m.move);
	//		char a = (char)(two % 8 + 97);
	//		int b = (int)(two / 8) + 1;
	//		char c = (char)(one % 8 + 97);
	//		int d = (int)(one / 8) + 1;
	//		cout<<++i<<". " << a + to_string(b) + c + to_string(d)<<endl;
	//	}

	//	cout << "Ruchow lacznie: " << moves.size() << endl;
	//	cout << "Castle rights white: " << board->castleRights[0][0] << " " << board->castleRights[0][1] << endl;
	//	cout << "Castle rights black: " << board->castleRights[1][0] << " " << board->castleRights[1][1] << endl;
	//	printBoard(*board);

	//	int number;

	//	cin >> number;
	//	if (number != 0)
	//	{
	//		for (auto& m : moves)
	//		{
	//			--number;
	//			if (number == 0)
	//			{
	//				MoveGeneration::makeMove(*board, m);
	//				history.push_back(m);
	//			}

	//		}
	//	}
	//	else
	//	{
	//		if (history.empty()) continue;
	//		Move m = history.back();
	//		MoveGeneration::unmakeMove(*board, m);
	//		history.pop_back();
	//	}

	//}
}


string Test::compareMoveGeneration(string stockfish, string myEngine, bool showEverything)
{
	string stockfish_nodes = stockfish.substr(stockfish.find("Nodes searched: ") + 16, stockfish.length() - stockfish.find("Nodes searched: ") - 20);

	if (!showEverything) return stockfish_nodes;

	std::stringstream ss(stockfish);
	std::stringstream ss2(myEngine);
	std::string to;

	list<string> stockfish_moves;
	list<string> my_moves;

	int i = 0;
	while (getline(ss, to, '\n')) {
		i++;
		if (i == 1) continue;
		if (to.length() < 2) break;
		stockfish_moves.push_back(to);
	}

	while (getline(ss2, to, '\n'))
	{
		my_moves.push_back(to);
	}

	
	cout << "Stockfish: " << stockfish_moves.size() << endl;
	cout << "MyEngine:  " << my_moves.size() << endl << endl;

	while (!stockfish_moves.empty())
	{
		string first = stockfish_moves.front();
		stockfish_moves.pop_front();
		size_t pos = first.find(":");
		string key = first.substr(0, pos);


		string value = first.substr(pos+2, first.find("\r")-pos-2);
		string value2 = "";
		for (auto it = my_moves.begin(); it != my_moves.end();) {

			size_t pos = (*it).find(":");
			string key2 = (*it).substr(0, pos);
			if (key2 == key) {
				value2 = (*it).substr(pos + 2);
				it = my_moves.erase(it);
			}
			else {
				++it;
			}
		}
		cout << key << ": " << value <<" " << value2<<" "<<((value == value2) ? "+" : "!!!!!!!!!!!!!!!" )<< endl;
	}
	cout << endl << "additional moves: " << endl;
	while (!my_moves.empty())
	{
		string first = my_moves.front();
		my_moves.pop_front();
		cout << first << endl;
	}


	return stockfish_nodes;
}