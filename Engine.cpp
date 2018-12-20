#include "Engine.h"
#include "Helper.h"

// remove includes below this
#include <bitset>
#include <iostream>
#include <stack>

namespace NATHAN {
	Engine::Engine() {
		current_board = new Board();
	}

	Engine::Engine(Board* b) {
		current_board = new Board(b);
	}

	Engine::~Engine() {
		//delete current_board;
	}
	
	uint64_t Engine::generate_slide_movesHV(uint64_t slide_position) {
		uint64_t current_piece = ((uint64_t)1 << slide_position);
		uint64_t horizontal = (current_board->getAllPieces() - 2 * current_piece) ^ Helper::reverse(Helper::reverse(current_board->getAllPieces()) - 2 * Helper::reverse(current_piece));
		uint64_t vertical = ((current_board->getAllPieces() & Helper::files[slide_position % 8]) - 2 * current_piece) ^ Helper::reverse(Helper::reverse(current_board->getAllPieces() & Helper::files[slide_position % 8]) - 2 * Helper::reverse(current_piece));
		return (horizontal & Helper::rows[slide_position / 8]) | (vertical & Helper::files[slide_position % 8]);
	}

	uint64_t Engine::generate_slide_movesDandAD(uint64_t slide_position) {
		uint64_t current_piece = ((uint64_t)1 << slide_position);
		uint64_t diagonal = ((current_board->getAllPieces() & Helper::diagonal[slide_position / 8 + slide_position % 8]) - 2 * current_piece) ^ Helper::reverse(Helper::reverse(current_board->getAllPieces() & Helper::diagonal[slide_position / 8 + slide_position % 8]) - 2 * Helper::reverse(current_piece));
		uint64_t anti_diagonal = ((current_board->getAllPieces() & Helper::antidiagonal[slide_position / 8 + 7 - slide_position % 8]) - 2 * current_piece) ^ Helper::reverse(Helper::reverse(current_board->getAllPieces() & Helper::antidiagonal[slide_position / 8 + 7 - slide_position % 8]) - 2 * Helper::reverse(current_piece));
		return (diagonal & Helper::diagonal[slide_position / 8 + slide_position % 8]) | (anti_diagonal & Helper::antidiagonal[slide_position / 8 + 7 - slide_position % 8]);
	}

	uint64_t Engine::unsafe_for_white() {
		uint64_t unsafe = 0;
		
		// pawns
		unsafe = ((current_board->get_pawns_black() >> 7) & ~Helper::files[0]);
		unsafe |= ((current_board->get_pawns_black() >> 9) & ~Helper::files[7]);

		// knights
		uint64_t knights = current_board->get_knights_black();

		while(knights) {
			uint64_t lsb_current_knight = Helper::lsb(knights);
			unsafe |= Helper::knight_moves[63 - lsb_current_knight];
			knights ^= ((uint64_t)1 << lsb_current_knight);
		}

		// bishops and queen diagonals
		uint64_t bishops_queens = current_board->get_bishops_black() | current_board->get_queens_black();
		while(bishops_queens) {
			uint64_t lsb_current_piece = Helper::lsb(bishops_queens);
			unsafe |= generate_slide_movesDandAD(lsb_current_piece);
			bishops_queens ^= ((uint64_t)1 << lsb_current_piece);
		}

		// rook and queen straights
		uint64_t rooks_queens = current_board->get_rooks_black() | current_board->get_queens_black();
		while(rooks_queens) {
			uint64_t lsb_current_piece = Helper::lsb(rooks_queens);
			unsafe |= generate_slide_movesHV(lsb_current_piece);
			rooks_queens ^= ((uint64_t)1 << lsb_current_piece);
		}

		// king
		unsafe |= Helper::king_moves[63 - Helper::lsb(current_board->get_king_black())];

		return unsafe;
	}

