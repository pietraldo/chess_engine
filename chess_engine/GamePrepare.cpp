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

 Bitboard* GamePrepare::attacBishop[M];
 Bitboard GamePrepare::magicBishop[M];

 Bitboard GamePrepare::magicRook[M];
 Bitboard* GamePrepare::attackRook[M];
 Bitboard GamePrepare::attackKnight[M];
 Bitboard GamePrepare::attackKing[M];
 Bitboard GamePrepare::attackPawnW[M];
 Bitboard GamePrepare::pawnMovesW[M];
 Bitboard GamePrepare::attackPawnB[M];
 Bitboard GamePrepare::pawnMovesB[M];

 GamePrepare::GamePrepare()
 {
	 init();

	 RookAttacks();
	 BishopAttacks();
	 KnightAttacks();
	 KingAttacks();
	 PawnAttacks();

	 PawnMoves();
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
 }

 void GamePrepare::KnightAttacks()
 {
	 for (int i = 0; i < M; i++)
	 {
		 attackKnight[i] = 0;
		 if (i / 8 <= 6 && i % 8 >= 2)
			 attackKnight[i] |= fields[i + 6];
		 if (i / 8 <= 5 && i % 8 >= 1)
			 attackKnight[i] |= fields[i + 15];
		 if (i / 8 <= 5 && i % 8 <= 6)
			 attackKnight[i] |= fields[i + 17];
		 if (i / 8 <= 6 && i % 8 <= 5)
			 attackKnight[i] |= fields[i + 10];
		 if (i / 8 >= 1 && i % 8 >= 2)
			 attackKnight[i] |= fields[i - 10];
		 if (i / 8 >= 2 && i % 8 >= 1)
			 attackKnight[i] |= fields[i - 17];
		 if (i / 8 >= 2 && i % 8 <= 6)
			 attackKnight[i] |= fields[i - 15];
		 if (i / 8 >= 1 && i % 8 <= 5)
			 attackKnight[i] |= fields[i - 6];

	 }
 }

 void GamePrepare::KingAttacks()
 {
	 for (int i = 0; i < M; i++)
	 {
		 attackKing[i] = 0;
		 if (i/8 >= 1&& i%8>=1 ) // down left
		 {
			 attackKing[i] |= fields[i-9];
		 }
		 if (i/8 >=1  ) // down middle
		 {
			 attackKing[i] |= fields[i - 8];
		 }
		 if (i/8>=1 && i%8 <=6) // down right
		 {
			 attackKing[i] |= fields[i - 7];
		 }
		 if ( i%8>=1 ) // midle left
		 {
			 attackKing[i] |= fields[i - 1];
		 }
		 if (i%8 <=6) // midle right
		 {
			 attackKing[i] |= fields[i +1];
		 }
		 if (i/8<=6 && i%8>=1 ) // top left
		 {
			 attackKing[i] |= fields[i +7];
		 }
		 if (i/8 <=6) // top middle
		 {
			 attackKing[i] |= fields[i + 8];
		 }
		 if (i/8 <=6 && i%8<=6 ) // top right
		 {
			 attackKing[i] |= fields[i+ 9];
		 }
	 }
 }

 Bitboard GamePrepare::reverseBits(Bitboard n) {
	 Bitboard result = 0;
	 for (int i = 0; i < 64; ++i) {
		 result = (result << 1) | (n & 1);
		 n >>= 1;
	 }
	 return result;
 }

 Bitboard* GamePrepare::generateKeysRook(int index)
 {
	 int m = index % N;
	 int n = index / N;
	 int p = exp2(N - 2);

	 Bitboard* combHor = new Bitboard[p];
	 Bitboard* combVer = new Bitboard[p];

	 for (int i = 0; i < p; i++)
	 {
		 combHor[i] = i;
		 combVer[i] = 0;
	 }

	 for (int i = 0; i < p; i++)
	 {
		 for (int j = 0; j < N - 2; j++)
		 {
			 combVer[i] |= (0 != (combHor[i] & fields[j])) ? fields[j * N] : 0;
		 }
		 combVer[i] <<= N;
	 }
	 for (int i = 0; i < p; i++)
		 combHor[i] <<= 1;

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

 void GamePrepare::RookAttacks(bool read_magic_from_file, string path_name)
 {
	 if(read_magic_from_file)
		 readNumbersFromFile(path_name, magicRook);

	 // this are all possible combinations of 
	 int p = exp2(N-2);

	 for (int index = 0; index <M; index++)
	 {
		 int size = rookShifts[index];


		 attackRook[index] = new Bitboard[exp2(size)];
		 Bitboard* keys = generateKeysRook(index);

		 
		 
		 Bitboard magic;
		 if (read_magic_from_file)
			 magic = magicRook[index];
		 else // generate again magic numbers
		 {
			 MagicBitboards mg = MagicBitboards();
			 magic = mg.findMagicNumber(keys, exp2(size), size);
			 magicRook[index] = magic;
			 cout << magicRook[index] << endl;
		 }
		 
		 // generating attacks for rooks
		 for (int i = 0; i < exp2(size); i++)
		 {
			 attackRook[index][(keys[i] * magic) >> (M - size)] = rookMoveForPossition(keys[i], index);
		 }

		 delete[] keys;
	 }

	 // write to the file 
	 if(!read_magic_from_file)
		writeNumbersToFile(path_name, magicRook);

 }

 void GamePrepare::PawnMoves()
 {
	 for (int i = 0; i < M; i++)
	 {
		 pawnMovesB[i] = 0;
		 pawnMovesW[i] = 0;
		 if ((int)(i / 8) == 1)
			 pawnMovesW[i] |= fields[i + 16];
		 if ((int)(i / 8) == 6)
			 pawnMovesB[i] |= fields[i - 16];
		 if (i / 8 >= 1 && i / 8 <= 6)
		 {
			pawnMovesB[i] |= fields[i - 8];
			pawnMovesW[i] |= fields[i + 8];
		 } 
	 }
 }

 void GamePrepare::PawnAttacks()
 {
	 // TODO: zrobic attaki na pierwszymi i ostatnim rzedzie
	 for (int i = 0; i < M; i++)
	 {
		attackPawnW[i] = 0;
		attackPawnB[i] = 0;

		if (i % 8 >= 1 && i <= 55)
		{
			attackPawnW[i] |= fields[i + 7];
		}
		if (i >= 8 && i % 8 >= 1)
		{
		attackPawnB[i] |= fields[i - 9];
		}
		if (i >= 8 && i % 8 <= 6)
		{
			attackPawnB[i] |= fields[i - 7];
		}
		if (i % 8 <= 6 && i <= 55)
		{
			attackPawnW[i] |= fields[i + 9];
		} 
	 }
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

 void GamePrepare::BishopAttacks(bool read_magic_from_file, string path_name)
 {
	 if (read_magic_from_file)
		 readNumbersFromFile(path_name, magicBishop);

	 for (int index = 0; index < M; index++)
	 {
		 int size = bishopShifts[index];


		 attacBishop[index] = new Bitboard[exp2(size)];
		 Bitboard* keys = generateKeysBishop(index);


		 // reading magic from file
		 Bitboard magic;
		 if(read_magic_from_file)
			 magic=magicBishop[index];
		 else
		 {
			 MagicBitboards mg = MagicBitboards();
			 magic = mg.findMagicNumber(keys, exp2(size), size);
			 magicBishop[index] = magic;
			 cout << magicBishop[index] << endl;
		 }
	

		 // generating attacks for bishops
		 for (int i = 0; i < exp2(size); i++)
		 {
			 attacBishop[index][(keys[i] * magic) >> (M - size)] = bishopMoveForPossition(keys[i], index);
		 }

		 delete[] keys;
	 }

	 
	 if (!read_magic_from_file)
		 writeNumbersToFile(path_name, magicBishop);
 }

 void GamePrepare::init()
 {
	 // initializing variable felds - fields[0] = A1 fields[1] = B1 and so on...
	 fields[0] = 1ULL;
	 for (int i = 1; i < M; i++)
	 {
		 fields[i] = 1ULL << i;
	 }
	 fields[M] = 0;

	 // initalizing variable vertical - veritical[0] = first column and vertical[1] = second column
	 vertical[0] = A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8;
	 for (int i = 1; i < N; i++)
		 vertical[i] = vertical[0] << i;

	 // horizontal variable
	 horizontal[0] = A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1;
	 for (int i = 1; i < N; i++)
		 horizontal[i] = horizontal[0] << (i * 8);


	 // rook mask
	 Bitboard frame = horizontal[0] | horizontal[N - 1] | vertical[0] | vertical[N - 1];
	 for (int i = 0; i < N; i++)
	 {
		 for (int j = 0; j < N; j++)
		 {
			 rookMask[i * N + j] = horizontal[i] | vertical[j];
			 rookMask[i * N + j] ^= (rookMask[i * N + j] & frame);


			 if (i == 0)
				 rookMask[i * N + j] |= (horizontal[i] ^ (fields[0] | fields[N - 1]));
			 else if (i == (N - 1))
				 rookMask[i * N + j] |= (horizontal[i] ^ (fields[(N - 1) * N] | fields[N * N - 1]));

			 if (j == 0)
				 rookMask[i * N + j] |= (vertical[j] ^ (fields[0] | fields[(N - 1) * N]));
			 else if (j == (N - 1))
				 rookMask[i * N + j] |= (vertical[j] ^ (fields[N - 1] | fields[N * N - 1]));

			 if ((fields[i * N + j] & rookMask[i * N + j]) != 0)
				 rookMask[i * N + j] ^= fields[i * N + j];
		 }
	 }

	 // bishop mask
	 for (int i = 0; i < M; i++)
	 {
		 bishopMask[i] = 0;

		 auto add = [&](int k, int j) {
			 int y = i / 8;
			 int x = i % 8;
			 while (x + j < 7 && x + j >= 1 && y + k < 7 && y + k >= 1)
			 {
				 bishopMask[i] |= fields[(y + k) * 8 + (x + j)];
				 x += j;
				 y += k;
			 }
		 };

		 add(-1, 1);
		 add(1, 1);
		 add(1, -1);
		 add(-1, -1);
	 }

	 // testing 
	 /*cout<<endl<<endl << "E5" << endl;
	 printBoard(bishopMask[h1(E5)]);
	 cout<<endl<<endl << "H1" << endl;
	 printBoard(bishopMask[h1(H1)]);
	 cout<<endl<<endl << "A8" << endl;
	 printBoard(bishopMask[h1(A8)]);
	 cout<<endl<<endl << "F1" << endl;
	 printBoard(bishopMask[h1(F1)]);
	 cout<<endl<<endl << "H4" << endl;
	 printBoard(bishopMask[h1(H4)]);*/
 }
