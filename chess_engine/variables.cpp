#pragma once
#include "variables.h"


#define M 64
#define N 8

using Bitboard = uint64_t;


Bitboard fields[M+1];
Bitboard vertical[N];
Bitboard horizontal[N];


Bitboard rookMask[M];
Bitboard bishopMask[M];

Color toggleColor(Color color) {
	return (color == WHITE) ? BLACK : WHITE;
}







