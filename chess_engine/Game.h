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


class Game
{
public:
	static int count;
	static string output;

	// check if king of colol king Color is under attack
	static bool isCheck(Board, int kingField, Color kingColor);

	static void makeMove(Board&, Move&);
	static void unmakeMove(Board&, Move&);

	// not checking if king is under check
	static bool isLegalCastleWhiteShort(Board); 
	static bool isLegalCastleWhiteLong(Board); 
	static bool isLegalCastleBlackShort(Board); 
	static bool isLegalCastleBlackLong(Board); 

	static void moveGeneration(Board, Color, list<Move>&);
	static int generation(Board* board, Color color,int max_deth, int depth = 1);
	static void test(Board& b);

	static Board* boardFromFEN(string fen);

private:
	// if move is king move or rook move it changes castle rights in board
	static void changeCastleRights(Board&, Move&);
	// it changes field of on passant
	static void changeOnPassantMove(Board&, Move&);

	// adds to list all moves from white rooks, figure - Bitboard which contains figures
	static void moveGenerationRookWhite(Board&, list<Move>&, Bitboard figure);
	static void moveGenerationBishopWhite(Board&, list<Move>&, Bitboard figure);
	static void moveGenerationKnightWhite(Board&, list<Move>&, Bitboard figure);
	static void moveGenerationPawnWhite(Board&, list<Move>&, Bitboard figure);
	static void moveGenerationKingWhite(Board&, list<Move>&, Bitboard figure);
	
	static void moveGenerationRookBlack(Board&, list<Move>&, Bitboard figure);
	static void moveGenerationBishopBlack(Board&, list<Move>&, Bitboard figure);
	static void moveGenerationKnightBlack(Board&, list<Move>&, Bitboard figure);
	static void moveGenerationPawnBlack(Board&, list<Move>&, Bitboard figure);
	static void moveGenerationKingBlack(Board&, list<Move>&, Bitboard figure);


};

