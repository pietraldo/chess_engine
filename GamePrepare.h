#pragma once

#include <iostream>
#include <fstream>

#include "MagicBitboards.h"
#include "variables.h"


class GamePrepare
{
public:

	GamePrepare(); 


	// rooks
	Bitboard magicRook[M];
	Bitboard* attackRook[M];
	static int rookShifts[M];

	// bishops
	Bitboard magicBishop[M];
	Bitboard* attacBishop[M];
	static int bishopShifts[M];

	// kights
	Bitboard attackKnight[M];

	// king
	Bitboard attackKing[M];

	// pawn
	Bitboard attackPawn[M];
	Bitboard pawnMoves[M];
	
	static void init();

private:
	
	
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

