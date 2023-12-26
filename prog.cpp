#include <iostream>
#include <cstdint>
#include "variables.h"


#define N 8


using namespace std;




void printBoard(Bitboard board);
int bit(Bitboard number, Bitboard index);
void QueenSlidingMoveGenartionExample();
uint64_t reverseBits(uint64_t n);


int main()
{
	QueenSlidingMoveGenartionExample();
}

void QueenSlidingMoveGenartionExample()
{
	Bitboard occupancy = C2 | G2 | E7 | A7 | D6 | D8 | G4 | E5 | D2;
	Bitboard queen = D4;

	Bitboard przekatna1 = C3 | B2 | A1 | E5 | F6 | G7 | H8 | D4;
	Bitboard przekatna2 = E3 | F2 | G1 | C5 | B6 | A7 | D4;
	Bitboard rzad = Rank4BB;
	Bitboard kolumna = FileDBB;

	cout << "Queen reach D4:";
	printBoard(przekatna1 | przekatna2 | rzad | kolumna);

	cout << endl << "Occupancy:";
	printBoard(occupancy);

	// generate positive attacks
	Bitboard EastAttack = (occupancy ^ ((occupancy & rzad) - 2 * queen)) & rzad;
	Bitboard NorthAttack = (occupancy ^ ((occupancy & kolumna) - 2 * queen)) & kolumna;
	Bitboard NorthEastAttack = (occupancy ^ ((occupancy & przekatna1) - 2 * queen)) & przekatna1;
	Bitboard NorthWestAttack = (occupancy ^ ((occupancy & przekatna2) - 2 * queen)) & przekatna2;

	Bitboard positiveAttack = EastAttack | NorthAttack | NorthWestAttack | NorthEastAttack;
	cout << endl << "Positive Attacks: ";
	printBoard(positiveAttack);

	// generate negative attacks
	Bitboard o = occupancy;
	Bitboard s = queen;
	Bitboard sReverse = reverseBits(s);

	Bitboard SouthAttack = reverseBits(reverseBits(o & kolumna) ^ (reverseBits(o & kolumna) - (2 * sReverse))) & kolumna;
	Bitboard WestAttack = reverseBits(reverseBits(o & rzad) ^ (reverseBits(o & rzad) - (2 * sReverse))) & rzad;
	Bitboard SouthWestAttack = reverseBits(reverseBits(o & przekatna2) ^ (reverseBits(o & przekatna2) - (2 * sReverse))) & przekatna2;
	Bitboard SouthEastAttack = reverseBits(reverseBits(o & przekatna1) ^ (reverseBits(o & przekatna1) - (2 * sReverse))) & przekatna1;

	Bitboard NegativeAttack = SouthAttack | WestAttack | SouthEastAttack | SouthWestAttack;
	cout << endl << "Negative Attacks: ";
	printBoard(NegativeAttack);

}

uint64_t reverseBits(uint64_t n) {
	uint64_t result = 0;
	for (int i = 0; i < 64; ++i) {
		result = (result << 1) | (n & 1);
		n >>= 1;
	}
	return result;
}

void printBoard(Bitboard board)
{
	cout << endl;
	for (int i = N-1; i>=0; i--)
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