#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdint>
#include <random>
#include <cstdlib> 

#include "variables.h"
#include "GamePrepare.h"
#include "Functions.h"
#include "Game.h"
#include "MoveGeneration.h"
#include "UciTranslator.h"
#ifdef _WIN32
#include "Test.h"
#endif

#define N 8
#define M 64

using namespace std;



int main()
{
	UciTranslator::startComunication();
}



