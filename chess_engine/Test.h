#pragma once

#include <iostream>
#include "MoveGeneration.h"
#include "ReadyChessEngine.h"
#include "Game.h"
#include "UciTranslator.h"

class Test
{
public:
	Test();
private:
	bool test1();
	bool test2();
	bool test3();
	bool test4();
	bool test5();
	bool test6();

	string compareMoveGeneration(string stockfish, string myEngine, bool);
};

