#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <string>
#include <iomanip> 
#include <algorithm> // for sort
#include <utility> // for pair
#include <chrono>

#include "variables.h"
#include "GamePrepare.h"
#include "MoveGeneration.h"
#include "Functions.h"
#include "Evaluation.h"



class Game
{
public:
	
	static void startGame();

	static Move PickBestMove(Board&, Color, int depth, int maxTime=3600);

	static float AlphaBetaPrunning(Board&, Color, float alpha, float beta, int maxDepth, list<Move>& pathHistory ,int depth=1);
	
	static float quisanceSearch(Board& board, Color color, float alpha, float beta, int max_depth, int depth, list<Move>& pathHistory);
	static bool positionStable(Board&) { return false; };
	
private:
	
	static void sortMovesMVLVA(vector<list<Move>::iterator>& moveList);
	
};















































































