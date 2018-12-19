#pragma once

#include <string>
#include <ostream>

namespace NATHAN {
	class Board {
	public:
		Board();
		Board(const Board* b);
		Board(std::string fen);
		Board(char board[8][8]);
		~Board();

		uint64_t getWhitePieces();
		uint64_t getBlackPieces();
		uint64_t getAllPieces();
		bool white_king_castle();
		bool white_queen_castle();
		bool black_king_castle();
		bool black_queen_castle();
		int moves_50();
		bool whites_move();
		void increase_ply();
		uint64_t get_ply();

		void set_white_pawns(uint64_t n);
		void set_black_pawns(uint64_t n);
		void set_white_rooks(uint64_t n);
		void set_black_rooks(uint64_t n);
		void set_white_knights(uint64_t n);
		void set_black_knights(uint64_t n);
		void set_white_bishops(uint64_t n);
		void set_black_bishops(uint64_t n);
		void set_white_queens(uint64_t n);
		void set_black_queens(uint64_t n);
		void set_white_king(uint64_t n);
		void set_black_king(uint64_t n);

		const uint64_t get_pawns_white() const;
		const uint64_t get_pawns_black() const;
		const uint64_t get_rooks_white() const;
		const uint64_t get_rooks_black() const;
		const uint64_t get_knights_white() const;
		const uint64_t get_knights_black() const;
		const uint64_t get_bishops_white() const;
		const uint64_t get_bishops_black() const;
		const uint64_t get_queens_white() const;
		const uint64_t get_queens_black() const;
		const uint64_t get_king_white() const;
		const uint64_t get_king_black() const;

		uint64_t get_knights_white();
		uint64_t get_knights_black();

		void determine_white_capture(uint64_t n);
		void determine_black_capture(uint64_t n);

		const uint64_t getWhitePieces() const;
		const uint64_t getBlackPieces() const;
		const uint64_t getAllPieces() const;
		const bool white_king_castle() const;
		const bool white_queen_castle() const;
		const bool black_king_castle() const;
		const bool black_queen_castle() const;
		const int moves_50() const;
		const bool whites_move() const;
		const uint64_t get_en_passant_white() const;
		const uint64_t get_en_passant_black() const;

		void clear_en_passant();

		void set_special_bit(int n);
		void unset_special_bit(int n);

	private:
		// board state
		uint64_t pawns_white;
		uint64_t pawns_black;
		uint64_t rooks_white;
		uint64_t rooks_black;
		uint64_t knights_white;
		uint64_t knights_black;
		uint64_t bishops_white;
		uint64_t bishops_black;
		uint64_t queens_white;
		uint64_t queens_black;
		uint64_t king_white;
		uint64_t king_black;

		// special states
		// bit (L2R) | indicates
		// 0         | queen rook black hasn't moved
		// 4         | king black hasn't moved
		// 7		 | king rook black hasn't moved
		// 24 - 39	 | 1 if can be captured via en passant
		// 40 - 47	 | ply counter, used to tell who's move it is, even white, odd black
		// 48 - 55	 | keeps track of 50 move rule
		// 56		 | queen rook white hasn't moved
		// 60		 | king white hasn't moved
		// 63		 | king rook white hasn't moved
		uint64_t special_states;
	};

	std::ostream & operator<<(std::ostream & out, const Board board);
}