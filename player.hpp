#ifndef TICTACTOE_PLAYER_HPP_INCLUDED
#define TICTACTOE_PLAYER_HPP_INCLUDED

#include <string>

namespace tictactoe {

struct game_make_move_interface;

struct player {
	virtual ~player() = default;

	/**
	 * Returns the name of the current player.
	 */
	virtual std::string name() const = 0;

	/**
	 * Determines and commits the next move.
	 */
	virtual void make_move(game_make_move_interface) = 0;
};

}

#endif // TICTACTOE_PLAYER_HPP_INCLUDED
