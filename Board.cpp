#include "Board.h"
#include "Helper.h"
#include <bitset>
#include <iostream>

namespace NATHAN {
	Board::Board() {
		pawns_white = 0b1111111100000000;
		pawns_black = pawns_white << 40;
		rooks_white = 0b10000001;
		rooks_black = rooks_white << 56;
		knights_white = 0b1000010;
		knights_black = knights_white << 56;
		bishops_white = 0b100100;
		bishops_black = bishops_white << 56;
		queens_white = 0b10000;
		queens_black = queens_white << 56;
		king_white = 0b1000;
		king_black = king_white << 56;
		special_states = 0b1000100100000001000000000000000000000000000000000000000010001001;
	}

	Board::Board(const Board* b) {
		pawns_white = b->pawns_white;
		pawns_black = b->pawns_black;
		rooks_white = b->rooks_white;
		rooks_black = b->rooks_black;
		knights_white = b->knights_white;
		knights_black = b->knights_black;
		bishops_white = b->bishops_white;
		bishops_black = b->bishops_black;
		queens_white = b->queens_white;
		queens_black = b->queens_black;
		king_white = b->king_white;
		king_black = b->king_black;
		special_states = b->special_states;
	}

	Board::Board(std::string fen) {
	}

	Board::Board(char board[8][8]) {
		pawns_white = 0;
		pawns_black = 0;
		rooks_white = 0;
		rooks_black = 0;
		knights_white = 0;
		knights_black = 0;
		bishops_white = 0;
		bishops_black = 0;
		queens_white = 0;
		queens_black = 0;
		king_white = 0;
		king_black = 0;
		special_states = 0b1000100100000001000000000000000000000000000000000000000010001001;

		for(int i = 0; i < 64; i++) {
			uint64_t bin = (1L << (63 - i));
			switch(board[i / 8][i % 8]) {
				case 'P':
					pawns_white |= bin;
					break;
				case 'R':
					rooks_white |= bin;
					break;
				case 'N':
					knights_white |= bin;
					break;
				case 'B':
					bishops_white |= bin;
					break;
				case 'Q':
					queens_white |= bin;
					break;
				case 'K':
					king_white |= bin;
					break;
				case 'p':
					pawns_black |= bin;
					break;
				case 'r':
					rooks_black |= bin;
					break;
				case 'n':
					knights_black |= bin;
					break;
				case 'b':
					bishops_black |= bin;
					break;
				case 'q':
					queens_black |= bin;
					break;
				case 'k':
					king_black |= bin;
					break;
			}
		}
	}

	Board::~Board() {
	}

	uint64_t Board::getWhitePieces() {
		return pawns_white |
			rooks_white |
			knights_white |
			bishops_white |
			queens_white |
			king_white;
	}

	uint64_t Board::getBlackPieces() {
		return pawns_black |
			rooks_black |
			knights_black |
			bishops_black |
			queens_black |
			king_black;
	}

	uint64_t Board::getAllPieces() {
		return getWhitePieces() | getBlackPieces();
	}

	bool Board::white_king_castle() {
		return ((special_states & 0b1001) | (getWhitePieces() & 0b110)) == 0b1001;
	}

	bool Board::white_queen_castle() {
		return ((special_states & 0b10001000) | (getWhitePieces() & 0b1110000)) == 0b10001000;
	}

	bool Board::black_king_castle() {
		return ((special_states & ((uint64_t)0b1001 << 56)) | (getBlackPieces() & ((uint64_t)0b110 << 56))) == ((uint64_t)0b1001 << 56);
	}

	bool Board::black_queen_castle() {
		return ((special_states & ((uint64_t)0b10001000) << 56) | (getBlackPieces() & ((uint64_t)0b1110000) << 56)) == ((uint64_t)0b10001000 << 56);
	}

	int Board::moves_50() {
		return (0b1111111100000000 & special_states) >> 8;
	}

	void Board::set_white_pawns(uint64_t n) {
		pawns_white = n;
	}

	void Board::set_black_pawns(uint64_t n) {
		pawns_black = n;
	}

	void Board::set_white_rooks(uint64_t n) {
		rooks_white = n;
	}

	void Board::set_black_rooks(uint64_t n) {
		rooks_black = n;
	}

	void Board::set_white_knights(uint64_t n) {
		knights_white = n;
	}

	void Board::set_black_knights(uint64_t n) {
		knights_black = n;
	}

	void Board::set_white_bishops(uint64_t n) {
		bishops_white = n;
	}

	void Board::set_black_bishops(uint64_t n) {
		bishops_black = n;
	}

	void Board::set_white_queens(uint64_t n) {
		queens_white = n;
	}

	void Board::set_black_queens(uint64_t n) {
		queens_black = n;
	}

	void Board::set_white_king(uint64_t n) {
		king_white = n;
	}

	void Board::set_black_king(uint64_t n) {
		king_black = n;
	}

	const uint64_t Board::get_pawns_white() const {
		return pawns_white;
	}

	const uint64_t Board::get_pawns_black() const {
		return pawns_black;
	}

	const uint64_t Board::get_rooks_white() const {
		return rooks_white;
	}

	const uint64_t Board::get_rooks_black() const {
		return rooks_black;
	}

	const uint64_t Board::get_knights_white() const {
		return knights_white;
	}

	const uint64_t Board::get_knights_black() const {
		return knights_black;
	}

