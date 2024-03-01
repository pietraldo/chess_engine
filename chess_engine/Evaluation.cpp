#include "Evaluation.h"


float Evaluation::Evaluate2(Board& board)
{

	float eval = 0;
	int values[] = { 5,3,3,1,9,100 };

	int color = 1;
	Color c = WHITE;
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < P; i++)
		{
			Bitboard figures = board.figure[j][i];
			while (figures)
			{
				int index = bitScanForward(figures);
				figures &= figures - 1;
				eval += values[i] * color;
			}
		}
		color *= -1;
	}
	return eval;
}
float Evaluation::Evaluate(Board& board)
{
	float eval = 0;
	int values[] = { 5000,3000,3000,1000,9000,100000 };

	int color = 1;
	Color c = WHITE;
	for (int j = 0; j < 2; j++)
	{
		Bitboard activity = 0;
		for (int i = 0; i < P; i++)
		{
			Bitboard figures = board.figure[j][i];
			while (figures)
			{
				int index = bitScanForward(figures);
				eval += evalsBoards[i + j * 6][index] * color;
				activity |= MoveGeneration::attacks_funcitons[i](board, index, c);

				figures &= figures - 1;
				eval += values[i] * color;
			}
		}

		while (activity)
		{
			activity &= activity - 1;
			eval += color * 4;
		}

		color *= -1;
		c = BLACK;
	}
	return eval;
}