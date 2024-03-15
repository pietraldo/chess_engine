#pragma once
#include <iostream>
#include <string>

#include "variables.h"

using namespace std;

int bit(Bitboard number, Bitboard index);
void printBitboard(Bitboard);
void printBoard(Board);
std::ostream& operator<<(std::ostream& os, const Move& move);
