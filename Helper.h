#pragma once

#include <cstdint>

namespace NATHAN {
	class Helper {
	public:

		Helper();

		static const int pop_count[];
		static const uint64_t knight_moves[];
		static const uint64_t king_moves[];
		static const uint64_t rook_moves[];
		static const uint64_t bishop_moves[];
		static const uint64_t queen_moves[];
		
		static const uint64_t files[];
		static const uint64_t rows[];
		static const uint64_t diagonal[];
		static const uint64_t antidiagonal[];

		static uint64_t pop_count64(uint64_t num);
		static uint64_t lsb(uint64_t num);
		static uint64_t reverse(uint64_t num);
	private:

	};
}