	uint64_t Engine::unsafe_for_black() {
		uint64_t unsafe = 0;
		
		// pawns
		unsafe = ((current_board->get_pawns_white() << 7) & ~Helper::files[7]);
		unsafe |= ((current_board->get_pawns_white() << 9) & ~Helper::files[0]);

		// knights
		uint64_t knights = current_board->get_knights_white();

		while(knights) {
			uint64_t lsb_current_knight = Helper::lsb(knights);
			unsafe |= Helper::knight_moves[63 - lsb_current_knight];
			knights ^= ((uint64_t)1 << lsb_current_knight);
		}

		// bishops and queen diagonals
		uint64_t bishops_queens = current_board->get_bishops_white() | current_board->get_queens_white();
		while(bishops_queens) {
			uint64_t lsb_current_piece = Helper::lsb(bishops_queens);
			unsafe |= generate_slide_movesDandAD(lsb_current_piece);
			bishops_queens ^= ((uint64_t)1 << lsb_current_piece);
		}

		// rook and queen straights
		uint64_t rooks_queens = current_board->get_rooks_white() | current_board->get_queens_white();
		while(rooks_queens) {
			uint64_t lsb_current_piece = Helper::lsb(rooks_queens);
			unsafe |= generate_slide_movesHV(lsb_current_piece);
			rooks_queens ^= ((uint64_t)1 << lsb_current_piece);
		}

		// king
		unsafe |= Helper::king_moves[63 - Helper::lsb(current_board->get_king_white())];

		return unsafe;
	}

