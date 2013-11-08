#include "human_player.hpp"

#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>

#include "game.hpp"

tictactoe::human_player::human_player(std::string name)
: player_name(name) {}

std::string tictactoe::human_player::name() const {
	return player_name;
}

void tictactoe::human_player::make_move(game_make_move_interface game) {
	const field::size_type
		order = game.field().order();

	game.field().print(
		std::cout,
		[&](std::string::size_type length, field::size_type index) {
			// reverse y-axis of index (check numpad to see why!)
			const field::size_type
				old_x = index % order,
				old_y = index / order,
				new_index = (order - old_y - 1) * order + old_x;

			std::stringstream ss;
			ss << std::setw(length) << new_index + 1;
			return ss.str();
		}
	);

	field::size_type index;
	while(true && std::cin) {
		try {
			(std::cout << "\nWhich tile do you want to play? ").flush();

			std::string line;
			std::getline(std::cin, line);
			std::stringstream ss(line);
			if (ss >> index) {
				index = index-1; // revert offset
				const field::size_type
					old_x = index % order,
					old_y = index / order,
					new_index = (order - old_y - 1) * order + old_x; // revert index transformation

				game.make_move(new_index);
				break;
			}
			else {
				std::cout << "That's not a valid tile number ... try again." << std::endl;
			}
		}
		catch(rule_violation_exception &e) {
			std::cout << e.what() << "\nTry again." << std::endl;
		}
	}
}
