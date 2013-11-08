#include <vector>

#include "field.hpp"

namespace tictactoe {
namespace computer_3x3_state_table {
	namespace {
		const field::tile
			X = field::tile::player1,
			O = field::tile::player2,
			_ = field::tile::empty,
			x = field::tile::empty,
			o = field::tile::empty;
	}

	struct entry {
		field::size_type occupied_tiles;
		field::size_type next_move;
		field pattern;
	};

	// X = mark by player 1, O = mark by player 2
	// _ = empty tile
	// x = computer is player 1 and should put a mark in this tile
	// o = computer is player 2 and should put a mark in this tile
	const std::vector<entry> data = {

////////////////////////////////////////////////////////////////////////////////
// States for player 1 (even number of tiles played)
//

		{ 0, 0, {
			x, _, _,
			_, _, _,
			_, _, _
		}},
		{ 2, 4, {
			X, O, _,
			_, x, _,
			_, _, _
		}},
		{ 2, 8, {
			X, _, O,
			_, _, _,
			_, _, x
		}},
		{ 2, 4, {
			X, _, _,
			_, x, O,
			_, _, _
		}},
		{ 2, 8, {
			X, _, _,
			_, O, _,
			_, _, x
		}},
		{ 2, 4, {
			X, _, _,
			_, x, _,
			_, _, O
		}},
		{ 4, 6, {
			X, O, _,
			_, X, _,
			x, _, O
		}},

////////////////////////////////////////////////////////////////////////////////
// States for player 2 (odd number of tiles played)
//

		{ 1, 0, {
			o, _, _,
			_, X, _,
			_, _, _
		}},
		{ 1, 4, {
			X, _, _,
			_, o, _,
			_, _, _
		}},
		{ 1, 4, {
			_, X, _,
			_, o, _,
			_, _, _
		}},
		{ 3, 2, {
			X, _, o,
			_, O, X,
			_, _, _
		}},
		{ 3, 1, {
			X, o, _,
			_, O, _,
			_, _, X
		}},
		{ 3, 2, {
			O, _, o,
			_, X, _,
			_, _, X
		}},
		{ 3, 7, {
			O, X, _,
			_, X, _,
			_, o, _
		}},
		{ 3, 0, {
			o, X, _,
			X, O, _,
			_, _, _
		}},
		{ 5, 6, {
			O, X, _,
			_, X, _,
			o, O, X
		}}

	};
}
}
