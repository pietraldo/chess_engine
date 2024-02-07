#pragma once

#include <cstdint>

#define M 64
#define N 8
#define P 6

using Bitboard = uint64_t;


enum Color { WHITE, BLACK };

extern Color toggleColor(Color color);


enum Figure {ROOK, BISHOP, KNIGHT, PAWN, QUEEN, KING};
enum Castle {LONGCASTLE, SHORTCASTLE};

typedef struct Board
{
	Bitboard figure[2][6]; 
	Bitboard occupancy[2];
	bool BLC; //black right to long castle
	bool BSC; //black right to short castle
	bool WLC; //white right to long castle
	bool WSC; //white right to short castle
	int onPassantField; // index o field 0-63
	Color whoToMove; // who's turn is to move
};


typedef struct Move
{
	Bitboard move=0; // bitboard two do xor operation on figrue[type_piece]
	int type_piece=0; // index of Bitboard - what type is this figure  (color * P + typePiece)
	Bitboard move2=0; 
	int type_piece2=0; 
	Bitboard move3=0; 
	int type_piece3=0; 

	bool shortCastle; // for possibility to undo move
	bool longCastle; // also
	int enPassant;  // and this also
};

constexpr Bitboard empty = 0ULL;

constexpr Bitboard A1 = 1ULL << 0;
constexpr Bitboard B1 = 1ULL << 1;
constexpr Bitboard C1 = 1ULL << 2;
constexpr Bitboard D1 = 1ULL << 3;
constexpr Bitboard E1 = 1ULL << 4;
constexpr Bitboard F1 = 1ULL << 5;
constexpr Bitboard G1 = 1ULL << 6;
constexpr Bitboard H1 = 1ULL << 7;

constexpr Bitboard A2 = 1ULL << 8;
constexpr Bitboard B2 = 1ULL << 9;
constexpr Bitboard C2 = 1ULL << 10;
constexpr Bitboard D2 = 1ULL << 11;
constexpr Bitboard E2 = 1ULL << 12;
constexpr Bitboard F2 = 1ULL << 13;
constexpr Bitboard G2 = 1ULL << 14;
constexpr Bitboard H2 = 1ULL << 15;

constexpr Bitboard A3 = 1ULL << 16;
constexpr Bitboard B3 = 1ULL << 17;
constexpr Bitboard C3 = 1ULL << 18;
constexpr Bitboard D3 = 1ULL << 19;
constexpr Bitboard E3 = 1ULL << 20;
constexpr Bitboard F3 = 1ULL << 21;
constexpr Bitboard G3 = 1ULL << 22;
constexpr Bitboard H3 = 1ULL << 23;

constexpr Bitboard A4 = 1ULL << 24;
constexpr Bitboard B4 = 1ULL << 25;
constexpr Bitboard C4 = 1ULL << 26;
constexpr Bitboard D4 = 1ULL << 27;
constexpr Bitboard E4 = 1ULL << 28;
constexpr Bitboard F4 = 1ULL << 29;
constexpr Bitboard G4 = 1ULL << 30;
constexpr Bitboard H4 = 1ULL << 31;

constexpr Bitboard A5 = 1ULL << 32;
constexpr Bitboard B5 = 1ULL << 33;
constexpr Bitboard C5 = 1ULL << 34;
constexpr Bitboard D5 = 1ULL << 35;
constexpr Bitboard E5 = 1ULL << 36;
constexpr Bitboard F5 = 1ULL << 37;
constexpr Bitboard G5 = 1ULL << 38;
constexpr Bitboard H5 = 1ULL << 39;

constexpr Bitboard A6 = 1ULL << 40;
constexpr Bitboard B6 = 1ULL << 41;
constexpr Bitboard C6 = 1ULL << 42;
constexpr Bitboard D6 = 1ULL << 43;
constexpr Bitboard E6 = 1ULL << 44;
constexpr Bitboard F6 = 1ULL << 45;
constexpr Bitboard G6 = 1ULL << 46;
constexpr Bitboard H6 = 1ULL << 47;

constexpr Bitboard A7 = 1ULL << 48;
constexpr Bitboard B7 = 1ULL << 49;
constexpr Bitboard C7 = 1ULL << 50;
constexpr Bitboard D7 = 1ULL << 51;
constexpr Bitboard E7 = 1ULL << 52;
constexpr Bitboard F7 = 1ULL << 53;
constexpr Bitboard G7 = 1ULL << 54;
constexpr Bitboard H7 = 1ULL << 55;

constexpr Bitboard A8 = 1ULL << 56;
constexpr Bitboard B8 = 1ULL << 57;
constexpr Bitboard C8 = 1ULL << 58;
constexpr Bitboard D8 = 1ULL << 59;
constexpr Bitboard E8 = 1ULL << 60;
constexpr Bitboard F8 = 1ULL << 61;
constexpr Bitboard G8 = 1ULL << 62;
constexpr Bitboard H8 = 1ULL << 63;


constexpr Bitboard FileABB = 0x0101010101010101ULL;
constexpr Bitboard FileBBB = FileABB << 1;
constexpr Bitboard FileCBB = FileABB << 2;
constexpr Bitboard FileDBB = FileABB << 3;
constexpr Bitboard FileEBB = FileABB << 4;
constexpr Bitboard FileFBB = FileABB << 5;
constexpr Bitboard FileGBB = FileABB << 6;
constexpr Bitboard FileHBB = FileABB << 7;

constexpr Bitboard Rank1BB = 0xFF;
constexpr Bitboard Rank2BB = Rank1BB << (8 * 1);
constexpr Bitboard Rank3BB = Rank1BB << (8 * 2);
constexpr Bitboard Rank4BB = Rank1BB << (8 * 3);
constexpr Bitboard Rank5BB = Rank1BB << (8 * 4);
constexpr Bitboard Rank6BB = Rank1BB << (8 * 5);
constexpr Bitboard Rank7BB = Rank1BB << (8 * 6);
constexpr Bitboard Rank8BB = Rank1BB << (8 * 7);


extern Bitboard fields[M+1]; // last one is empty
extern Bitboard vertical[N];
extern Bitboard horizontal[N];


extern Bitboard rookMask[M];
extern Bitboard bishopMask[M];


extern void printBoard(Board);
extern void printBitboard(Bitboard board);


// variables needed for checking castle
constexpr Bitboard shortWhiteCastle = F1 | G1;
constexpr Bitboard longWhiteCastle = B1 | C1 | D1;
constexpr Bitboard shortBlackCastle = F8 | G8;
constexpr Bitboard longBlackCastle = B8 | C8 | D8;




