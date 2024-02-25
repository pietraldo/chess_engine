#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdint>
#include <random>
#include <cstdlib> 

#include "variables.h"
#include "GamePrepare.h"
#include "Functions.h"
#include "Game.h"
#include "MoveGeneration.h"
#include "Test.h"

#define N 8
#define M 64

using namespace std;



int main()
{
	
	//GamePrepare();
	//
	////movesExamples();
	//Board* board= Game::boardFromFEN("rnbqk2r/2p2ppp/p3pn2/1B6/3PP3/b1N5/1P3PPP/R1BQ1KNR b kq - 0 9");
	//board= Game::boardFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	////Game::test(*board);


	//printBoard(*board);
	 //cout<<Game::generation(board,board->whoToMove,5)<<endl;
	//printBoard(*board);
	Test::Test();

	//Game::startGame();
}



