#ifndef TICTACTOE_COMPUTER_PLAYER_HPP
#define TICTACTOE_COMPUTER_PLAYER_HPP

#include "player.hpp"

namespace tictactoe {

struct computer_player : player {
	/**
	 * Create a new computer player with a random name.
	 */
	computer_player();

	std::string name() const override;
	void make_move(game_make_move_interface) override;

private:
	std::string player_name;
};

}

#endif // TICTACTOE_COMPUTER_PLAYER_HPP
