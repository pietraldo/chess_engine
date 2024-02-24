#include "Game.h"

void Game::startGame()
{
	GamePrepare();
	Board* board = MoveGeneration::boardFromFEN("7r/1p4pp/3kpn2/pR6/4p3/P7/1P4PP/2R3K1 w - - 0 26");
	printBoard(*board);

	/*list<Move> moveList = list<Move>();
	MoveGeneration::generateMovesNew(*board,BLACK, moveList);*/

	cout<<"Best move: "<<PickBestMove(*board, WHITE);
	cout << " " << num;
}
int Game::num = 0;
float Game::Evaluate(Board& board)
{
	num++;
	float eval = 0;
	int values[] = {5,3,3,1,9,10000};
	
	int color = 1;
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < P; i++)
		{
			Bitboard figures = board.figure[j][i];
			while (figures)
			{
				figures &= figures - 1;
				eval += values[i] *color;
			}
		}
		color *= -1;
	}
	return eval;
}
int maxDepth = 4;
Move Game::PickBestMove(Board& board, Color color)
{
	list<Move> moveList = list<Move>();
	MoveGeneration::generateMovesNew(board, color, moveList);
	
	if (moveList.empty())
		exit(-22);

	float alpha=-99999;
	float beta=99999;

	Move bestMove = moveList.front();
	MoveGeneration::makeMove(board,bestMove);
	float bestEval = AlphaBetaPrunning(board, toggleColor(color), alpha, beta, maxDepth);
	MoveGeneration::unmakeMove(board, bestMove);
	cout << bestMove << endl;
	if (color == WHITE) // maxinmazing
	{
		alpha = bestEval;
		for (auto it = next(moveList.begin()); it != moveList.end(); ++it)
		{
			MoveGeneration::makeMove(board, *it);
			float evalMove = AlphaBetaPrunning(board, toggleColor(color), bestEval, beta, maxDepth);
			MoveGeneration::unmakeMove(board, *it);
			cout<<(*it)<<": "<< evalMove<<endl;
			if (evalMove > bestEval)
			{
				bestEval = evalMove;
				bestMove = *it;
			}
		}
	}
	else // minimazing
	{
		beta = bestEval;
		for (auto it = next(moveList.begin()); it != moveList.end(); ++it)
		{
			MoveGeneration::makeMove(board, *it);
			float evalMove = AlphaBetaPrunning(board, toggleColor(color), alpha, bestEval, maxDepth);
			MoveGeneration::unmakeMove(board, *it);
			if (evalMove < bestEval)
			{
				bestEval = evalMove;
				bestMove = *it;
			}
		}
	}
	return bestMove;
}

float Game::AlphaBetaPrunning(Board& board, Color color, float alpha, float beta, int maxDepth, int depth)
{

	if (depth == maxDepth)
	{
		return Evaluate(board);
	}
	list<Move> moveList = list<Move>();
	MoveGeneration::generateMovesNew(board,color,moveList);
	
	float value;
	if (color == WHITE) // maximizer
	{
		value = -999999;

		for (Move m : moveList)
		{
			MoveGeneration::makeMove(board, m);
			float moveValue = AlphaBetaPrunning(board, toggleColor(color),value, beta, maxDepth, depth+1);
			if (moveValue > value)
			{
				value = moveValue;
			}
			MoveGeneration::unmakeMove(board, m);

			if (value > beta)
				return value;
		}
	}
	else // minimizer
	{
		value = 999999;

		for (Move m : moveList)
		{
			MoveGeneration::makeMove(board, m);
			float moveValue = AlphaBetaPrunning(board, toggleColor(color), alpha, value, maxDepth, depth + 1);
			if (moveValue < value)
			{
				value = moveValue;
			}
			MoveGeneration::unmakeMove(board, m);

			if (value < alpha)
				return value;
		}
		
	}
	return value;
}