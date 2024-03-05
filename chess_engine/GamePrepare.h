#pragma once

#include <iostream>
#include <fstream>
#include <cmath>

#include "MagicBitboards.h"
#include "variables.h"


class GamePrepare
{
public:

	GamePrepare(); 


	// rooks
	static Bitboard magicRook[M];
	static Bitboard* attackRook[M];
	static int rookShifts[M];

	// bishops
	static Bitboard magicBishop[M];
	static Bitboard* attacBishop[M];
	static int bishopShifts[M];

	// kights
	static Bitboard attackKnight[M];

	// king
	static Bitboard attackKing[M];

	// pawn
	static Bitboard attackPawn[2][M];
	static Bitboard pawnMoves[2][M];

private:

	void init();
	
	Bitboard reverseBits(Bitboard n);
	
	void RookAttacks(bool read_magic_from_file=true, string path_name="rookMagic.txt");
	Bitboard rookMoveForPossition(Bitboard occupancy, int index);
	Bitboard* generateKeysRook(int index);

	void BishopAttacks(bool read_magic_from_file = true, string path_name = "bishopMagic.txt");
	Bitboard bishopMoveForPossition(Bitboard occupancy, int index);
	Bitboard* generateKeysBishop(int index);

	void KnightAttacks();
	void KingAttacks();

	void PawnAttacks();
	void PawnMoves();
	
	void writeNumbersToFile(string name, Bitboard* numbers);
	void readNumbersFromFile(string name, Bitboard* numbers);
};

