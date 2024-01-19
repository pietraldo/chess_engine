#include "Test.h"
#include <sstream>
#include <queue>
#include <string>
#include <algorithm>
#include <chrono>


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
	GamePrepare();

	bool show=false;
	int depth = 2;
	string fen[] = { 
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" ,
		"rnbqkbnr/p3ppp1/1p5p/3p4/2BP4/5Q2/PPP2PPP/RNB1K1NR w KQkq - 0 6",
		"rnbqkbnr/pPpppp1p/8/8/8/8/P1pPP2P/RNBQKBNR w KQkq - 0 1",
		"rnbqkbnr/pppppppp/8/3P4/8/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1",
		"rnb1kbnr/pppppppp/4q3/3P4/7P/3QK3/PPP2PPP/RNB2BNR w kq - 0 1",
		"rn2kbnr/pppppppp/8/3P2q1/1b5P/3Q4/PPPK1PPP/RNB2BNR w HAkq - 0 1",
		"rn2kbnr/pppppppp/5b2/3P4/3q3P/1N1Q4/PPPK1PPP/R1B2BNR b kq - 0 1",
		"r3k2r/p1pppppp/2np1bn1/3P4/1Q1q3P/1NB2N2/PPP2BPP/R3K2R b KQkq - 0 1",
		"r3k2r/p1pppppp/2np1bn1/3P4/1Q1q3P/1NB2N2/PPP2BPP/R3K2R w KQkq - 0 1",
		"8/1P1k4/8/3K4/8/7p/8/8 w - - 0 1"
	};
	long sumN = 0;
	double sumT = 0;
	for (int i = 0; i < size(fen); i++)
	{
		cout << "Position: " << i + 1 << endl;

		// stockfish response
		string stockfish = ReadyChessEngine::GiveAnswer(("position fen " + fen[i] + "\ngo perft " + to_string(depth) + "\n\quit\n").c_str());

		Board* board = Game::boardFromFEN(fen[i]);
		Game::output = "";
		auto start = high_resolution_clock::now();

		// my response
		int nodes = Game::generation(board, board->whoToMove, depth);

		auto stop = high_resolution_clock::now();
		double seconds = (double)duration_cast<microseconds>(stop - start).count()/1000000;

		string myEngine = Game::output;
		string stockfish_nodes=compareMoveGeneration(stockfish, myEngine, false);

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
	test1();
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