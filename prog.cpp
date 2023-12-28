#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdint>
#include <random>
#include <cstdlib> 



#include "variables.h"
#include "GamePrepare.h"

#define N 8
#define M 64

using namespace std;



int bit(Bitboard number, Bitboard index);
void QueenSlidingMoveGenartionExample();
Bitboard reverseBits(Bitboard n);
int h1(Bitboard b);
int h2(Bitboard b, int index);

Bitboard* KnightAttack();

void init();


int main()
{
	init();
	
	GamePrepare prepare = GamePrepare();
	



	// game
	//Bitboard occupancy = A3 | D4 | G8 | F4 | D5 | E7 | C5 | F2 | H4 | D3 | B4; // zajete pola na planszy
	//Bitboard rook = G1; // pole na ktorej jest wieza
	//printBoard(occupancy); // wyswietlanie planszy

	//// generowanie wszystkich ruchow
	//int num = log2(rook);
	//occupancy &= (rookMask[num]);
	//Bitboard att = prepare.attackRook[num][occupancy * prepare.magicRook[num] >> (M - prepare.rookShifts[num])];
	//printBoard(att);// wyswietlanie wszystkich ruchow


	// game
	Bitboard occupancy2 = A3 | D4 | G8 | F4 | D5 | E7 | C5 | F2 | H4 | D3 | B4; // zajete pola na planszy
	Bitboard bishop = C4; // pole na ktorej jest wieza

	printBoard(occupancy2); // wyswietlanie planszy

	// generowanie wszystkich ruchow
	int num2 = log2(bishop);
	occupancy2 &= (bishopMask[num2]);
	Bitboard att2 = prepare.attacBishop[num2][(occupancy2 * prepare.magicBishop[num2]) >> (M - prepare.bishopShifts[num2])];
	printBoard(att2);// wyswietlanie wszystkich ruchow
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

void init()
{
	fields[0] = 1ULL;
	for (int i = 1; i < M; i++)
	{
		fields[i] = 1ULL << i;

	}

	vertical[0] = A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8;
	for (int i = 1; i < N; i++)
		vertical[i] = vertical[0] << i;

	horizontal[0] = A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1;
	for (int i = 1; i < N; i++)
		horizontal[i] = horizontal[0] << (i*8);



	Bitboard frame = horizontal[0] | horizontal[N - 1] | vertical[0] | vertical[N - 1];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			rookMask[i * N + j] = horizontal[i] | vertical[j];
			rookMask[i * N + j] ^= (rookMask[i * N + j] & frame);
			

			if (i == 0)
				rookMask[i * N + j] |= (horizontal[i] ^ (fields[0] | fields[N - 1]));
			else if(i==(N-1))
				rookMask[i * N + j] |= (horizontal[i] ^ (fields[(N-1)*N] | fields[N*N - 1]));
			
			if (j == 0)
				rookMask[i * N + j] |= (vertical[j] ^ (fields[0] | fields[(N - 1)*N]));
			else if (j == (N - 1))
				rookMask[i * N + j] |= (vertical[j] ^ (fields[N-1] | fields[N * N - 1]));

			if((fields[i*N+j]& rookMask[i*N+j])!=0)
				rookMask[i * N + j] ^= fields[i * N + j];
		}
	}
	//printBoard(rookMask[h1(C3)]);

	for (int i = 0; i < M; i++)
	{
		bishopMask[i] = 0;

		auto add = [&](int k, int j) {
			int y = i / 8;
			int x = i % 8;
			while (x + j < 7 && x + j >= 1 && y + k < 7 && y + k >= 1)
			{
				bishopMask[i] |= fields[(y + k) * 8 + (x + j)];
				x += j;
				y += k;
			}
		};

		add(-1, 1);
		add(1, 1);
		add(1, -1);
		add(-1, -1);
	}

	// testing 
   	/*cout<<endl<<endl << "E5" << endl;
	printBoard(bishopMask[h1(E5)]);
	cout<<endl<<endl << "H1" << endl;
	printBoard(bishopMask[h1(H1)]);
	cout<<endl<<endl << "A8" << endl;
	printBoard(bishopMask[h1(A8)]);
	cout<<endl<<endl << "F1" << endl;
	printBoard(bishopMask[h1(F1)]);
	cout<<endl<<endl << "H4" << endl;
	printBoard(bishopMask[h1(H4)]);*/
}



int h1(Bitboard b)
{
	return (int)log2(b);
}

int h2(Bitboard k, int index)
{
	return (k * 36029351611875328ULL) >> (64 - 12);
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

Bitboard reverseBits(Bitboard n) {
	Bitboard result = 0;
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