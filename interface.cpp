#include <iostream>

#include <bitset>
#include <vector>

#include <cstdlib>
#include <ctime>
#include <chrono>

#include "Helper.h"
#include "Board.h"
#include "Engine.h"

using namespace NATHAN;

int main()
{

	char board[8][8] = {
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','p','p','p','p'},
                {' ',' ',' ',' ',' ',' ',' ',' '},
                {' ',' ',' ',' ',' ',' ',' ',' '},
                {' ',' ',' ',' ',' ',' ',' ',' '},
                {' ',' ',' ',' ',' ',' ',' ',' '},
                {'P','P','P','P','P','P','P','P'},
                {'R','N','B','Q','K','B','N','R'}};

	char board2[8][8] = {
				{'r',' ',' ',' ','k',' ',' ','r'},
                {'p',' ','p','p','q','p','b',' '},
                {'b','n',' ',' ','p','n','p',' '},
                {' ',' ',' ','P','N',' ',' ',' '},
                {' ','p',' ',' ','P',' ',' ',' '},
                {' ',' ','N',' ',' ','Q',' ','p'},
                {'P','P','P','B','B','P','P','P'},
                {'R',' ',' ',' ','K',' ',' ','R'}};

	char board3[8][8] = {
				{' ',' ',' ',' ',' ',' ',' ',' '},
                {' ',' ',' ',' ',' ',' ',' ',' '},
                {' ',' ',' ',' ',' ',' ',' ',' '},
                {'k','p',' ',' ',' ',' ',' ','R'},
                {' ',' ',' ',' ',' ',' ',' ','K'},
                {' ',' ',' ',' ',' ',' ',' ',' '},
                {' ',' ',' ',' ',' ',' ',' ',' '},
                {' ',' ',' ',' ',' ',' ',' ',' '}};
	Board* b = new Board(board);
	/*for(int i = 24; i < 40; i++) {
		b->set_special_bit((uint64_t)1 << i);
	}*/
	//b->unset_special_bit(59);


	std::cout << "Starting position:" << std::endl;
	std::cout << *b << std::endl;

//	Engine* e = new Engine(b);

	/*std::vector<Board*>* knight_moves = e->generate_knight_moves_black();

	for(unsigned int i = 0; i < knight_moves->size(); i++) {
		std::cout << *knight_moves->at(i) << std::endl;
		std::cout << knight_moves->at(i)->get_ply() << std::endl;
	}

	std::vector<Board*>* rook_moves = e->generate_rook_moves_black();

	std::cout << rook_moves->size() << std::endl;

	for(unsigned int i = 0; i < rook_moves->size(); i++) {
		std::cout << *rook_moves->at(i) << std::endl;
	}

	std::vector<Board*>* bishop_moves = e->generate_bishop_moves_black();

	for(unsigned int i = 0; i < bishop_moves->size(); i++) {
		std::cout << *bishop_moves->at(i) << std::endl;
	}

	std::cout << bishop_moves->size() << std::endl;

	std::vector<Board*>* queen_moves = e->generate_queen_moves_black();

	for(unsigned int i = 0; i < queen_moves->size(); i++) {
		std::cout << *queen_moves->at(i) << std::endl;
	}

	std::cout << queen_moves->size() << std::endl;

	std::vector<Board*>* king_moves = e->generate_king_moves_black();

	for(unsigned int i = 0; i < king_moves->size(); i++) {
		std::cout << *king_moves->at(i) << std::endl;
	}

	std::cout << king_moves->size() << std::endl;

	std::vector<Board*>* pawn_moves = e->generate_pawn_moves_black();

	for(unsigned int i = 0; i < pawn_moves->size(); i++) {
		std::cout << *pawn_moves->at(i) << std::endl;
	}

	std::cout << pawn_moves->size() << std::endl;

	std::vector<Board*>* white_moves = e->generate_white_moves();

	for(unsigned int i = 0; i < white_moves->size(); i++) {
		std::cout << *white_moves->at(i) << std::endl;
	}

	std::cout << "White moves: " << white_moves->size() << std::endl;

	std::vector<Board*>* black_moves = e->generate_black_moves();

	for(unsigned int i = 0; i < black_moves->size(); i++) {
		std::cout << *black_moves->at(i) << std::endl;
	}

	std:: cout << "Black moves: " << black_moves->size() << std::endl;*/

	/*std::vector<Engine*>* engines = new std::vector<Engine*>();
	int random;
	srand(time(NULL));
	
	for(int k = 0; k < 100000; k++) {
		char board[8][8];
		for(int i = 0; i < 8; i++) {
			for(int j = 0; j < 8; j++) {
				board[i][j] = ' ';
			}
		}
		board[0][4] = 'k';
		board[7][4] = 'K';
		for(int i = 2; i < 6; i++) {
			for(int j = 0; j < 8; j++) {
				random = rand() % 100;
				switch(random) {
					case 0:
						board[i][j] = 'P';
						break;
					case 1:
						board[i][j] = 'R';
						break;
					case 2:
						board[i][j] = 'N';
						break;
					case 3:
						board[i][j] = 'B';
						break;
					case 4:
						board[i][j] = 'Q';
						break;
				}
			}
		}
		engines->push_back(new Engine(new Board(board)));
	}

	uint64_t calculated = 0;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	for(unsigned int i = 0; i < engines->size(); i++) {
		engines->at(i)->generate_white_moves();
		//calculated += engines->at(i)->generate_white_moves()->size();
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Taken: " <<  std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() << std::endl;

	for(unsigned int i = 0; i < engines->size(); i++){
		std::cout << *engines->at(i)->get_board() << std::endl;
		std::cout << engines->at(i)->generate_white_moves()->size() << std::endl;
	}*/

	

	for(int i = 1; i < 7; i++) {
		Engine* e = new Engine(b);
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		uint64_t perft = e->perft(i);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "Perft: " << i << "\t" << perft << "\t" << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() << std::endl;
		delete e;
	}

	delete b;

	/*std::vector<Board*>* moves = e->generate_knight_moves_white();
	

	for(unsigned int i = 0; i < moves->size(); i++) {
		std::cout << *moves->at(i) << std::endl;
	}

	std::cout << moves->size() << std::endl;*/

	return 0;
}