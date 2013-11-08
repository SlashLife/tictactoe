#ifndef TICTACTOE_HUMAN_PLAYER_HPP_INCLUDED
#define TICTACTOE_HUMAN_PLAYER_HPP_INCLUDED

#include "player.hpp"

namespace tictactoe {

struct human_player : player {
	/**
	 * Create a new human player.
	 * \param name The name of the player.
	 */
	human_player(std::string name);

	std::string name() const override;
	void make_move(game_make_move_interface) override;

private:
	std::string player_name;
};

}

#endif // TICTACTOE_HUMAN_PLAYER_HPP_INCLUDED
