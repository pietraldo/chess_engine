#pragma once

#include <iostream>
#include <string>
#include <chrono>


#include "variables.h"
#include "GamePrepare.h"
#include "MoveGeneration.h"
#include "Functions.h"
#include "Game.h"


class UciTranslator
{
	
public:
	UciTranslator() { GamePrepare(); };

	static string TranslateMove(Board&, Move&);
	int goPerft(int depth);
	void showBoard();
	void readBoard(Board&);
	void getBestMove(int depth, int maxTime=3600);

private:
	Board board;
};

