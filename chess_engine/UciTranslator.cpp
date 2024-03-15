#include "UciTranslator.h"

int MAX_TIME = 5;

string UciTranslator::TranslateMove(Board& board, Move& m)
{
	int one = bitScanForward(m.move);
	int two = bitScanForward(m.move ^ fields[one]);
	Bitboard aa = ((board.whoToMove == WHITE) ? board.occupancy[WHITE] : board.occupancy[BLACK]);

	if (fields[one] & aa)
		swap(two, one);
	
	std::string ans = "";
	ans += two % 8 + 'a';
	ans += two / 8 + '1';
	ans += one % 8 + 'a';
	ans += one / 8 + '1';

	string promotion = "";
	if (m.move3 != 0)
	{
		switch (m.type_piece3)
		{
			case 0:
				promotion = "r";
				break;
			case 1:
				promotion = "b";
				break;
			case 2:
				promotion = "b";
				break;
			case 4:
				promotion = "q";
				break;
		}
		int promotion_field = bitScanForward(m.move3);
		one = bitScanForward(m.move);
		string ans2="";
		ans2 += one % 8 + 'a';
		ans2 += one / 8 + '1';
		ans2 += promotion_field % 8 + 'a';
		ans2 += promotion_field / 8 + '1';
		
		return ans2+promotion;
	}
	
	return ans;
}

Board* UciTranslator::boardFromFEN(string fen)
{
	// example string: r4rk1/1bpq1ppp/pp2p3/3nP3/PbBP4/2N3B1/1P2QPPP/R2R2K1 w - - 1 18

	// substracting first part from string
	size_t pos = fen.find(' ');
	string position = fen.substr(0, pos);

	fen = fen.substr(pos + 1);
	pos = fen.find(' ');
	string move = fen.substr(0, pos);

	fen = fen.substr(pos + 1);
	pos = fen.find(' ');
	string castle = fen.substr(0, fen.find(' '));

	fen = fen.substr(pos + 1);
	pos = fen.find(' ');
	string onPassantField = fen.substr(0, fen.find(' '));

	Board* board = new Board();
	board->whoToMove = move == "w" ? WHITE : BLACK;
	board->onPassantField = onPassantField == "-" ? 64 : int(onPassantField[0] - 97) + (int)(onPassantField[1] - '1') * 8;
	board->castleRights[WHITE][SHORTCASTLE] = castle.find("K") != string::npos;
	board->castleRights[WHITE][LONGCASTLE] = castle.find("Q") != string::npos;
	board->castleRights[BLACK][SHORTCASTLE] = castle.find("k") != string::npos;
	board->castleRights[BLACK][LONGCASTLE] = castle.find("q") != string::npos;

	int row = 7;
	int column = 0;
	for (int i = 0; i < position.size(); i++)
	{
		switch (position[i])
		{
		case 'r':
			board->figure[BLACK][ROOK] |= fields[row * N + column];
			break;
		case 'R':
			board->figure[WHITE][ROOK] |= fields[row * N + column];
			break;
		case 'q':
			board->figure[BLACK][QUEEN] |= fields[row * N + column];
			break;
		case 'Q':
			board->figure[WHITE][QUEEN] |= fields[row * N + column];
			break;
		case 'k':
			board->figure[BLACK][KING] |= fields[row * N + column];
			break;
		case 'K':
			board->figure[WHITE][KING] |= fields[row * N + column];
			break;
		case 'p':
			board->figure[BLACK][PAWN] |= fields[row * N + column];
			break;
		case 'P':
			board->figure[WHITE][PAWN] |= fields[row * N + column];
			break;
		case 'N':
			board->figure[WHITE][KNIGHT] |= fields[row * N + column];
			break;
		case 'n':
			board->figure[BLACK][KNIGHT] |= fields[row * N + column];
			break;
		case 'B':
			board->figure[WHITE][BISHOP] |= fields[row * N + column];
			break;
		case 'b':
			board->figure[BLACK][BISHOP] |= fields[row * N + column];
			break;
		case '/':
			column = 0;
			row--;
			continue;
			break;
		default:
			column += (int)(position[i] - '1');
			break;
		}
		column++;
	}
	for (int i = 0; i < 6; i++)
	{
		board->occupancy[WHITE] |= board->figure[WHITE][i];
		board->occupancy[BLACK] |= board->figure[BLACK][i];
	}
	return board;
}

int UciTranslator::goPerft(int depth)
{
	int nodes = MoveGeneration::goPerft(board, board.whoToMove, depth);
	cout << "Number of nodes: " << nodes << endl;
	return nodes;
}

void UciTranslator::showBoard()
{
	printBoard(board);
}

void UciTranslator::readBoard(Board& b)
{
	board = Board(b);
}

