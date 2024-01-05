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
	

	printBitboard(occupancy); // wyswietlanie planszy

	// generowanie wszystkich ruchow
	int num2 = log2(bishop);
	Bitboard occupancy2 =occupancy & (bishopMask[num2]);
	Bitboard att2 = prepare.attacBishop[num2][(occupancy2 * prepare.magicBishop[num2]) >> (M - prepare.bishopShifts[num2])];
	printBitboard(att2);// wyswietlanie wszystkich ruchow

	// rook moves
	int num3 = log2(rook);
	Bitboard occupancy3 = occupancy & (rookMask[num3]);
	Bitboard att3 = prepare.attackRook[num3][(occupancy3 * prepare.magicRook[num3]) >> (M - prepare.rookShifts[num3])];
	printBitboard(att3);// wyswietlanie wszystkich ruchow

	// knight moves
	int num4 = log2(knight);
	Bitboard att4 = prepare.attackKnight[num4];
	printBitboard(att4);

	// king moves
	int num5 = log2(king);
	Bitboard att5 = prepare.attackKing[num5];
	printBitboard(att5);

	// pawn moves
	int num6 = log2(pawn);
	Bitboard att6 = prepare.pawnMovesB[num6];
	printBitboard(att6);

	// pawn attacks
	int num7 = log2(pawn2);
	Bitboard att7 = prepare.attackPawnW[num7];
	printBitboard(att7);

	// Board
	Board board;
	board.pawnW = A2 | B2 | C2 | E2 | F2 | G3 | H2;
	board.pawnB = A6 | B7 | D6 | F7 | G7 | H6;
	board.rookB = A8 | F8;
	board.rookW = B1 | F1;
	board.queenB = F6;
	board.queenW = D1;
	board.kingB = G8;
	board.kingW = G1;
	board.bishopW = G2;
	board.bishopB = C8|C5;
	board.knightB = C6;
	board.knightW = E4 | F3;
	printBoard(board);
}







