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

#ifdef _WIN32
int bitScanForward(Bitboard bb)
{
	unsigned long index;
	_BitScanForward64(&index, bb);
	return index;
}
#elif __linux__
int bitScanForward(Bitboard bb)
{
	return __builtin_ctzll(bb);
}
#endif


Color toggleColor(Color color) {
	return (color == WHITE) ? BLACK : WHITE;
}


std::string Move::MovetoString() const
{
	int one = bitScanForward(move);
	int two = bitScanForward(move ^ fields[one]);
	
	std::string ans="";
	ans+= two % 8 + 'a';
	ans += two / 8 + '1';
	ans += one % 8 + 'a';
	ans += one / 8 + '1';
	
	
	return ans;
}




