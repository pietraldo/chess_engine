#include "GamePrepare.h"


 int GamePrepare::rookShifts[M] = {

			12, 11, 11, 11, 11, 11, 11, 12,
			11, 10, 10, 10, 10, 10, 10, 11,
			11, 10, 10, 10, 10, 10, 10, 11,
			11, 10, 10, 10, 10, 10, 10, 11,
			11, 10, 10, 10, 10, 10, 10, 11,
			11, 10, 10, 10, 10, 10, 10, 11,
			11, 10, 10, 10, 10, 10, 10, 11,
			12, 11, 11, 11, 11, 11, 11, 12,
 };

 int GamePrepare::bishopShifts[M] = {
			6, 5, 5, 5, 5, 5, 5, 6,
			5, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 7, 7, 7, 7, 5, 5,
			5, 5, 7, 9, 9, 7, 5, 5,
			5, 5, 7, 9, 9, 7, 5, 5,
			5, 5, 7, 7, 7, 7, 5, 5,
			5, 5, 5, 5, 5, 5, 5, 5,
			6, 5, 5, 5, 5, 5, 5, 6,
 };

 GamePrepare::GamePrepare()
 {
	 readNumbersFromFile("rookmagic.txt", magicRook);
	 readNumbersFromFile("bishopMagic.txt", magicBishop);

	 RookAttacks();

	 Bitboard occupancy2 = A3 | D4 | G8 | F4 | D5 | E7 | C5 | F2 | H4 | D3 | B4; // zajete pola na planszy
	 Bitboard bishop = E3; // pole na ktorej jest wieza

	 //printBoard(bishopMoveForPossition(occupancy2, 20));

	 BishopAttacks();
 }

 Bitboard GamePrepare::rookMoveForPossition(Bitboard occupancy, int index)
 {
	 
	 Bitboard file = fields[index];
	 int m = index / 8;
	 int n = index % 8;


	 Bitboard row = Rank1BB<<(m*N);
	 Bitboard column = FileABB<<n;
	
	 Bitboard o = occupancy;
	 Bitboard s = file;
	 Bitboard sReverse = reverseBits(s);

	 // generate attacks
	 Bitboard EastAttack = (occupancy ^ ((occupancy & row) - 2 * file)) & row;
	 Bitboard NorthAttack = (occupancy ^ ((occupancy & column) - 2 * file)) & column;
	 Bitboard SouthAttack = reverseBits(reverseBits(o & column) ^ (reverseBits(o & column) - (2 * sReverse))) & column;
	 Bitboard WestAttack = reverseBits(reverseBits(o & row) ^ (reverseBits(o & row) - (2 * sReverse))) & row;

	 return  EastAttack | NorthAttack | SouthAttack | WestAttack;
 }

 Bitboard GamePrepare::bishopMoveForPossition(Bitboard occupancy, int index)
 {
	 int m = index / 8;
	 int n = index % 8;


	 Bitboard ans = 0ULL;

	 auto add = [&](int k, int j) {
		 int y = index / 8;
		 int x = index % 8;
		 while (x + j < 8 && x + j >= 0 && y + k < 8 && y + k >= 0)
		 {
			 ans |= fields[(y + k) * 8 + (x + j)];
			 
			 if (0 != (fields[(y + k) * 8 + (x + j)] & occupancy))
				 break;

			 x += j;
			 y += k;
		 }
	 };

	 add(-1, 1);
	 add(1, 1);
	 add(1, -1);
	 add(-1, -1);



	 return  ans;
	 //Bitboard file = fields[index];
	 //int m = index / 8;
	 //int n = index % 8;


	 //Bitboard diagonal1=0ULL;
	 //Bitboard diagonal2=0ULL;

	 //auto add = [&](int k, int j, Bitboard* b) {
		// int y = index / 8;
		// int x = index % 8;
		// while (x + j < 8 && x + j >= 0 && y + k < 8 && y + k >= 0)
		// {
		//	 *b |= fields[(y + k) * 8 + (x + j)];
		//	 x += j;
		//	 y += k;
		// }
	 //};

	 //add(-1, 1, &diagonal2);
	 //add(1, 1, &diagonal1);
	 //add(1, -1, &diagonal1);
	 //add(-1, -1, &diagonal2);

	 //
	 //Bitboard o = occupancy;
	 //Bitboard s = file;
	 //Bitboard sReverse = reverseBits(s);

	 //// generate attacks
	 //Bitboard EastAttack = (occupancy ^ ((occupancy & diagonal1) - 2 * file)) & diagonal1;
	 //Bitboard NorthAttack = (occupancy ^ ((occupancy & diagonal1) - 2 * file)) & diagonal1;
	 //Bitboard SouthAttack = reverseBits(reverseBits(o & diagonal2) ^ (reverseBits(o & diagonal2) - (2 * sReverse))) & diagonal2;
	 //Bitboard WestAttack = reverseBits(reverseBits(o & diagonal2) ^ (reverseBits(o & diagonal2) - (2 * sReverse))) & diagonal2;

	 //return  EastAttack | NorthAttack | SouthAttack | WestAttack;
 }


 Bitboard GamePrepare::reverseBits(Bitboard n) {
	 Bitboard result = 0;
	 for (int i = 0; i < 64; ++i) {
		 result = (result << 1) | (n & 1);
		 n >>= 1;
	 }
	 return result;
 }

 Bitboard* GamePrepare::generateKeysRook(int index, Bitboard* combHor, Bitboard* combVer)
 {
	 int m = index % N;
	 int n = index / N;
	 int p = exp2(N - 2);

	 // generate all keys for file index

	 int size = rookShifts[index];


	 Bitboard* keys = new Bitboard[exp2(size)];
	 int t = 0;
	 for (int i = 0; i < p; i++)
	 {
		 for (int j = 0; j < p; j++)
		 {
			 Bitboard key_n = (combVer[i] << m) | (combHor[j] << (n * N));
			 if ((key_n & fields[index]) != 0)
				 key_n ^= fields[index];

			 bool already_there = false;
			 for (int k = 0; k < t; k++)
			 {
				 if (keys[k] == key_n)
				 {
					 already_there = true;
					 break;
				 }
			 }
			 if (!already_there)
			 {
				 keys[t] = key_n;
				 t++;
			 }

			
		 }
	 }
	 return keys;
 }

 void GamePrepare::writeNumbersToFile(string name, Bitboard* numbers)
 {
	 ofstream file(name);
	 for (int i = 0; i < M; i++)
		 file << numbers[i] << endl;
	 file.close();
 }

 void GamePrepare::readNumbersFromFile(string name, Bitboard* numbers)
 {
	 ifstream file(name);
	 for (int i = 0; i < M; i++)
		 file >> numbers[i];
	 file.close();
 }

 void GamePrepare::RookAttacks()
 {
	 // this are all possible combinations of 
	 int p = exp2(N-2);
	 Bitboard* combHor = new Bitboard[p];
	 Bitboard* combVer = new Bitboard[p];

	 for (int i = 0; i < p; i++)
	 {
		 combHor[i] = i;
		 combVer[i] = 0;
	 }
		 
	 for (int i = 0; i < p; i++)
	 {
		 for (int j = 0; j < N-2; j++)
		 {
			 combVer[i] |= (0 != (combHor[i] & fields[j])) ? fields[j * N] : 0;
		 }
		 combVer[i] <<= N;
	 }
	 for (int i = 0; i < p; i++)
		 combHor[i] <<=  1;
	 
	 for (int index = 0; index <M; index++)
	 {
		 int size = rookShifts[index];


		 attackRook[index] = new Bitboard[exp2(size)];
		 Bitboard* keys = generateKeysRook(index, combHor, combVer);

		 
		 // reading magic from file
		 Bitboard magic = magicRook[index];

		 // genreatiting magic

		 /*MagicBitboards mg = MagicBitboards();
		 magic = mg.findMagicNumber(keys, exp2(size), size);
		 magicRook[index] = magic;
		 cout << magicRook[index] << endl;*/

		 // generating attacks for rooks
		 for (int i = 0; i < exp2(size); i++)
		 {
			 attackRook[index][(keys[i] * magic) >> (M - size)] = rookMoveForPossition(keys[i], index);
		 }

		 delete[] keys;
	 }

	 // just need to write to file on the begining while generating numbers
	 // writeNumbersToFile("rookMagic.txt", magicRook);

 }

 Bitboard* GamePrepare::generateKeysBishop(int index)
 {
	 int size = bishopShifts[index];

	 Bitboard* keys = new Bitboard[exp2(size)];
	 for (int i = 0; i < exp2(size); i++)
	 {
		 keys[i] = 0;

		 int n = 0;
		 for (int j = 0; j < M; j++)
		 {
			 if ((fields[j] & bishopMask[index]) != 0)
			 {
				 if ((fields[n] & i) != 0)
					 keys[i] |= fields[j];
				 n++;
			 }
		 }
		 
	 }

	 return keys;
 }

 void GamePrepare::BishopAttacks()
 {
	 for (int index = 0; index < M; index++)
	 {
		 int size = bishopShifts[index];


		 attacBishop[index] = new Bitboard[exp2(size)];
		 Bitboard* keys = generateKeysBishop(index);


		 // reading magic from file
		 Bitboard magic = magicBishop[index];

		 // genreatiting magic

		 //MagicBitboards mg = MagicBitboards();
		 //Bitboard magic = mg.findMagicNumber(keys, exp2(size), size);
		 //magicBishop[index] = magic;
		 //cout << magicBishop[index] << endl;

		 // generating attacks for bishops
		 for (int i = 0; i < exp2(size); i++)
		 {
			 attacBishop[index][(keys[i] * magic) >> (M - size)] = bishopMoveForPossition(keys[i], index);
		 }

		 delete[] keys;
	 }

	 // just need to write to file on the begining while generating numbers
	 //writeNumbersToFile("bishopMagic.txt", magicBishop);

 }