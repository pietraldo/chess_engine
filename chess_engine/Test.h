#pragma once

#include <iostream>
#include "MoveGeneration.h"
#include "ReadyChessEngine.h"

class Test
{
public:
	Test();
private:
	bool test1();
	bool test2();

	string compareMoveGeneration(string stockfish, string myEngine, bool);
};

