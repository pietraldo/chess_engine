#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdint>
#include <random>
#include <cstdlib> 



#include "variables.h"
#include "GamePrepare.h"
#include "Functions.h"

#define N 8
#define M 64

using namespace std;



int h1(Bitboard b);
int h2(Bitboard b, int index);

Bitboard* KnightAttack();

int main()
{
	
	GamePrepare prepare = GamePrepare();
	

	// game example
	Bitboard occupancy = A3 | D4 | G8 | F4 | D5 | E7 | C5 | F2 | H4 | D3 | B4; // zajete pola na planszy
	Bitboard bishop = A7; // pole na ktorej jest wieza
	Bitboard rook=C3;
	Bitboard knight = H8;
	Bitboard king = H8;
	

	printBoard(occupancy); // wyswietlanie planszy

	// generowanie wszystkich ruchow
	int num2 = log2(bishop);
	Bitboard occupancy2 =occupancy & (bishopMask[num2]);
	Bitboard att2 = prepare.attacBishop[num2][(occupancy2 * prepare.magicBishop[num2]) >> (M - prepare.bishopShifts[num2])];
	printBoard(att2);// wyswietlanie wszystkich ruchow

	// rook moves
	int num3 = log2(rook);
	Bitboard occupancy3 = occupancy & (rookMask[num3]);
	Bitboard att3 = prepare.attackRook[num3][(occupancy3 * prepare.magicRook[num3]) >> (M - prepare.rookShifts[num3])];
	printBoard(att3);// wyswietlanie wszystkich ruchow

	// knight moves
	int num4 = log2(knight);
	Bitboard att4 = prepare.attackKnight[num4];
	printBoard(att4);

	// king moves
	int num5 = log2(king);
	Bitboard att5 = prepare.attackKing[num5];
	printBoard(att5);
}




Bitboard* KnightAttack()
{
	Bitboard* attacks =  new Bitboard[M];
	
	
	for (int i = 0; i < M; i++)
	{
		attacks[i] = 0;
		if (i/8<=6 && i % 8 >= 2)
			attacks[i] |= fields[i + 6];
		if (i / 8 <= 5 && i%8>=1)
			attacks[i] |= fields[i + 15];
		if (i/8<=5 && i % 8 <= 6)
			attacks[i] |= fields[i + 17];
		if (i/8<=6 && i % 8 <=5)
			attacks[i] |= fields[i + 10];
		if (i/8 >=1 && i % 8 >= 2)
			attacks[i] |= fields[i - 10];
		if (i/8>=2 && i % 8 >= 1)
			attacks[i] |= fields[i - 17];
		if (i/8 >=2 && i % 8 <= 6)
			attacks[i] |= fields[i -15];
		if (i/8 >=1 && i % 8 <=5)
			attacks[i] |= fields[i -6];

	}
	
	return attacks;
}




int h1(Bitboard b)
{
	return (int)log2(b);
}

int h2(Bitboard k, int index)
{
	return (k * 36029351611875328ULL) >> (64 - 12);
}



