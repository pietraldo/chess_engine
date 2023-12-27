#include "GamePrepare.h"

 void  GamePrepare::Prepare()
 { 
	 MagicBitboards mg =  MagicBitboards();
	 RookAttacks();
 }


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

 Bitboard GamePrepare::reverseBits(Bitboard n) {
	 Bitboard result = 0;
	 for (int i = 0; i < 64; ++i) {
		 result = (result << 1) | (n & 1);
		 n >>= 1;
	 }
	 return result;
 }

 void GamePrepare::RookAttacks()
 {

	 Bitboard* attacks[M];

	 // this are all possible combinations of 
	 int p = exp2(N-2);
	 Bitboard* combHor = new Bitboard[p];
	 Bitboard* combVer = new Bitboard[p];

	 Bitboard magicBitbords[64];

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

		 int m = index % N;
		 int n = index / N;


		 // generate all keys for file index

		 int size = rookShifts[index];

		 attacks[index] = new Bitboard[exp2(size)];

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
					
				 //printBoard(keys[t]);
			 }
		 }

		 // genreatiting magic

		 MagicBitboards mg = MagicBitboards();
		 Bitboard magic = mg.findMagicNumber(keys, exp2(size), size);
		 magicBitbords[index] = magic;
		 cout << magicBitbords[index] << endl;

		 int* count = new int[exp2(size)];

		 for (int i = 0; i < exp2(size); i++)
			 count[i] = 0;

		 for (int i = 0; i < exp2(size); i++)
		 {
			 int indeks = (keys[i] * magic) >> (M - size);
			 count[indeks]++;
		 }

		 for (int i = 0; i < exp2(size); i++)
		 {
			 /*if (count[i] != 0)
				 cout << i << " -> " << count[i] << endl;*/
		 }

		 for (int i = 0; i < exp2(size); i++)
		 {
			 attacks[index][(keys[i] * magic) >> (M - size)] = rookMoveForPossition(keys[i], index);
			 //printBoard(keys[i]);
			 //printBoard(attacks[index][(keys[i] * magic) >> (M - size)]);
		 }

		 


		 delete[] count;
		 delete[] keys;
	 }

	 Bitboard occupancy = A3 | D4 | G8|F4|D5 | E7 | C5 | F2 | H4|D3|B4; // zajete pola na planszy
	 Bitboard rook = D6; // pole na ktorej jest wieza
	 printBoard(occupancy); // wyswietlanie planszy

	 // generowanie wszystkich ruchow
	 int num = log2(rook);
	 occupancy &= (rookFiles[num]);
	 Bitboard att = attacks[num][occupancy * magicBitbords[num] >> (M - rookShifts[num])];
	
	
	 printBoard(att);// wyswietlanie wszystkich ruchow
 }