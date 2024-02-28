#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <intrin.h>
#include <string>


#include <iomanip> 

#include "variables.h"
#include "GamePrepare.h"
#include "Functions.h"
#include "UciTranslator.h"


class MoveGeneration
{
public:
	static int count;
	static string output;

	// check if king of colol king Color is under attack
	static bool isCheck(Board, int kingField, Color kingColor);

	static void makeMove(Board&, Move&);
	static void unmakeMove(Board&, Move&);

	// checking all conditions for making castle
	static bool isLegalCastle(Board&, Color, Castle);

	
	static void generateMoves(Board&, Color, Figure, list<Move>&, Bitboard mask, Bitboard pinned);

	static void generateMovesNew(Board&, Color, list<Move>&);


	static void generateOnlyCaptureMoves(Board&, Color, list<Move>&);
	static void generateOnlyCaptureMoves2(Board&, Color, list<Move>&);

	static int generation(Board* board, Color color, int max_deth, int depth = 1);

	static int goPerft(Board& board, Color color, int max_depth, int depth=1);


	
	static void addMovesToList(Board&, Color, int figure_type, int index_figure, Bitboard attacks, list<Move>&);
	

	// if move is king move or rook move it changes castle rights in board
	static void changeCastleRights(Board&, Move&);

	// it changes field of on passant
	static void changeOnPassantMove(Board&, Move&);

	// returns Bitboard of fields that figure is attacking in this possition
	static Bitboard attackRookf(Board&, int index, Color);
	static Bitboard attackBishopf(Board&, int index, Color);
	static Bitboard attackQueenf(Board&, int index, Color);
	static Bitboard attackKnightf(Board&, int index, Color);
	static Bitboard attackKingf(Board&, int index, Color);

	static Bitboard attackPawnf(Board&, int index, Color);
	static void addPawnMoves(Board&, Color, list<Move>&, Bitboard mask, Bitboard pinned);

	// check if castle is legal and if so, adds to move list
	static void addCastleMove(Board&, Color, list<Move>&);

	// list of functions that generates moves
	static constexpr Bitboard(*attacks_funcitons[])(Board&, int, Color) = { attackRookf, attackBishopf, attackKnightf, attackPawnf, attackQueenf, attackKingf };
};















































































