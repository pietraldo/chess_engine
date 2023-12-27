#pragma once

#include <iostream>

#include "MagicBitboards.h"
#include "variables.h"

class GamePrepare
{
public:
	
	static void Prepare(); 
	static void RookAttacks();

private:
	static int rookShifts[M];
	static Bitboard rookMoveForPossition(Bitboard occupancy, int index);
	static Bitboard reverseBits(Bitboard n);
};