void UciTranslator::getBestMove(int depth, int maxTime)
{
	auto start = std::chrono::high_resolution_clock::now();

	Move bestMove=Game::PickBestMove(board, board.whoToMove,depth, maxTime);
	
	auto stop = std::chrono::high_resolution_clock::now();
	double seconds = (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000000;

	
	//cout << "Time: " << seconds << endl << endl;
	giveResponse("bestmove " + TranslateMove(board, bestMove));
}

void UciTranslator::startComunication()
{
	cout << "Kuba Engine :)" << endl;
	
	UciTranslator tr=UciTranslator();
	
	std::string line;

	while (std::getline(std::cin, line)) 
	{
		write_to_file("consolelog.txt", "lichess: "+line);
		vector<string> vec = devideStringMoves(line);
		int size = vec.size();

		if (vec[0] == "position")
		{
			if (size>1 && vec[1] == "fen")
			{
				string fen = "";
				for (int i = 0; i < 6; i++)
				{
					fen += vec[i + 2]+" ";
				}

				Board* b = UciTranslator::boardFromFEN(fen);
				tr.readBoard(*b);

				if (size >= 9 && vec[8]=="moves")
				{
					vector<string> ss(vec.begin() + 9, vec.begin()+size);
					tr.loadAlgebraicMoves(ss);
				}
			}
			else if (vec[1] == "startpos")
			{
				string startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
				Board* b = UciTranslator::boardFromFEN(startpos);
				tr.readBoard(*b);

				if (size >= 3 && vec[2] == "moves")
				{
					vector<string> ss(vec.begin() + 3, vec.begin() + size);
					tr.loadAlgebraicMoves(ss);
				}
			}
		}
		else if (vec[0] == "d")
		{
			tr.showBoard();
		}
		else if (vec[0] == "uci")
		{
			tr.giveResponse("uciok");
		}
		else if (vec[0] == "isready")
		{
			tr.giveResponse("readyok");
		}
		else if (vec[0] == "go")
		{
			int depth = 80;
			tr.getBestMove(depth, MAX_TIME);
			continue;

			if (vec[1] == "perft")
			{
				int depth = stoi(vec[2]);
				tr.goPerft(depth);
			}
			else if (vec[1] == "depth")
			{
				int depth = stoi(vec[2]);
				//cout << depth << endl;
				tr.getBestMove(depth, MAX_TIME);
				//tr.quit();
			}
			else if (vec[1] == "movetime")
			{
				int depth = stoi(vec[2]);
				tr.getBestMove(depth, MAX_TIME);
			}
		}
		else if (vec[0] == "quit")
		{
			tr.quit();
		}
	}
}
void UciTranslator::quit()
{
	exit(0);
}

void UciTranslator::test()
{

	
	return;
}

void UciTranslator::loadAlgebraicMoves(vector<string> algebraicMoves)
{
	for (int i = 0; i < algebraicMoves.size(); i++)
	{
		//cout << "Proccessing move: " << algebraicMoves[i] << endl;
		Move m = convertAlgebraicToMove(algebraicMoves[i]);
		MoveGeneration::makeMove(board, m);
		board.whoToMove = toggleColor(board.whoToMove);
	}
}

vector<string> UciTranslator::devideStringMoves(string s)
{
	istringstream iss(s);
	vector<string> div;
	string token;
	while (iss >> token) {
		div.push_back(token);
	}
	return div;
}

// algebric is a string in format a3b4 or a7b8q 
Move UciTranslator::convertAlgebraicToMove(string algebraic)
{
	Move m;
	int color = board.whoToMove;
	m.castleRights[0] = board.castleRights[color][0];
	m.castleRights[1] = board.castleRights[color][1];
	m.enPassant = board.onPassantField;

	int x1 = algebraic[0] - 'a';
	int y1 = algebraic[1] - '1';
	int x2 = algebraic[2] - 'a';
	int y2 = algebraic[3] - '1';

	Bitboard from = fields[x1 + y1 * 8];
	Bitboard to = fields[x2 + y2 * 8];

	int type1 = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (board.figure[j][i] & from)
			{
				type1 = i+ color * 6;
				color = j;
			}

		}
	}
	m.move = from | to;
	m.type_piece = type1;

	m.castleRights[0] = board.castleRights[color][0];
	m.castleRights[1] = board.castleRights[color][1];
	m.enPassant = board.onPassantField;

	if (type1%6==KING) // castle
	{
		if (from==E1 && to==C1)
		{
			m.move2 = A1|D1;
			m.type_piece2 = color * P + ROOK;
			return m;
		}
		else if (from==E1 && to == G1)
		{
			m.move2 = H1|F1;
			m.type_piece2 = color * P + ROOK;
			return m;
		}
		else if( from==E8 && to ==C8)
		{
			m.move2 = A8 | D8;
			m.type_piece2 = color * P + ROOK;
			return m;
		}
		else if( from==E8 && to ==G8)
		{
			m.move2 = H8 | F8;
			m.type_piece2 = color * P + ROOK;
			return m;
		}
		
	}

	// checking captures
	for (int i = 0; i < 6; i++)
	{
		if (board.figure[(color + 1) % 2][i] & to) // is there capture
		{
			m.move2 = to;
			m.type_piece2 = i+((color+1)%2)*6;

			return m;
		}
	}

	if (algebraic.length() == 5) // promotion
	{
		m.move = from;
		m.move3 = to;
		if (algebraic[4] == 'q')
			m.type_piece3 = QUEEN+color*6;
		if (algebraic[4] == 'k')
			m.type_piece3 = KNIGHT + color * 6;
		if (algebraic[4] == 'b')
			m.type_piece3 = BISHOP + color * 6;
		if (algebraic[4] == 'r')
			m.type_piece3 = ROOK + color * 6;
		return m;
	}

	// there was capture
	if (m.type_piece2 != 0)
		return m;

	// checking enPassant
	if (fields[board.onPassantField] == to && type1==PAWN && x1!=x2)
	{
		if (color == WHITE)
		{
			m.move2 = fields[(y2 - 1) * 8 + x2];
		}
		else
		{
			m.move2 = fields[(y2 + 1) * 8 + x2];
		}
		m.type_piece2 = PAWN+((color+1)%2)*6;
	}
	return m;
}

void UciTranslator::write_to_file(const std::string& file_name, const std::string& content) 
{
	std::ofstream file(file_name_log, std::ios::app); // Open file in append mode
	if (file.is_open()) {
		file << content<<"\n";
		//std::cout << "Content successfully appended to " << file_name << std::endl;
		file.close();
	}
	else {
		//std::cerr << "Unable to open file: " << file_name << std::endl;
	}
}

void UciTranslator::giveResponse(const std::string& content)
{
	cout << content<<endl;
	write_to_file(file_name_log,"bot: "+content);
}