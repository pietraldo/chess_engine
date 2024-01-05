#pragma once

#include <cstdint>

#define M 64
#define N 8

using Bitboard = uint64_t;

typedef struct Board
{
	Bitboard occupancyW;
	Bitboard occupancyB;
	Bitboard pawnW;
	Bitboard pawnB;
	Bitboard kingW;
	Bitboard kingB;
	Bitboard queenW;
	Bitboard queenB;
	Bitboard bishopW;
	Bitboard bishopB;
	Bitboard rookW;
	Bitboard rookB;
	Bitboard knightB;
	Bitboard knightW;
	bool BLC; //black right to long castle
	bool BSC; //black right to short castle
	bool WLC; //white right to long castle
	bool WSC; //white right to short castle
	Bitboard onPassantField;
};



enum Color {WHITE, BLACK};

extern Bitboard empty;

extern Bitboard A1;
extern Bitboard B1;
extern Bitboard C1;
extern Bitboard D1;
extern Bitboard E1;
extern Bitboard F1;
extern Bitboard G1;
extern Bitboard H1;

extern Bitboard A2;
extern Bitboard B2;
extern Bitboard C2;
extern Bitboard D2;
extern Bitboard E2;
extern Bitboard F2;
extern Bitboard G2;
extern Bitboard H2;

extern Bitboard A3;
extern Bitboard B3;
extern Bitboard C3;
extern Bitboard D3;
extern Bitboard E3;
extern Bitboard F3;
extern Bitboard G3;
extern Bitboard H3;

extern Bitboard A4;
extern Bitboard B4;
extern Bitboard C4;
extern Bitboard D4;
extern Bitboard E4;
extern Bitboard F4;
extern Bitboard G4;
extern Bitboard H4;

extern Bitboard A5;
extern Bitboard B5;
extern Bitboard C5;
extern Bitboard D5;
extern Bitboard E5;
extern Bitboard F5;
extern Bitboard G5;
extern Bitboard H5;

extern Bitboard A6;
extern Bitboard B6;
extern Bitboard C6;
extern Bitboard D6;
extern Bitboard E6;
extern Bitboard F6;
extern Bitboard G6;
extern Bitboard H6;

extern Bitboard A7;
extern Bitboard B7;
extern Bitboard C7;
extern Bitboard D7;
extern Bitboard E7;
extern Bitboard F7;
extern Bitboard G7;
extern Bitboard H7;

extern Bitboard A8;
extern Bitboard B8;
extern Bitboard C8;
extern Bitboard D8;
extern Bitboard E8;
extern Bitboard F8;
extern Bitboard G8;
extern Bitboard H8;



extern Bitboard FileABB;
extern Bitboard FileBBB;
extern Bitboard FileCBB;
extern Bitboard FileDBB;
extern Bitboard FileEBB;
extern Bitboard FileFBB;
extern Bitboard FileGBB;
extern Bitboard FileHBB;

extern Bitboard Rank1BB;
extern Bitboard Rank2BB;
extern Bitboard Rank3BB;
extern Bitboard Rank4BB;
extern Bitboard Rank5BB;
extern Bitboard Rank6BB;
extern Bitboard Rank7BB;
extern Bitboard Rank8BB;

extern Bitboard fields[M];
extern Bitboard vertical[N];
extern Bitboard horizontal[N];


extern Bitboard rookMask[M];
extern Bitboard bishopMask[M];


extern void printBoard(Board);
extern void printBitboard(Bitboard board);






