#include <iostream>
#include <memory>

#include "computer_player.hpp"
#include "game.hpp"
#include "human_player.hpp"

using namespace tictactoe;

std::unique_ptr<player> make_player(std::string name) {
	return ("cpu" == name)
		? std::unique_ptr<player>(new computer_player())
		: std::unique_ptr<player>(new human_player(name));
}

int main(int argc, const char * const argv[]) {
	if (argc < 3) {
		std::cerr <<
			"Usage:\n"
			"\t" << argv[0] << " <player1> <player2>\n"
			"\n"
			"<player1>, <player2>\n"
			"\tThe names for the respective players.\n"
			"\tTo play against the computer, use the name \"cpu\".\n";
	}
	else {
		std::unique_ptr<player>
			player1(make_player(argv[1])),
			player2(make_player(argv[2]));

		game(*player1, *player2);
	}

	return 0;
}
