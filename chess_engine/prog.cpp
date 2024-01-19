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
#include "Test.h"

#define N 8
#define M 64

using namespace std;

void movesExamples();

Board* startBoard();
Board* boardExample2();
Board* boardExample3();
Board* boardExample4();

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
}
Board* startBoard()
{
	Board* board = new Board();
	(*board).figure[whitePawn] = A2| B2 | C2 | D2 | E2 | F2|G2|H2;
	(*board).figure[blackPawn] = A7 | B7 | C7 |D7 |E7 |F7 | G7 | H7;
	(*board).figure[blackRook] = A8 | H8;
	(*board).figure[whiteRook] = A1 | H1;
	(*board).figure[blackQueen] = D8;
	(*board).figure[whiteQueen] = D1;
	(*board).figure[blackKing] = E8;
	(*board).figure[whiteKing] = E1;
	(*board).figure[whiteBishop] = C1|F1;
	(*board).figure[blackBishop] = C8 | F8;
	(*board).figure[blackKnight] = B8|G8;
	(*board).figure[whiteKnight] = B1 | G1;

	board->figure[whiteOccupancy] = 0;
	board->figure[blackOccupancy] = 0;
	for (int i = 0; i < 6; i++)
	{
		board->figure[whiteOccupancy] |= board->figure[i];
		board->figure[blackOccupancy] |= board->figure[6 + i];
	}
	board->BLC = true;
	board->BSC = true;
	board->WLC = true;
	board->WSC = true;
	//board->onPassantField = log2(G6);

	return board;
}
Board* boardExample3()
{
	Board* board = new Board();
	(*board).figure[whitePawn] = A3 ;
	(*board).figure[blackPawn] = A4 ;
	(*board).figure[blackRook] = 0;
	(*board).figure[whiteRook] = 0;
	(*board).figure[blackQueen] = 0;
	(*board).figure[whiteQueen] = 0;
	(*board).figure[blackKing] = 0;
	(*board).figure[whiteKing] = 0;
	(*board).figure[whiteBishop] = 0;
	(*board).figure[blackBishop] = 0;
	(*board).figure[blackKnight] = 0;
	(*board).figure[whiteKnight] = 0;

	board->figure[whiteOccupancy] = 0;
	board->figure[blackOccupancy] = 0;
	for (int i = 0; i < 6; i++)
	{
		board->figure[whiteOccupancy] |= board->figure[i];
		board->figure[blackOccupancy] |= board->figure[6 + i];
	}
	board->BLC = true;
	board->BSC = true;
	board->WLC = true;
	board->WSC = true;

	return board;
}

//r4rk1/1bpq1ppp/pp2p3/3nP3/PbBP4/2N3B1/1P2QPPP/R2R2K1 w - - 1 18
Board* boardExample4()
{
	Board* board = new Board();
	(*board).figure[whitePawn] = A4|B2|D4|E5|F2|G2|H2;
	(*board).figure[blackPawn] = A6|B6|C7|E6|F7|G7|H7;
	(*board).figure[blackRook] = A8|F8;
	(*board).figure[whiteRook] = A1|D1;
	(*board).figure[blackQueen] = D7;
	(*board).figure[whiteQueen] = E2;
	(*board).figure[blackKing] = G8;
	(*board).figure[whiteKing] = G1;
	(*board).figure[whiteBishop] = G3|C4;
	(*board).figure[blackBishop] = B4|B7;
	(*board).figure[blackKnight] = D5;
	(*board).figure[whiteKnight] = C3;

	board->figure[whiteOccupancy] = 0;
	board->figure[blackOccupancy] = 0;
	for (int i = 0; i < 6; i++)
	{
		board->figure[whiteOccupancy] |= board->figure[i];
		board->figure[blackOccupancy] |= board->figure[6 + i];
	}
	board->BLC = false;
	board->BSC = false;
	board->WLC = false;
	board->WSC = false;
	board->onPassantField = 64;
	return board;
}

Board* boardExample2()
{
	Board* board= new Board();
	(*board).figure[whitePawn] = A2 | B2 | C2 | E2 | F2 | G3;
	(*board).figure[blackPawn] = A6 | B7 | F7 | G7 | H6 ;
	(*board).figure[blackRook] = A8 | H8;
	(*board).figure[whiteRook] = B1 | D6;
	(*board).figure[blackQueen] = F6;
	(*board).figure[whiteQueen] = D3;
	(*board).figure[blackKing] = E8;
	(*board).figure[whiteKing] = G1;
	(*board).figure[whiteBishop] = D4;
	(*board).figure[blackBishop] = C8 | C5;
	(*board).figure[blackKnight] = C6;
	(*board).figure[whiteKnight] = E4 | F3;
	board->figure[whiteOccupancy] = 0;
	board->figure[blackOccupancy] = 0;
	for (int i = 0; i < 6; i++)
	{
		board->figure[whiteOccupancy] |= board->figure[i];
		board->figure[blackOccupancy] |= board->figure[6+i];
	}
	
	
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