	const uint64_t Board::get_bishops_white() const {
		return bishops_white;
	}

	const uint64_t Board::get_bishops_black() const {
		return bishops_black;
	}

	const uint64_t Board::get_queens_white() const {
		return queens_white;
	}

	const uint64_t Board::get_queens_black() const {
		return queens_black;
	}

	const uint64_t Board::get_king_white() const {
		return king_white;
	}

	const uint64_t Board::get_king_black() const {
		return king_black;
	}

	///////////////////////////////////////////////////////////

	uint64_t Board::get_knights_white() {
		return knights_white;
	}

	uint64_t Board::get_knights_black() {
		return knights_black;
	}

	void Board::determine_white_capture(uint64_t n) {
		pawns_black &= ~n;
		rooks_black &= ~n;
		knights_black &= ~n;
		bishops_black &= ~n;
		queens_black &= ~n;
	}

	void Board::determine_black_capture(uint64_t n) {
		pawns_white &= ~n;
		rooks_white &= ~n;
		knights_white &= ~n;
		bishops_white &= ~n;
		queens_white &= ~n;
	}

	bool Board::whites_move() {
		return !(((uint64_t)1 << 16) & special_states);
	}

	void Board::increase_ply() {
		special_states = (special_states & ~(0b11111111 << 16)) | ((((special_states & 0b11111111 << 16) >> 16) + 1) << 16);
	}

	uint64_t Board::get_ply() {
		return (special_states & (0b11111111 << 16)) >> 16;
	}

	const uint64_t Board::getWhitePieces() const {
		return pawns_white |
			rooks_white |
			knights_white |
			bishops_white |
			queens_white |
			king_white;
	}

	const uint64_t Board::getBlackPieces() const {
		return pawns_black |
			rooks_black |
			knights_black |
			bishops_black |
			queens_black |
			king_black;
	}
	const uint64_t Board::getAllPieces() const {
		return getWhitePieces() | getBlackPieces();
	}

	const int Board::moves_50() const {
		return (0b1111111100000000 & special_states) >> 8;
	}

	const bool Board::whites_move() const {
		return 281474976710656 & special_states;
	}

	const uint64_t Board::get_en_passant_white() const {
		return Helper::rows[4] & special_states;
	}

	const uint64_t Board::get_en_passant_black() const {
		return Helper::rows[3] & special_states;
	}

	void Board::clear_en_passant() {
		special_states &= ~((((uint64_t)1 << 17) - 1) << 24);
	}

	void Board::set_special_bit(int n) {
		special_states |= (uint64_t)1 << n;
	}

	void Board::unset_special_bit(int n) {
		special_states &= ~((uint64_t)1 << n);
	}

	std::ostream& operator<<(std::ostream& out, const Board board) {
		/*std::string str = std::bitset<64>(board.getAllPieces()).to_string();
		for (int i = 0; i < 8; i++) {
			out << str.substr(8 * i, 8) << std::endl;
		}

		out << std::endl;
		out << "WKC: " << (board.white_king_castle() ? 1 : 0) << std::endl;
		out << "WQC: " << (board.white_queen_castle() ? 1 : 0) << std::endl;
		out << "BKC: " << (board.black_king_castle() ? 1 : 0) << std::endl;
		out << "BQC: " << (board.black_queen_castle() ? 1 : 0) << std::endl;
		out << "C|P: " << board.moves_50() << std::endl;
		out << "W|B: " << (board.whites_move() ? std::string("W") : std::string("B")) << std::endl; */

		char b[8][8];

		for(int i = 0; i < 64; i++) {
			b[i / 8][i % 8] = '.';
			if((1 & (board.get_pawns_white() >> i)) == 1) {
				b[i / 8][i % 8] = 'P';
			}
			else if((1 & (board.get_pawns_black() >> i)) == 1) {
				b[i / 8][i % 8] = 'p';
			}
			else if((1 & (board.get_rooks_white() >> i)) == 1) {
				b[i / 8][i % 8] = 'R';
			}
			else if((1 & (board.get_rooks_black() >> i)) == 1) {
				b[i / 8][i % 8] = 'r';
			}
			else if((1 & (board.get_knights_white() >> i)) == 1) {
				b[i / 8][i % 8] = 'N';
			}
			else if((1 & (board.get_knights_black() >> i)) == 1) {
				b[i / 8][i % 8] = 'n';
			}
			else if((1 & (board.get_bishops_white() >> i)) == 1) {
				b[i / 8][i % 8] = 'B';
			}
			else if((1 & (board.get_bishops_black() >> i)) == 1) {
				b[i / 8][i % 8] = 'b';
			}
			else if((1 & (board.get_queens_white() >> i)) == 1) {
				b[i / 8][i % 8] = 'Q';
			}
			else if((1 & (board.get_queens_black() >> i)) == 1) {
				b[i / 8][i % 8] = 'q';
			}
			else if((1 & (board.get_king_white() >> i)) == 1) {
				b[i / 8][i % 8] = 'K';
			}
			else if((1 & (board.get_king_black() >> i)) == 1) {
				b[i / 8][i % 8] = 'k';
			}
		}

		for(int i = 7; i >= 0; i--) {
			for(int j = 7; j >= 0; j--) {
				out << b[i][j];
			}
			out << std::endl;
		}

		return out;
	}
}