	std::vector<Board>* Engine::generate_knight_moves_white() {
		std::vector<Board>* moves = new std::vector<Board>();

		uint64_t knights = current_board->get_knights_white();

		while (knights) {
			uint64_t lsb_current_knight = Helper::lsb(knights);

			uint64_t possible_moves = Helper::knight_moves[63 - lsb_current_knight];

			while (possible_moves) {
				uint64_t lsb_possible_moves = Helper::lsb(possible_moves);
				uint64_t try_move = (uint64_t)1 << lsb_possible_moves;
				if (!(try_move & current_board->getWhitePieces())) {
					Board* new_board = new Board(current_board);
					uint64_t new_knights = current_board->get_knights_white() ^ ((uint64_t)1 << lsb_current_knight);
					new_knights |= try_move;
					new_board->set_white_knights(new_knights);
					new_board->determine_white_capture(try_move);
					new_board->clear_en_passant();
					new_board->increase_ply();
					Engine e = Engine(new_board);
					if(!(e.unsafe_for_white() & new_board->get_king_white())) {
						moves->push_back(new_board);
					}
				}
				possible_moves ^= ((uint64_t)1 << lsb_possible_moves);
			}

			knights ^= ((uint64_t)1 << lsb_current_knight);
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_knight_moves_black() {
		std::vector<Board>* moves = new std::vector<Board>();

		uint64_t knights = current_board->get_knights_black();

		while (knights) {
			uint64_t lsb_current_knight = Helper::lsb(knights);

			uint64_t possible_moves = Helper::knight_moves[63 - lsb_current_knight];

			while (possible_moves) {
				uint64_t lsb_possible_moves = Helper::lsb(possible_moves);
				uint64_t try_move = (uint64_t)1 << lsb_possible_moves;
				if (!(try_move & current_board->getBlackPieces())) {
					Board* new_board = new Board(current_board);
					uint64_t new_knights = current_board->get_knights_black() ^ ((uint64_t)1 << lsb_current_knight);
					new_knights |= try_move;
					new_board->set_black_knights(new_knights);
					new_board->determine_black_capture(try_move);
					new_board->clear_en_passant();
					new_board->increase_ply();
					Engine e = Engine(new_board);
					if(!(e.unsafe_for_black() & new_board->get_king_black())) {
						moves->push_back(new_board);
					}
				}
				possible_moves ^= ((uint64_t)1 << lsb_possible_moves);
			}

			knights ^= ((uint64_t)1 << lsb_current_knight);
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_rook_moves_white() {
		std::vector<Board>* moves = new std::vector<Board>();

		uint64_t rooks = current_board->get_rooks_white();

		while(rooks) {
			uint64_t slide_position = Helper::lsb(rooks);
			uint64_t current_rook = ((uint64_t)1 << slide_position);
			uint64_t new_rooks = generate_slide_movesHV(slide_position) & ~(current_board->getWhitePieces());
			if(new_rooks){
				while(new_rooks) {
					uint64_t lsb_new_rooks = Helper::lsb(new_rooks);
					uint64_t try_move = (uint64_t)1 << lsb_new_rooks;
					uint64_t really_new_rooks = current_board->get_rooks_white() ^ current_rook;
					really_new_rooks |= try_move;
					Board* new_board = new Board(current_board);
					new_board->set_white_rooks(really_new_rooks);
					new_board->determine_white_capture(try_move);
					new_board->clear_en_passant();
					new_board->increase_ply();
					Engine e = Engine(new_board);
					if(!(e.unsafe_for_white() & new_board->get_king_white())) {
						moves->push_back(new_board);
					}
					new_rooks ^= try_move;
				}
			}
			rooks ^= current_rook;
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_rook_moves_black() {
		std::vector<Board>* moves = new std::vector<Board>();

		uint64_t rooks = current_board->get_rooks_black();

		while(rooks) {
			uint64_t slide_position = Helper::lsb(rooks);
			uint64_t current_rook = ((uint64_t)1 << slide_position);
			uint64_t new_rooks = generate_slide_movesHV(slide_position) & ~(current_board->getBlackPieces());
			if(new_rooks){
				while(new_rooks) {
					uint64_t lsb_new_rooks = Helper::lsb(new_rooks);
					uint64_t try_move = (uint64_t)1 << lsb_new_rooks;
					uint64_t really_new_rooks = current_board->get_rooks_black() ^ current_rook;
					really_new_rooks |= try_move;
					Board* new_board = new Board(current_board);
					new_board->set_black_rooks(really_new_rooks);
					new_board->determine_black_capture(try_move);
					new_board->clear_en_passant();
					new_board->increase_ply();
					Engine e = Engine(new_board);
					if(!(e.unsafe_for_black() & new_board->get_king_black())) {
						moves->push_back(new_board);
					}
					new_rooks ^= try_move;
				}
			}
			rooks ^= current_rook;
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_bishop_moves_white() {
		std::vector<Board>* moves = new std::vector<Board>();

		uint64_t bishops = current_board->get_bishops_white();

		while(bishops) {
			uint64_t slide_position = Helper::lsb(bishops);
			uint64_t current_bishop = ((uint64_t)1 << slide_position);
			uint64_t new_bishops = generate_slide_movesDandAD(slide_position) & ~(current_board->getWhitePieces());
			if(new_bishops){
				while(new_bishops) {
					uint64_t lsb_new_bishops = Helper::lsb(new_bishops);
					uint64_t try_move = (uint64_t)1 << lsb_new_bishops;
					uint64_t really_new_bishops = current_board->get_bishops_white() ^ current_bishop;
					really_new_bishops |= try_move;
					Board* new_board = new Board(current_board);
					new_board->set_white_bishops(really_new_bishops);
					new_board->determine_white_capture(try_move);
					new_board->clear_en_passant();
					new_board->increase_ply();
					Engine e = Engine(new_board);
					if(!(e.unsafe_for_white() & new_board->get_king_white())) {
						moves->push_back(new_board);
					}
					new_bishops ^= try_move;
				}
			}
			bishops ^= current_bishop;
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_bishop_moves_black() {
		std::vector<Board>* moves = new std::vector<Board>();

		uint64_t bishops = current_board->get_bishops_black();

		while(bishops) {
			uint64_t slide_position = Helper::lsb(bishops);
			uint64_t current_bishop = ((uint64_t)1 << slide_position);
			uint64_t new_bishops = generate_slide_movesDandAD(slide_position) & ~(current_board->getBlackPieces());
			if(new_bishops){
				while(new_bishops) {
					uint64_t lsb_new_bishops = Helper::lsb(new_bishops);
					uint64_t try_move = (uint64_t)1 << lsb_new_bishops;
					uint64_t really_new_bishops = current_board->get_bishops_black() ^ current_bishop;
					really_new_bishops |= try_move;
					Board* new_board = new Board(current_board);
					new_board->set_black_bishops(really_new_bishops);
					new_board->determine_black_capture(try_move);
					new_board->clear_en_passant();
					new_board->increase_ply();
					Engine e = Engine(new_board);
					if(!(e.unsafe_for_black() & new_board->get_king_black())) {
						moves->push_back(new_board);
					}
					new_bishops ^= try_move;
				}
			}
			bishops ^= current_bishop;
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_queen_moves_white() {
		std::vector<Board>* moves = new std::vector<Board>();

		uint64_t queens = current_board->get_queens_white();

		while(queens) {
			uint64_t slide_position = Helper::lsb(queens);
			uint64_t current_queen = ((uint64_t)1 << slide_position);
			uint64_t new_queens = (generate_slide_movesDandAD(slide_position) | generate_slide_movesHV(slide_position)) & ~(current_board->getWhitePieces());
			if(new_queens){
				while(new_queens) {
					uint64_t lsb_new_queens = Helper::lsb(new_queens);
					uint64_t try_move = (uint64_t)1 << lsb_new_queens;
					uint64_t really_new_queens = current_board->get_queens_white() ^ current_queen;
					really_new_queens |= try_move;
					Board* new_board = new Board(current_board);
					new_board->set_white_queens(really_new_queens);
					new_board->determine_white_capture(try_move);
					new_board->clear_en_passant();
					new_board->increase_ply();
					Engine e = Engine(new_board);
					if(!(e.unsafe_for_white() & new_board->get_king_white())) {
						moves->push_back(new_board);
					}
					new_queens ^= try_move;
				}
			}
			queens ^= current_queen;
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_queen_moves_black() {
		std::vector<Board>* moves = new std::vector<Board>();

		uint64_t queens = current_board->get_queens_black();

		while(queens) {
			uint64_t slide_position = Helper::lsb(queens);
			uint64_t current_queen = ((uint64_t)1 << slide_position);
			uint64_t new_queens = (generate_slide_movesDandAD(slide_position) | generate_slide_movesHV(slide_position)) & ~(current_board->getBlackPieces());
			if(new_queens){
				while(new_queens) {
					uint64_t lsb_new_queens = Helper::lsb(new_queens);
					uint64_t try_move = (uint64_t)1 << lsb_new_queens;
					uint64_t really_new_queens = current_board->get_queens_black() ^ current_queen;
					really_new_queens |= try_move;
					Board* new_board = new Board(current_board);
					new_board->set_black_queens(really_new_queens);
					new_board->determine_black_capture(try_move);
					new_board->clear_en_passant();
					new_board->increase_ply();
					Engine e = Engine(new_board);
					if(!(e.unsafe_for_black() & new_board->get_king_black())) {
						moves->push_back(new_board);
					}
					new_queens ^= try_move;
				}
			}
			queens ^= current_queen;
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_king_moves_white() {
		std::vector<Board>* moves = new std::vector<Board>();
		uint64_t unsafe = unsafe_for_white();

		uint64_t kings = current_board->get_king_white();

		uint64_t lsb_current_king = Helper::lsb(kings);

		uint64_t possible_moves = Helper::king_moves[63 - lsb_current_king] & ~unsafe;

		while (possible_moves) {
			uint64_t lsb_possible_moves = Helper::lsb(possible_moves);
			uint64_t try_move = (uint64_t)1 << lsb_possible_moves;
			if (!(try_move & current_board->getWhitePieces())) {
				Board* new_board = new Board(current_board);
				uint64_t new_kings = current_board->get_king_white() ^ ((uint64_t)1 << lsb_current_king);
				new_kings |= try_move;
				new_board->set_white_king(new_kings);
				new_board->determine_white_capture(try_move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_white() & new_board->get_king_white())) {
					moves->push_back(new_board);
				}
			}
			possible_moves ^= ((uint64_t)1 << lsb_possible_moves);
		}

		//castling
		// king side castle
		if(current_board->white_king_castle() && (unsafe & 0b1110) == 0) {
			Board* new_board = new Board(current_board);
			uint64_t new_rooks = (current_board->get_rooks_white() ^ 1) | 4; // hard coded, it's the only possibility
			new_board->set_white_rooks(new_rooks);
			new_board->set_white_king(2); // hard coded, it's the only possibility
			new_board->clear_en_passant();
			new_board->increase_ply();
			moves->push_back(new_board);
		}

		// queen side castle
		if(current_board->white_queen_castle() && (unsafe & 0b111000) == 0) {
			Board* new_board = new Board(current_board);
			uint64_t new_rooks = (current_board->get_rooks_white() ^ 128) | 16; // hard coded, it's the only possibility
			new_board->set_white_rooks(new_rooks);
			new_board->set_white_king(32); // hard coded, it's the only possibility
			new_board->clear_en_passant();
			new_board->increase_ply();
			moves->push_back(new_board);
		}

		/*#ifndef PERF
		if(current_board->white_king_castle()) {
			Board* new_board = new Board(current_board);
		}
		#else
			std::cout << "Perf defined"	 << std::endl;
		#endif*/

		return moves;
	}

	std::vector<Board>* Engine::generate_king_moves_black() {
		std::vector<Board>* moves = new std::vector<Board>();
		uint64_t unsafe = unsafe_for_black();

		uint64_t kings = current_board->get_king_black();

		uint64_t lsb_current_king = Helper::lsb(kings);

		uint64_t possible_moves = Helper::king_moves[63 - lsb_current_king] & ~unsafe;

		while (possible_moves) {
			uint64_t lsb_possible_moves = Helper::lsb(possible_moves);
			uint64_t try_move = (uint64_t)1 << lsb_possible_moves;
			if (!(try_move & current_board->getBlackPieces())) {
				Board* new_board = new Board(current_board);
				uint64_t new_kings = current_board->get_king_black() ^ ((uint64_t)1 << lsb_current_king);
				new_kings |= try_move;
				new_board->set_black_king(new_kings);
				new_board->determine_black_capture(try_move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_black() & new_board->get_king_black())) {
					moves->push_back(new_board);
				}
			}
			possible_moves ^= ((uint64_t)1 << lsb_possible_moves);
		}

		//castling
		// king side castle
		if(current_board->black_king_castle() && (unsafe & ((uint64_t)0b1110 << 56)) == 0) {
			Board* new_board = new Board(current_board);
			uint64_t new_rooks = (current_board->get_rooks_black() ^ ((uint64_t)1 << 56)) | ((uint64_t)4 << 56); // hard coded, it's the only possibility
			new_board->set_black_rooks(new_rooks);
			new_board->set_black_king((uint64_t)2 << 56); // hard coded, it's the only possibility
			new_board->clear_en_passant();
			new_board->increase_ply();
			moves->push_back(new_board);
		}

		// queen side castle
		if(current_board->black_queen_castle() && (unsafe & ((uint64_t)0b111000 << 56)) == 0) {
			Board* new_board = new Board(current_board);
			uint64_t new_rooks = (current_board->get_rooks_black() ^ ((uint64_t)128 << 56)) | ((uint64_t)16 << 56); // hard coded, it's the only possibility
			new_board->set_black_rooks(new_rooks);
			new_board->set_black_king((uint64_t)32 << 56); // hard coded, it's the only possibility
			new_board->clear_en_passant();
			new_board->increase_ply();
			moves->push_back(new_board);
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_pawn_moves_white() {
		std::vector<Board>* moves = new std::vector<Board>();
		uint64_t black_pieces = current_board->getBlackPieces();
		uint64_t white_pieces = current_board->getWhitePieces();

		uint64_t pawns = current_board->get_pawns_white() & ~Helper::rows[6];
		while(pawns) {
			uint64_t current_pawn = ((uint64_t)1 << Helper::lsb(pawns));
			uint64_t new_pawns = (current_pawn << 8);
			new_pawns &= ~white_pieces;
			new_pawns &= ~black_pieces;
			new_pawns |= (((current_pawn << 7) & ~Helper::files[7]) & black_pieces);
			new_pawns |= (((current_pawn << 9) & ~Helper::files[0]) & black_pieces);
			
			while(new_pawns) {
				uint64_t lsb_new_pawn = Helper::lsb(new_pawns);
				uint64_t move = ((uint64_t)1 << lsb_new_pawn);
				Board* new_board = new Board(current_board);
				uint64_t all_white_pawns = current_board->get_pawns_white() ^ current_pawn;
				all_white_pawns |= move;
				new_board->set_white_pawns(all_white_pawns);
				new_board->determine_white_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_white() & new_board->get_king_white())) {
					moves->push_back(new_board);
				}
				new_pawns ^= move;
			}

			
			// en passant
			new_pawns |= ((((current_pawn & ~Helper::files[7]) << 9) & current_board->get_en_passant_white() << 8) | (((current_pawn & ~Helper::files[0]) << 7) & current_board->get_en_passant_white() << 8));
			if(new_pawns) {
				Board* new_board = new Board(current_board);
				new_board->set_black_pawns(new_board->get_pawns_black() ^ current_board->get_en_passant_white());
				new_board->set_white_pawns((new_board->get_pawns_white() ^ current_pawn) | new_pawns);
				new_board->clear_en_passant();
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_white() & new_board->get_king_white())) {
					moves->push_back(new_board);
				}
			}

			pawns ^= current_pawn;
		}

		// rank 2
		pawns = current_board->get_pawns_white() & Helper::rows[1];
		while(pawns) {
			uint64_t current_pawn = ((uint64_t)1 << Helper::lsb(pawns));
			uint64_t new_pawn = (current_pawn << 8) & ~current_board->getAllPieces();
			new_pawn = (new_pawn << 8) & ~current_board->getAllPieces();
			if(new_pawn) {
				uint64_t lsb_new_pawn = Helper::lsb(new_pawn);
				uint64_t move = ((uint64_t)1 << lsb_new_pawn);
				Board* new_board = new Board(current_board);
				uint64_t all_white_pawns = current_board->get_pawns_white() ^ current_pawn;
				all_white_pawns |= move;
				new_board->set_white_pawns(all_white_pawns);
				new_board->clear_en_passant();
				new_board->set_special_bit(lsb_new_pawn);
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_white() & new_board->get_king_white())) {
					moves->push_back(new_board);
				}
			}

			pawns ^= current_pawn;
		}

		// rank 7
		pawns = current_board->get_pawns_white() & Helper::rows[6];
		while(pawns) {
			uint64_t current_pawn = ((uint64_t)1 << Helper::lsb(pawns));
			uint64_t new_piece = current_pawn << 8;
			new_piece &= ~white_pieces;
			new_piece &= ~black_pieces;
			new_piece |= (((current_pawn << 7) & ~Helper::files[7]) & black_pieces);
			new_piece |= (((current_pawn << 9) & ~Helper::files[0]) & black_pieces);

			while(new_piece) {
				uint64_t lsb_new_piece = Helper::lsb(new_piece);
				uint64_t move = ((uint64_t)1 << lsb_new_piece);
				// queen
				Board* new_board = new Board(current_board);
				uint64_t all_white_pawns = current_board->get_pawns_white() ^ current_pawn;
				new_board->set_white_pawns(all_white_pawns);
				new_board->set_white_queens(current_board->get_queens_white() | move);
				new_board->determine_white_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_white() & new_board->get_king_white())) {
					moves->push_back(new_board);
				}
				else{
					new_piece ^= move;
					break;
				}
				// rook
				new_board = new Board(current_board);
				new_board->set_white_pawns(all_white_pawns);
				new_board->set_white_rooks(current_board->get_rooks_white() | move);
				new_board->determine_white_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				moves->push_back(new_board);
				// knight
				new_board = new Board(current_board);
				new_board->set_white_pawns(all_white_pawns);
				new_board->set_white_knights(current_board->get_knights_white() | move);
				new_board->determine_white_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				moves->push_back(new_board);
				// bishop
				new_board = new Board(current_board);
				new_board->set_white_pawns(all_white_pawns);
				new_board->set_white_bishops(current_board->get_bishops_white() | move);
				new_board->determine_white_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				moves->push_back(new_board);

				new_piece ^= move;
			}
			pawns ^= current_pawn;
		}

		return moves;
	}

	// not working

std::vector<Board>* Engine::generate_pawn_moves_black() {
		std::vector<Board>* moves = new std::vector<Board>();
		uint64_t white_pieces = current_board->getWhitePieces();
		uint64_t black_pieces = current_board->getBlackPieces();

		uint64_t pawns = current_board->get_pawns_black() & ~Helper::rows[1];
		while(pawns) {
			uint64_t current_pawn = ((uint64_t)1 << Helper::lsb(pawns));
			uint64_t new_pawns = (current_pawn >> 8);
			new_pawns &= ~black_pieces;
			new_pawns &= ~white_pieces;
			new_pawns |= (((current_pawn >> 7) & ~Helper::files[0]) & white_pieces);
			new_pawns |= (((current_pawn >> 9) & ~Helper::files[7]) & white_pieces);
			
			while(new_pawns) {
				uint64_t lsb_new_pawn = Helper::lsb(new_pawns);
				uint64_t move = ((uint64_t)1 << lsb_new_pawn);
				Board* new_board = new Board(current_board);
				uint64_t all_black_pawns = current_board->get_pawns_black() ^ current_pawn;
				all_black_pawns |= move;
				new_board->set_black_pawns(all_black_pawns);
				new_board->determine_black_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_black() & new_board->get_king_black())) {
					moves->push_back(new_board);
				}
				new_pawns ^= move;
			}

			
			// en passant
			new_pawns |= ((((current_pawn & ~Helper::files[0]) >> 9) & current_board->get_en_passant_black() >> 8) | (((current_pawn & ~Helper::files[7]) >> 7) & current_board->get_en_passant_black() >> 8));
			if(new_pawns) {
				Board* new_board = new Board(current_board);
				new_board->set_white_pawns(new_board->get_pawns_white() ^ current_board->get_en_passant_black());
				new_board->set_black_pawns((new_board->get_pawns_black() ^ current_pawn) | new_pawns);
				new_board->clear_en_passant();
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_black() & new_board->get_king_black())) {
					moves->push_back(new_board);
				}
			}

			pawns ^= current_pawn;
		}

		// rank 7
		pawns = current_board->get_pawns_black() & Helper::rows[6];
		while(pawns) {
			uint64_t current_pawn = ((uint64_t)1 << Helper::lsb(pawns));
			uint64_t new_pawn = (current_pawn >> 8) & ~current_board->getAllPieces();
			new_pawn = (new_pawn >> 8) & ~current_board->getAllPieces();
			if(new_pawn) {
				uint64_t lsb_new_pawn = Helper::lsb(new_pawn);
				uint64_t move = ((uint64_t)1 << lsb_new_pawn);
				Board* new_board = new Board(current_board);
				uint64_t all_black_pawns = current_board->get_pawns_black() ^ current_pawn;
				all_black_pawns |= move;
				new_board->set_black_pawns(all_black_pawns);
				new_board->clear_en_passant();
				new_board->set_special_bit(lsb_new_pawn);
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_black() & new_board->get_king_black())) {
					moves->push_back(new_board);
				}
			}

			pawns ^= current_pawn;
		}

		// rank 2
		pawns = current_board->get_pawns_black() & Helper::rows[1];
		while(pawns) {
			uint64_t current_pawn = ((uint64_t)1 << Helper::lsb(pawns));
			uint64_t new_piece = current_pawn >> 8;
			new_piece &= ~black_pieces;
			new_piece &= ~white_pieces;
			new_piece |= (((current_pawn >> 7) & ~Helper::files[0]) & white_pieces);
			new_piece |= (((current_pawn >> 9) & ~Helper::files[7]) & white_pieces);

			while(new_piece) {
				uint64_t lsb_new_piece = Helper::lsb(new_piece);
				uint64_t move = ((uint64_t)1 << lsb_new_piece);
				// queen
				Board* new_board = new Board(current_board);
				uint64_t all_black_pawns = current_board->get_pawns_black() ^ current_pawn;
				new_board->set_black_pawns(all_black_pawns);
				new_board->set_black_queens(current_board->get_queens_black() | move);
				new_board->determine_black_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				Engine e = Engine(new_board);
				if(!(e.unsafe_for_black() & new_board->get_king_black())) {
					moves->push_back(new_board);
				}
				else{
					new_piece ^= move;
					break;
				}
				// rook
				new_board = new Board(current_board);
				new_board->set_black_pawns(all_black_pawns);
				new_board->set_black_rooks(current_board->get_rooks_black() | move);
				new_board->determine_black_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				moves->push_back(new_board);
				// knight
				new_board = new Board(current_board);
				new_board->set_black_pawns(all_black_pawns);
				new_board->set_black_knights(current_board->get_knights_black() | move);
				new_board->determine_black_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				moves->push_back(new_board);
				// bishop
				new_board = new Board(current_board);
				new_board->set_black_pawns(all_black_pawns);
				new_board->set_black_bishops(current_board->get_bishops_black() | move);
				new_board->determine_black_capture(move);
				new_board->clear_en_passant();
				new_board->increase_ply();
				moves->push_back(new_board);

				new_piece ^= move;
			}
			pawns ^= current_pawn;
		}

		return moves;
	}

	std::vector<Board>* Engine::generate_white_moves() {
		std::vector<Board>* moves = new std::vector<Board>();

		std::vector<Board>* intermediate = generate_pawn_moves_white();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_rook_moves_white();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_knight_moves_white();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_bishop_moves_white();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_queen_moves_white();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_king_moves_white();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;

		return moves;
	}

	std::vector<Board>* Engine::generate_black_moves() {
		std::vector<Board>* moves = new std::vector<Board>();

		std::vector<Board>* intermediate = generate_pawn_moves_black();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_rook_moves_black();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_knight_moves_black();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_bishop_moves_black();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_queen_moves_black();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;
		intermediate = generate_king_moves_black();
		moves->insert(moves->end(), intermediate->begin(), intermediate->end());
		delete intermediate;

		return moves;
	}

	uint64_t Engine::perft(uint64_t depth) {
		uint64_t nodes_searched = 0; // don't count the first node
		uint64_t max = 0;

		std::stack<Board>* boards = new std::stack<Board>();
		boards->push(current_board);
		while(boards->size()) {
			current_board = &boards->top();
			boards->pop();
			if(current_board->get_ply() == (depth - 1)) {
				if(current_board->whites_move()) {
					std::vector<Board>* moves = generate_white_moves();
					nodes_searched += moves->size();
					delete moves;

				}
				else {
					std::vector<Board>* moves = generate_black_moves();
					nodes_searched += moves->size();
					delete moves;
				}
			}
			else {
				std::vector<Board>* intermediate;
				if(current_board->whites_move()) {
					intermediate = generate_white_moves();
				}
				else{
					intermediate = generate_black_moves();
				}
				while(!intermediate->empty()) {
					boards->push(intermediate->back());
					intermediate->pop_back();
				}
				delete intermediate;
				/*if(boards->size() > max) {
					max = boards->size();
					std::cout << max << std::endl;
				}*/
			}
			//delete current_board;
		}

		//current_board = new Board();

		delete boards;

		return nodes_searched;
	}

	Board* Engine::get_board() {
		return current_board;
	}
}