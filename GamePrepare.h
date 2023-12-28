#pragma once

#include <iostream>
#include <fstream>

#include "MagicBitboards.h"
#include "variables.h"


class GamePrepare
{
public:
	
	// rooks
	Bitboard magicRook[M];
	Bitboard* attackRook[M];

	// bishops
	Bitboard magicBishop[M];
	Bitboard* attacBishop[M];

	GamePrepare(); 
	
	static int rookShifts[M];
	static int bishopShifts[M];
private:
	
	
	Bitboard reverseBits(Bitboard n);
	
	void RookAttacks();
	Bitboard rookMoveForPossition(Bitboard occupancy, int index);
	Bitboard* generateKeysRook(int index, Bitboard* combHor, Bitboard* combVer);

	void BishopAttacks();
	Bitboard bishopMoveForPossition(Bitboard occupancy, int index);
	Bitboard* generateKeysBishop(int index);
	
	void writeNumbersToFile(string name, Bitboard* numbers);
	void readNumbersFromFile(string name, Bitboard* numbers);
};

