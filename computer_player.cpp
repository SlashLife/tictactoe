#include "computer_player.hpp"

#include <cassert>
#include <iostream>
#include <random>

#include "game.hpp"
#include "computer_state_table.inc"

namespace {
	const char * random_computer_name() {
		static const char * const computer_names[] = {
			"BESM", "DeathStation 9000", "ENIAC", "ILLIAC", "MANIAC I", "OPREMA",
			"PDP-11", "TRADIC", "WEIZAC", "Zuse Z1"
		};
		constexpr std::ptrdiff_t num_names = sizeof(computer_names)/sizeof(*computer_names);

		// remembering one name will suffice to make sure that two
		// CPU players will get different names
		static std::ptrdiff_t previous_name_index = num_names; // initialize as "none of those"

		//std::random_device rd;
		constexpr int rd = 12345; // DEBUG: workaround for Windows: No working implementation of std::random_device available on MinGW w/ G++ 4.7.2
		std::mt19937 gen(rd);

		std::ptrdiff_t random_name_index;
		do {
			random_name_index = std::uniform_int_distribution<>
				(0, num_names-1)
				(gen);
		} while(previous_name_index == random_name_index);

		previous_name_index = random_name_index;

		return computer_names[random_name_index];
	}
}

tictactoe::computer_player::computer_player()
: player_name(random_computer_name()) {
}

std::string tictactoe::computer_player::name() const {
	return player_name;
}

void tictactoe::computer_player::make_move(game_make_move_interface game) {
	const field playfield(game.field());
	playfield.print(std::cout);
	std::cout << '\n';

	const field::size_type
		order = playfield.order(),
		size  = playfield.size();
	assert(order == 3 && "This AI will only work with a 3x3 playing field.");

	field::size_type occupied_tiles = 0;
	for(field::size_type index = 0; index < size; ++index) {
		occupied_tiles += (game[index] != field::tile::empty);
	}

	// Try to close a row
	if (4 <= occupied_tiles) { // makes no sense before both players have two tiles
		for(field::size_type index = 0; index < size; ++index) {
			if (playfield[index] == field::tile::empty && playfield.check_win_condition(index, game.current_player())) {
				game.make_move(index);
				return;
			}
		}
	}

	// Prevent opponet from closing
	if (3 <= occupied_tiles) { // makes no sense before first place has two tiles
		for(field::size_type index = 0; index < size; ++index) {
			if (playfield[index] == field::tile::empty && playfield.check_win_condition(index, game.opponent_player())) {
				game.make_move(index);
				return;
			}
		}
	}

	// check move database for next move
	{
		assert(game.field().order() == 3 &&
			"The move database only contains moves for 3x3 playing fields.");

		std::vector<game_make_move_interface> interfaces;
		interfaces.reserve(8);
		interfaces.emplace_back(game);
		interfaces.emplace_back(game.mirror());
		for(int i=0; i<6; ++i) {
			interfaces.emplace_back(interfaces[i].rotate());
		}
		/* interfaces = {
			orig, orig mirrored,
			90deg, 90deg mirrored,
			180deg, 180deg mirrored,
			270deg, 270deg mirrored
		} */

		for(const auto entry : computer_3x3_state_table::data) {
			if (entry.occupied_tiles != occupied_tiles) {
				continue;
			}

			for(auto interface : interfaces) {
				field::size_type index;
				for(index = 0; index < size; ++index) {
					if (interface[index] != entry.pattern[index]) {
						break;
					}
				}
				if (index == size) {
					interface.make_move(entry.next_move);
					return;
				}
			}
		}
	}

	// Still around? Then we're already set up for a tie, so it doesn't matter
	// Just take the first free field.
	{
		for(field::size_type index = 0; index < size; ++index) {
			try {
				game.make_move(index);
				return;
			}
			catch(rule_violation_exception &) {
				// ignore and try next field - after all we don't care which
				// one we get.
			}
		}
	}
}
