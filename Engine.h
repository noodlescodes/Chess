#pragma once

#include "Board.h"

#include <vector>
#include <memory>

namespace NATHAN {
	class Engine {
	public:
		Engine();
		Engine(Board b);
		~Engine();

		std::vector<Board> generate_knight_moves_white();
		std::vector<Board> generate_knight_moves_black();
		std::vector<Board> generate_rook_moves_white();
		std::vector<Board> generate_rook_moves_black();
		std::vector<Board> generate_bishop_moves_white();
		std::vector<Board> generate_bishop_moves_black();
		std::vector<Board> generate_queen_moves_white();
		std::vector<Board> generate_queen_moves_black();
		std::vector<Board> generate_king_moves_white();
		std::vector<Board> generate_king_moves_black();
		std::vector<Board> generate_pawn_moves_white();
		std::vector<Board> generate_pawn_moves_black();

		std::vector<Board> generate_white_moves();
		std::vector<Board> generate_black_moves();

		uint64_t perft(uint64_t depth);

		Board get_board();

		//uint64_t unsafe_for_white();

	private:
		Board current_board;
		uint64_t generate_slide_movesHV(uint64_t slide_position);
		uint64_t generate_slide_movesDandAD(uint64_t slide_position);

		uint64_t unsafe_for_white();
		uint64_t unsafe_for_black();
	};
}