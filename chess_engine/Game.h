#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <intrin.h>
#include <string>
#include <iomanip> 
#include <algorithm> // for sort
#include <utility> // for pair
#include <chrono>

#include "variables.h"
#include "GamePrepare.h"
#include "MoveGeneration.h"
#include "Functions.h"



class Game
{
public:
	
	static void startGame();
	static float Evaluate(Board& board);
	static float Evaluate2(Board& board);
	static Move PickBestMove(Board&, Color, int depth, int maxTime=3600);

	static float AlphaBetaPrunning(Board&, Color, float alpha, float beta, int maxDepth, list<Move>& pathHistory ,int depth=1);
	
	static float quisanceSearch(Board& board, Color color, float alpha, float beta, int max_depth, int depth, list<Move>& pathHistory);
	static bool positionStable(Board&) { return false; };
	
	static int num;

private:
	
	static void sortMovesMVLVA(vector<list<Move>::iterator>& moveList);
	
};















































































