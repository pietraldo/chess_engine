#include "Functions.h"


void printBitboard(Bitboard board)
{
	cout << endl;
	for (int i = N - 1; i >= 0; i--)
	{
		for (int j = 0; j < N; j++)
		{
			if (bit(board, i * N + j)) {
				// Print '1' in green
				std::cout << "\033[32m1\033[0m ";
			}
			else {
				// Print '0' in white
				std::cout << "\033[37m0\033[0m ";
			}
		}
		cout << endl;
	}
}

int bit(Bitboard number, Bitboard index)
{
	return (number >> index) & 1;
}

void printBoard(Board board)
{
	char figure[M];
	for (int i = 0; i < M; i++)
	{
		figure[i] = ' ';
		if (board.bishopB & fields[i])
			figure[i] = 'b';
		if (board.bishopW & fields[i])
			figure[i] = 'B';
		if (board.rookB & fields[i])
			figure[i] = 'r';
		if (board.rookW & fields[i])
			figure[i] = 'R';
		if (board.queenB & fields[i])
			figure[i] =  'q';
		if (board.queenW & fields[i])
			figure[i] = 'Q';
		if (board.kingB & fields[i])
			figure[i] = 'k';
		if (board.kingW & fields[i])
			figure[i] = 'K';
		if (board.knightB & fields[i])
			figure[i] = 'n';
		if (board.knightW & fields[i])
			figure[i] = 'N';
		if (board.pawnB & fields[i])
			figure[i] = 'p';
		if (board.pawnW & fields[i])
			figure[i] = 'P';
	}
	cout << endl;
	for (int i = N-1; i >=0; i--)
	{
		cout << "+---+---+---+---+---+---+---+---+" << endl;
		for (int j = 0; j < N; j++)
		{
			cout <<"| " << figure[i * N + j] << " ";
		}
		cout <<"| "<<i+1 << endl;
	}
	cout << "+---+---+---+---+---+---+---+---+" << endl;
	cout << "  A   B   C   D   E   F   G   H  " << endl;
}