#include "Game.h"

void Game::startGame()
{
	GamePrepare();
	Board* board = MoveGeneration::boardFromFEN("7r/1p4pp/3kpn2/pR6/4p3/P7/1P4PP/2R3K1 w - - 0 26");
	printBoard(*board);

	/*list<Move> moveList = list<Move>();
	MoveGeneration::generateMovesNew(*board,BLACK, moveList);*/

	cout<<"Best move: "<<PickBestMove(*board, WHITE);
	cout << " " << num<<endl;

	num = 0;
	cout << "Best move: " << PickBestMove2(*board, WHITE);
	cout << " " << num<<endl;
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


int maxDepth = 5;
Move Game::PickBestMove(Board& board, Color color)
{
	
	list<Move> moveList = list<Move>();
	MoveGeneration::generateMovesNew(board, color, moveList);
	
	if (moveList.empty())
		exit(-22);

	vector<list<Move>::iterator> moves(moveList.size());
	vector<float> evals(moves.size());
	
	vector<std::pair<float, list<Move>::iterator>> combined;
	for (size_t i = 0; i < evals.size(); ++i) {
		combined.push_back(std::make_pair(evals[i], moves[i]));
	}

	int k = 0;
	for (auto it = moveList.begin(); it != moveList.end(); ++it)
	{
		combined[k].second = it;
		MoveGeneration::makeMove(board, *it);
		combined[k].first=Evaluate(board);
		MoveGeneration::unmakeMove(board, *it);
		k++;
	}
	auto compareWhite = [](const pair<float, list<Move>::iterator>& p1, const pair<float, list<Move>::iterator>& p2) {
		return p1.first > p2.first; // Sort in descending order
	};
	auto compareBlack = [](const pair<float, list<Move>::iterator>& p1, const pair<float, list<Move>::iterator>& p2) {
		return p1.first < p2.first; // Sort in ascending order
	};

	Move bestMove;
	float bestEval;

	float alpha=-99999;
	float beta=99999;

	for (int i = 1; i <= maxDepth; i++)
	{
		if (color == WHITE) // maxinmazing
		{
			bestEval = -99999;
			sort(combined.begin(), combined.end(), compareWhite);
			alpha = bestEval;
			for (int j=0; j<combined.size(); j++)
			{
				MoveGeneration::makeMove(board, *combined[j].second);
				float evalMove = AlphaBetaPrunning(board, toggleColor(color), bestEval, beta, i);
				MoveGeneration::unmakeMove(board, *combined[j].second);
				
				if(i==maxDepth)
					cout << (*combined[j].second) << ": " << evalMove << endl;
				
				combined[j].first = evalMove;
				if (evalMove > bestEval)
				{
					bestEval = evalMove;
					bestMove = *combined[j].second;
				}
			}
		}
		else // minimazing
		{
			bestEval = 99999;
			sort(combined.begin(), combined.end(), compareBlack);
			beta = bestEval;
			for (int j = 0; j < combined.size(); j++)
			{
				MoveGeneration::makeMove(board, *combined[j].second);
				float evalMove = AlphaBetaPrunning(board, toggleColor(color), alpha, bestEval, i);
				MoveGeneration::unmakeMove(board, *combined[j].second);
				
				if (i == maxDepth)
					cout << (*combined[j].second) << ": " << evalMove << endl;
				
				combined[j].first = evalMove;
				if (evalMove < bestEval)
				{
					bestEval = evalMove;
					bestMove = *combined[j].second;
				}
			}
		}
		cout << "DEPTH: " << i << " best move: " << bestMove << endl;
	}
	
	return bestMove;
}

Move Game::PickBestMove2(Board& board, Color color)
{
	list<Move> moveList = list<Move>();
	MoveGeneration::generateMovesNew(board, color, moveList);

	if (moveList.empty())
		exit(-22);

	float alpha = -999999;
	float beta = 999999;

	Move bestMove = moveList.front();
	MoveGeneration::makeMove(board, bestMove);
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
			cout << (*it) << ": " << evalMove << endl;
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
			cout << (*it) << ": " << evalMove << endl;
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
	// TODO: don't do recursive just to evaluate
	if (depth == maxDepth)
	{
		return Evaluate(board);
	}
	list<Move> moveList = list<Move>();
	MoveGeneration::generateMovesNew(board,color,moveList);

	if (board.figure[color][KING] == 0)
	{
		return (color == WHITE) ? -999999 : 999999;
	}
	if (moveList.empty())
	{
		if (MoveGeneration::isCheck(board, bitScanForward(board.figure[color][KING]), color))
		{
			return (color == WHITE) ? -999999 : 999999;
		}
		else
		{
			return 0;
		}
	}
	
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