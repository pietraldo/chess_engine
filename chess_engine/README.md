# chess_engine

## Introduction

I'm building a chess engine in C++, using bitboards and some smart tricks with magic numbers to make it efficient. Right now, my main goal is to make the move generation better.

## Instalation

g++ -O3 -march=native -o game3 prog.cpp Game.cpp Functions.cpp MoveGeneration.cpp UciTranslator.cpp variables.cpp MagicBitboards.cpp GamePrepare.cpp Evaluation.cpp -lm


## Roadmap

Here's the plan:

- [x] Figure out the magic numbers
- [x] Get the move generation working
- [x] Test the move generation
- [ ] Speed up the move generation
- [ ] Create a way to evaluate the game state
- [ ] Build a function to find the best move
- [ ] Meet the UCI standards for chess engines


 
 
 
 
 
 
 