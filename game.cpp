#include <cassert>

#include <iostream>

#include "game.hpp"
#include "player.hpp"



namespace {
	tictactoe::player &if_tile_state(
		tictactoe::field::tile state,
		tictactoe::player &if_player_1,
		tictactoe::player &if_player_2
	) {
		assert(state != tictactoe::field::tile::empty);
		return (state == tictactoe::field::tile::player1)
			? if_player_1
			: if_player_2;
	}

	tictactoe::field::size_type identity_transformation(const tictactoe::field &, tictactoe::field::size_type index) {
		return index;
	}
}



////////////////////////////////////////////////////////////////////////////////
// tictactoe::game_state
//

namespace tictactoe {
	struct game_state {
		game_state()
		: field()
		, current_player(tictactoe::field::tile::player2)
		, can_move(false)
		, game_won(false) {}

		void prepare_next_move() {
			if (can_move) {
				throw rule_violation_exception("You have not made a move.");
			}
			if (game_won) {
				throw std::runtime_error("The game is already won.");
			}
			else {
				current_player = opponent();
				can_move = true;
			}
		}

		tictactoe::field::tile opponent() {
			return (current_player == tictactoe::field::tile::player1)
				? tictactoe::field::tile::player2
				: tictactoe::field::tile::player1;
		}

		tictactoe::field field;
		tictactoe::field::tile current_player;
		bool can_move;
		bool game_won;
	};
}



////////////////////////////////////////////////////////////////////////////////
// tictactoe::game_make_move_interface
//

tictactoe::game_make_move_interface::game_make_move_interface(game_state &state, transformation transformation_func)
: state(state)
, transformation_func(transformation_func) {}

tictactoe::field::tile tictactoe::game_make_move_interface::operator[](tictactoe::field::size_type index) const {
	return state.field[transformation_func(state.field, index)];
}

void tictactoe::game_make_move_interface::make_move(tictactoe::field::size_type index) {
	if (!state.can_move) {
		throw rule_violation_exception("You have already made your move.");
	}
	try {
		tictactoe::field::tile &tile = state.field[transformation_func(state.field, index)];
		if (tile != tictactoe::field::tile::empty) {
			throw rule_violation_exception("The chosen tile is already occupied.");
		}
		tile = state.current_player;
		state.game_won = state.field.check_win_condition(index);
		state.can_move = false;
	}
	catch(std::out_of_range &) {
		throw rule_violation_exception("The chosen tile is invalid.");
	}
}

const tictactoe::field &tictactoe::game_make_move_interface::field() const {
	return state.field;
}

tictactoe::field::tile tictactoe::game_make_move_interface::current_player() const {
	return state.current_player;
}

tictactoe::field::tile tictactoe::game_make_move_interface::opponent_player() const {
	return state.opponent();
}

tictactoe::game_make_move_interface tictactoe::game_make_move_interface::transform(transformation new_transformation) const {
	transformation current_transformation = transformation_func; // necessary to prevent capture by reference
	return game_make_move_interface(
		state,
		[current_transformation, new_transformation](const tictactoe::field &field, tictactoe::field::size_type index)
		-> tictactoe::field::size_type {
			return new_transformation(field, current_transformation(field, index));
		}
	);
}

tictactoe::game_make_move_interface tictactoe::game_make_move_interface::rotate() const {
	return transform(
		[](const tictactoe::field &field, tictactoe::field::size_type index)
		-> tictactoe::field::size_type {
			const tictactoe::field::size_type
				order = field.order(),
				max_coord = order - 1,

				old_x = index % order,
				old_y = index / order,

				                           // Transformation matrix:
				new_x = max_coord - old_y, //   | 0 -1 |
				new_y = old_x,             //   | 1  0 |

				transformed_index = new_x + new_y * order;

			return transformed_index;
		}
	);
}

tictactoe::game_make_move_interface tictactoe::game_make_move_interface::mirror() const {
	return transform(
		[](const tictactoe::field &field, tictactoe::field::size_type index)
		-> tictactoe::field::size_type {
			const tictactoe::field::size_type
				order = field.order(),
				max_coord = order - 1,

				old_x = index % order,
				old_y = index / order,

				                           // Transformation matrix:
				new_x = max_coord - old_x, //   | -1 0 |
				new_y = old_y,             //   |  0 1 |

				transformed_index = new_x + new_y * order;

			return transformed_index;
		}
	);
}



////////////////////////////////////////////////////////////////////////////////
// main game function
//

tictactoe::player *tictactoe::game(player &player1, player &player2) {
	game_state state;

	auto current_player = [&]() -> player& {
		return if_tile_state(state.current_player, player1, player2);
	};
	auto opponent_player = [&]() -> player& {
		return if_tile_state(state.current_player, player2, player1);
	};

	try {
		field::size_type moves_left = state.field.size();
		while(moves_left-- && !state.game_won) {
			state.prepare_next_move();
			std::cout << current_player().name() << ": Your turn!\n";
			current_player().make_move(game_make_move_interface(state, identity_transformation));
		}

		std::cout << "Game over!\n";
		state.field.print(std::cout);
		std::cout << '\n';

		if (state.game_won) {
			std::cout <<
				"Congratulations, " << current_player().name() << ", you won!\n" <<
				opponent_player().name() << ", better luck next time.\n";
			return &(current_player());
		}
		else {
			std::cout <<
				"It's a tie. Why not give it another try and play again?\n";
			return nullptr;
		}
	}
	catch(rule_violation_exception &e) {
		std::cout <<
			current_player().name() << " has violated the rules.\n"
			"Congratulations, " << opponent_player().name() << ", you won!\n";
		return &(opponent_player());
	}
	catch(...) {
		std::cout <<
			"Something went wrong during " << current_player().name() << "s turn.\n"
			"The game is called off.\n";
		throw;
	}
}
