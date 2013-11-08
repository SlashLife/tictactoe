#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>

#include "computer_player.hpp"
#include "field.hpp"
#include "game.hpp"
#include "player.hpp"

using namespace tictactoe;

struct test_player : player {
	/**
	 * Creates a new CPU test player with a predefined test pattern.
	 */
	test_player(
		const std::vector<field::size_type> pattern,
		field::size_type seed
	) {
		std::stringstream namebuilder;
		namebuilder << "test_player({";

		moves.reserve(pattern.size());
		for(auto move_pattern : pattern) {
			const auto move = seed % move_pattern;
			seed /= move_pattern;

			if (!moves.empty()) namebuilder << ", ";
			namebuilder << move;
			moves.emplace_back(move);
		}
		next_move = moves.begin();

		namebuilder << "}) @ " << static_cast<const void *>(this);
		playername = namebuilder.str();
	}

	std::string name() const override {
		return playername;
	}

	void make_move(game_make_move_interface game_interface) override {
		try {
			field::size_type index = 0;
			while(index < game_interface.field().size()) {
				if (game_interface[index] == field::tile::empty) {
					if (0 == *next_move) {
						++next_move;
						game_interface.make_move(index);
						return;
					}
					--*next_move;
				}
				++index;
			}
			std::cerr << "FAILURE: Did not make a move!\n";
			exit(1);
		}
		catch(...) {
			std::cerr << "FAILURE: Exception caught!\n";
			exit(1);
		}
	}

	std::string playername;
	std::vector<field::size_type> moves;
	std::vector<field::size_type>::iterator next_move;
};

int main() {
	field::size_type stats[3] = {0, 0, 0};

	{ std::vector<field::size_type> pattern = {9, 7, 5, 3, 1};
		const field::size_type num_patterns = std::accumulate(
			pattern.begin(), pattern.end(), 1,
			std::multiplies<field::size_type>()
		);

		for(field::size_type seed=0; seed < num_patterns; ++seed) {
			computer_player computer;
			test_player tester(pattern, seed);
			player *winner = game(tester, computer);

			++stats[
				1
				+(winner == &computer)
				-(winner == &tester)
			];

			if (winner == &tester) {
				std::cerr << "FAILURE: Computer loses!\n";
				return 1;
			}
		}
	}

	{ std::vector<field::size_type> pattern = {8, 6, 4, 2};
		const field::size_type num_patterns = std::accumulate(
			pattern.begin(), pattern.end(), 1,
			std::multiplies<field::size_type>()
		);

		for(field::size_type seed=0; seed < num_patterns; ++seed) {
			computer_player computer;
			test_player tester(pattern, seed);
			player *winner = game(computer, tester);

			++stats[
				1
				+(winner == &computer)
				-(winner == &tester)
			];

			if (winner == &tester) {
				std::cerr << "FAILURE: Computer loses!\n";
				return 1;
			}
		}
	}

	std::cout <<
		"Stats:\n"
		"  Tester wins: " << stats[0] << "\n"
		"  Draw game:   " << stats[1] << "\n"
		"  CPU wins:    " << stats[2] << "\n"
		"Total games played: " << (stats[0] + stats[1] + stats[2]) << "\n";
	return 0;
}
