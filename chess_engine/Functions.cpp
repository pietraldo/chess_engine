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
		if (board.figure[blackBishop] & fields[i])
			figure[i] = 'b';
		if (board.figure[whiteBishop] & fields[i])
			figure[i] = 'B';
		if (board.figure[blackRook] & fields[i])
			figure[i] = 'r';
		if (board.figure[whiteRook] & fields[i])
			figure[i] = 'R';
		if (board.figure[blackQueen] & fields[i])
			figure[i] =  'q';
		if (board.figure[whiteQueen] & fields[i])
			figure[i] = 'Q';
		if (board.figure[blackKing] & fields[i])
			figure[i] = 'k';
		if (board.figure[whiteKing] & fields[i])
			figure[i] = 'K';
		if (board.figure[blackKnight] & fields[i])
			figure[i] = 'n';
		if (board.figure[whiteKnight] & fields[i])
			figure[i] = 'N';
		if (board.figure[blackPawn] & fields[i])
			figure[i] = 'p';
		if (board.figure[whitePawn] & fields[i])
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