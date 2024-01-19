#pragma once

#include <iostream>
#include "Game.h"
#include "ReadyChessEngine.h"

class Test
{
public:
	Test();
private:
	bool test1();

	string compareMoveGeneration(string stockfish, string myEngine, bool);
};

