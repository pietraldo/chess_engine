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

#define N 8
#define M 64

using namespace std;

void movesExamples();
Board* boardExample();
Board* boardExample2();

int main()
{
	
	GamePrepare();
	
	movesExamples();
	Board* board=boardExample2();

	
	cout << Game::isLegalCastleBlackShort(*board);
}
Board* boardExample()
{
	Board* board= new Board();
	(*board).pawnW = A2 | B2 | C2 | E2 | F2 | G3 | H2;
	(*board).pawnB = A6 | B7 | D6 | F7 | G7 | H6;
	(*board).rookB = A8 | F8|H1;
	(*board).rookW = B1 | F1;
	(*board).queenB = F6;
	(*board).queenW = D1;
	(*board).kingB = G8;
	(*board).kingW = G1;
	(*board).bishopW = G2;
	(*board).bishopB = C8 | C5;
	(*board).knightB = C6;
	(*board).knightW = E4 | F3;
	board->occupancyB = board->knightB | board->pawnB | board->kingB | board->queenB | board->rookB | board->bishopB;
	board->occupancyW = board->knightW | board->pawnW | board->kingW | board->queenW | board->rookW | board->bishopW;
	printBoard((*board));
	return board;
 }

Board* boardExample2()
{
	Board* board= new Board();
	(*board).pawnW = A2 | B2 | C2 | E2 | F2 | G3 ;
	(*board).pawnB = A6 | B7  | F7 | G7 | H6|H2;
	(*board).rookB = A8 | H8;
	(*board).rookW = B1 | F1;
	(*board).queenB = F6;
	(*board).queenW = D3;
	(*board).kingB = E8;
	(*board).kingW = G1;
	(*board).bishopW = G2;
	(*board).bishopB = C8 | C5;
	(*board).knightB = C6;
	(*board).knightW = E4 | F3;
	board->occupancyB = board->knightB | board->pawnB | board->kingB | board->queenB | board->rookB | board->bishopB;
	board->occupancyW = board->knightW | board->pawnW | board->kingW | board->queenW | board->rookW | board->bishopW;
	printBoard((*board));
	return board;
 }

void movesExamples()
{
	Bitboard occupancy = A3 | D4 | G8 | F4 | D5 | E7 | C5 | F2 | H4 | D3 | B4; // zajete pola na planszy
	Bitboard bishop = G1; // pole na ktorej jest wieza
	Bitboard rook = C3;
	Bitboard knight = H8;
	Bitboard king = E3;
	Bitboard pawn = H7;
	Bitboard pawn2 = E7;


	printBitboard(occupancy); // wyswietlanie planszy

	// generowanie wszystkich ruchow
	int num2 = log2(bishop);
	Bitboard occupancy2 = occupancy & (bishopMask[num2]);
	Bitboard att2 = GamePrepare::attacBishop[num2][(occupancy2 * GamePrepare::magicBishop[num2]) >> (M - GamePrepare::bishopShifts[num2])];
	printBitboard(att2);// wyswietlanie wszystkich ruchow

	// rook moves
	int num3 = log2(rook);
	Bitboard occupancy3 = occupancy & (rookMask[num3]);
	Bitboard att3 = GamePrepare::attackRook[num3][(occupancy3 * GamePrepare::magicRook[num3]) >> (M - GamePrepare::rookShifts[num3])];
	printBitboard(att3);// wyswietlanie wszystkich ruchow

	// knight moves
	int num4 = log2(knight);
	Bitboard att4 = GamePrepare::attackKnight[num4];
	printBitboard(att4);

	// king moves
	int num5 = log2(king);
	Bitboard att5 = GamePrepare::attackKing[num5];
	printBitboard(att5);

	// pawn moves
	int num6 = log2(pawn);
	Bitboard att6 = GamePrepare::pawnMovesB[num6];
	printBitboard(att6);

	// pawn attacks
	int num7 = log2(pawn2);
	Bitboard att7 = GamePrepare::attackPawnW[num7];
	printBitboard(att7);
}





