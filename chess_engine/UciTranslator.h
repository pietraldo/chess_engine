#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>


#include "variables.h"
#include "GamePrepare.h"
#include "MoveGeneration.h"
#include "Functions.h"
#include "Game.h"

const std::string file_name_log = "console_log.txt";
class UciTranslator
{
	
public:
	UciTranslator() { GamePrepare(); board = *boardFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); };

	static Board* boardFromFEN(string fen);
	static string TranslateMove(Board&, Move&);

	int goPerft(int depth);
	void showBoard();
	void readBoard(Board&);
	void getBestMove(int depth, int maxTime=3600);
	void quit();

	static void startComunication();

	void test();

	static void write_to_file(const std::string& file_name, const std::string& content);

	static void giveResponse(const std::string& content);
private:
	Board board;

	Move convertAlgebraicToMove(string algebraic);
	void loadAlgebraicMoves(vector<string> algebraicMoves);
	static vector<string> devideStringMoves(string);

	
};

