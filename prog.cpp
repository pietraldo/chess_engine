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
	Bitboard king = E3;
	Bitboard pawn = H7;
	Bitboard pawn2 = E7;
	

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

	// pawn moves
	int num6 = log2(pawn);
	Bitboard att6 = prepare.pawnMovesB[num6];
	printBoard(att6);

	// pawn attacks
	int num7 = log2(pawn2);
	Bitboard att7 = prepare.attackPawnW[num7];
	printBoard(att7);
}






int h1(Bitboard b)
{
	return (int)log2(b);
}

int h2(Bitboard k, int index)
{
	return (k * 36029351611875328ULL) >> (64 - 12);
}



