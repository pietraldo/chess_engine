# chess_engine

## Introduction

I'm building chess engine in c++. Engine is based on Bitboards and MagicBitboards. It uses alpha-beta pruning and quisance search 

## Instalation

### Windows
On Windows you can just open chess_engine.sln in Visual Studio and compile it.

### Linux
1. Clone repository
``` bash
 git clone https://github.com/pietraldo/chess_engine.git
```
2. Go to folder
``` bash
cd chess_engine
cd chess_engine
```
3. Compile
```bash
g++ -O3 -march=native -o game3 prog.cpp Game.cpp Functions.cpp MoveGeneration.cpp UciTranslator.cpp variables.cpp MagicBitboards.cpp GamePrepare.cpp Evaluation.cpp -lm
```

## Usage
I have implemented so far few UCI commends:
### Setting position
Starting position
```bash
position startpos
```
Fen position
```bash
position fen x
```
### Finding how many are position on some depth
```bash
go perft x
```
### Finding best move
```bash
go depth x
```

## Roadmap

Here's the plan:

- [x] Figure out the magic numbers
- [x] Get the move generation working
- [x] Test the move generation
- [x] Speed up the move generation
- [x] Create a way to evaluate the game state
- [x] Build a function to find the best move
- [ ] Meet the UCI standards for chess engines
- [ ] Make Transposible Table
- [ ] Make Multithreading


 
 
 
 
 
 
 
