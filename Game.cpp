#include "Game.h"

bool Game::isCheck(Board board, int kingField, Color kingColor)
{
	if (kingColor == WHITE)
	{
		Bitboard occ = board.occupancyW | board.occupancyB;
		
		// rook
		Bitboard mask = occ&rookMask[kingField];
		Bitboard attackRook=GamePrepare::attackRook[kingField][(mask*GamePrepare::magicRook[kingField]) >> (M - GamePrepare::rookShifts[kingField])];
		if ((attackRook & board.rookB) != 0|| (attackRook & board.queenB) != 0) return true;
		
		// bishop
		mask = occ & bishopMask[kingField];
		Bitboard attackBishop = GamePrepare::attacBishop[kingField][(mask * GamePrepare::magicBishop[kingField]) >> (M - GamePrepare::bishopShifts[kingField])];
		if (((attackBishop & board.bishopB) != 0) || ((attackBishop & board.queenB) != 0)) return true;
		
		// knight
		if ((GamePrepare::attackKnight[kingField] & board.knightB) != 0) return true;

		// pawn
		if ((GamePrepare::attackPawnW[kingField] & board.pawnB) != 0) return true;
		return false;
	}
	
	Bitboard occ = board.occupancyW | board.occupancyB;

	// rook
	Bitboard mask = occ & rookMask[kingField];
	Bitboard attackRook = GamePrepare::attackRook[kingField][(mask * GamePrepare::magicRook[kingField]) >> (M - GamePrepare::rookShifts[kingField])];
	if ((attackRook & board.rookW) != 0 || (attackRook & board.queenW) != 0) return true;

	// bishop
	mask = occ & bishopMask[kingField];
	Bitboard attackBishop = GamePrepare::attacBishop[kingField][(mask * GamePrepare::magicBishop[kingField]) >> (M - GamePrepare::bishopShifts[kingField])];
	if (((attackBishop & board.bishopW) != 0) || ((attackBishop & board.queenW) != 0)) return true;

	// knight
	if ((GamePrepare::attackKnight[kingField] & board.knightW) != 0) return true;

	// pawn
	if ((GamePrepare::attackPawnB[kingField] & board.pawnW) != 0) return true;
	
	return false;
}

bool Game::isLegalCastleWhiteShort(Board b)
{
	// checking if rook and king are on correct fields
	if ((b.rookW & H1 )== 0 || (b.kingW & E1) == 0) return false;
	
	// checking if field F1 and G1 are under attack
	if (isCheck(b, 5, WHITE) || isCheck(b, 6, WHITE)) return false;

	return true;
}
bool Game::isLegalCastleWhiteLong(Board b)
{
	// checking if rook and king are on correct fields
	if ((b.rookW & A1) == 0 || (b.kingW & E1) == 0) return false;

	// checking if field F1 and G1 are under attack
	if (isCheck(b, 2, WHITE) || isCheck(b, 3, WHITE)) return false;

	return true;
}
bool Game::isLegalCastleBlackShort(Board b)
{
	// checking if rook and king are on correct fields
	if ((b.rookB & H8) == 0 || (b.kingB & E8) == 0) return false;

	// checking if field F1 and G1 are under attack
	if (isCheck(b, 61, BLACK) || isCheck(b, 62, BLACK)) return false;

	return true;
}
bool Game::isLegalCastleBlackLong(Board b)
{
	if ((b.rookB & A8) == 0 || (b.kingB & E8) == 0) return false;

	// checking if field F1 and G1 are under attack
	if (isCheck(b, 58, BLACK) || isCheck(b, 59, BLACK)) return false;

	return true;
}

void Game::moveGeneration(Board b, Color)
{

}