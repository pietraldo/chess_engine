#include "MagicBitboards.h"

MagicBitboards::MagicBitboards()
{
	srand(seed);
}

Bitboard MagicBitboards::random_bitboard() {

	Bitboard u1, u2, u3, u4;

	u1 = (Bitboard)(rand()) & 0xFFFF; 
	u2 = (Bitboard)(rand()) & 0xFFFF;
	u3 = (Bitboard)(rand()) & 0xFFFF;
	u4 = (Bitboard)(rand()) & 0xFFFF;

	return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

Bitboard MagicBitboards::random_bitboard_fewbits() 
{
	return random_bitboard() & random_bitboard() & random_bitboard();
}


// function finds bitbord b that (key*b)>>(M-relativeBits) is a key to hash function
Bitboard MagicBitboards::findMagicNumber(Bitboard* keys, int keys_size, int relativeBits)
{
	
	Bitboard magic = 0;
	while (1)
	{
		magic = random_bitboard_fewbits(); // generating magic number
		
		// checking if magic number is correct
		bool* used = new bool[keys_size];

		for (int i = 0; i < keys_size; i++)
			used[i] = false;

		for (int i = 0; i < keys_size; i++)
		{
			int ind_arr = (keys[i] * magic) >> (64 - relativeBits);
			if (used[ind_arr])
				break;
			used[ind_arr] = true;
		}

		bool git = true;
		for (int i = 0; i < keys_size; i++)
			if (used[i] == false)
				git = false;

		if (git == true)
		{
			delete[] used;
			return magic;
		}


		delete[] used;
	}
	return magic;
}