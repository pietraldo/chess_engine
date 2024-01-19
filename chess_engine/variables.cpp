#pragma once
#include "variables.h"




#define M 64
#define N 8

using Bitboard = uint64_t;

  Bitboard empty = 0ULL;

 Bitboard A1 = 1ULL << 0;
 Bitboard B1 = 1ULL << 1;
 Bitboard C1 = 1ULL << 2;
 Bitboard D1 = 1ULL << 3;
 Bitboard E1 = 1ULL << 4;
  Bitboard F1 = 1ULL << 5;
  Bitboard G1 = 1ULL << 6;
  Bitboard H1 = 1ULL << 7;

  Bitboard A2 = 1ULL << 8;
Bitboard B2 = 1ULL << 9;
Bitboard C2 = 1ULL << 10;
Bitboard D2 = 1ULL << 11;
Bitboard E2 = 1ULL << 12;
Bitboard F2 = 1ULL << 13;
Bitboard G2 = 1ULL << 14;
Bitboard H2 = 1ULL << 15;

  Bitboard A3 = 1ULL << 16;
Bitboard B3 = 1ULL << 17;
Bitboard C3 = 1ULL << 18;
Bitboard D3 = 1ULL << 19;
Bitboard E3 = 1ULL << 20;
Bitboard F3 = 1ULL << 21;
Bitboard G3 = 1ULL << 22;
Bitboard H3 = 1ULL << 23;

  Bitboard A4 = 1ULL << 24;
Bitboard B4 = 1ULL << 25;
Bitboard C4 = 1ULL << 26;
Bitboard D4 = 1ULL << 27;
Bitboard E4 = 1ULL << 28;
Bitboard F4 = 1ULL << 29;
Bitboard G4 = 1ULL << 30;
Bitboard H4 = 1ULL << 31;

  Bitboard A5 = 1ULL << 32;
Bitboard B5 = 1ULL << 33;
Bitboard C5 = 1ULL << 34;
Bitboard D5 = 1ULL << 35;
Bitboard E5 = 1ULL << 36;
Bitboard F5 = 1ULL << 37;
Bitboard G5 = 1ULL << 38;
Bitboard H5 = 1ULL << 39;

  Bitboard A6 = 1ULL << 40;
Bitboard B6 = 1ULL << 41;
Bitboard C6 = 1ULL << 42;
Bitboard D6 = 1ULL << 43;
Bitboard E6 = 1ULL << 44;
Bitboard F6 = 1ULL << 45;
Bitboard G6 = 1ULL << 46;
Bitboard H6 = 1ULL << 47;

  Bitboard A7 = 1ULL << 48;
Bitboard B7 = 1ULL << 49;
Bitboard C7 = 1ULL << 50;
Bitboard D7 = 1ULL << 51;
Bitboard E7 = 1ULL << 52;
Bitboard F7 = 1ULL << 53;
Bitboard G7 = 1ULL << 54;
Bitboard H7 = 1ULL << 55;

  Bitboard A8 = 1ULL << 56;
Bitboard B8 = 1ULL << 57;
Bitboard C8 = 1ULL << 58;
Bitboard D8 = 1ULL << 59;
Bitboard E8 = 1ULL << 60;
Bitboard F8 = 1ULL << 61;
Bitboard G8 = 1ULL << 62;
Bitboard H8 = 1ULL << 63;

 Bitboard FileABB = 0x0101010101010101ULL;
 Bitboard FileBBB = FileABB << 1;
 Bitboard FileCBB = FileABB << 2;
 Bitboard FileDBB = FileABB << 3;
 Bitboard FileEBB = FileABB << 4;
 Bitboard FileFBB = FileABB << 5;
 Bitboard FileGBB = FileABB << 6;
 Bitboard FileHBB = FileABB << 7;

 Bitboard Rank1BB = 0xFF;
 Bitboard Rank2BB = Rank1BB << (8 * 1);
 Bitboard Rank3BB = Rank1BB << (8 * 2);
 Bitboard Rank4BB = Rank1BB << (8 * 3);
 Bitboard Rank5BB = Rank1BB << (8 * 4);
 Bitboard Rank6BB = Rank1BB << (8 * 5);
 Bitboard Rank7BB = Rank1BB << (8 * 6);
 Bitboard Rank8BB = Rank1BB << (8 * 7);

Bitboard fields[M+1];
Bitboard vertical[N];
Bitboard horizontal[N];


Bitboard rookMask[M];
Bitboard bishopMask[M];









