#pragma once

#include <iostream>
#include "MoveGeneration.h"
#include "ReadyChessEngine.h"
#include "Game.h"

class Test
{
public:
	Test();
private:
	bool test1();
	bool test2();
	bool test3();
	bool test4();

	string compareMoveGeneration(string stockfish, string myEngine, bool);
};

