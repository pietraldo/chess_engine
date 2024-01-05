#pragma once

#include <iostream>
#include <fstream>

#include "variables.h"
#include "GamePrepare.h"


class Game
{
public:
	
	static bool isCheck(Board, int kingField, Color kingColor);

	// not checking if king is under check
	static bool isLegalCastleWhiteShort(Board); 
	static bool isLegalCastleWhiteLong(Board); 
	static bool isLegalCastleBlackShort(Board); 
	static bool isLegalCastleBlackLong(Board); 

	static void moveGeneration(Board, Color);


private:



};

