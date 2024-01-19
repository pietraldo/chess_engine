#pragma once

#include <random>
#include <iostream>

#include "variables.h"

using namespace std;


using Bitboard = uint64_t;

class MagicBitboards
{
public:

	MagicBitboards();
	Bitboard findMagicNumber(Bitboard* keys, int keys_size,  int relativeBits);

private:
	const unsigned int seed = 5882312;
	Bitboard random_bitboard();
	Bitboard random_bitboard_fewbits();
};

