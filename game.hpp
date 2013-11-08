#ifndef TICTACTOE_GAME_HPP_INCLUDED
#define TICTACTOE_GAME_HPP_INCLUDED

#include <functional>
#include <stdexcept>
#include <string>

#include "field.hpp"

namespace tictactoe {

struct game_state;
struct player;

struct rule_violation_exception : std::runtime_error {
	rule_violation_exception(std::string what)
	: std::runtime_error(what) {}
};

struct game_make_move_interface {
	/**
	 * Transformation callback.
	 * \param A reference to the field on which the transformation is to take
	 *        place.
	 * \param The flat tile index to be transformed.
	 * \return The transformed index.
	 * \note The field that is referenced always represents the original field
	 *       regardless of any other transformations with higher priority.
	 */
	typedef std::function<tictactoe::field::size_type(const tictactoe::field &, tictactoe::field::size_type)> transformation;

	/**
	 * Create a new move interface from a game state and an initial
	 * transformation.
	 */
	game_make_move_interface(game_state &, transformation);

	/**
	 * Returns the state of the tile with the given index, taking all
	 * active transformations into account.
	 * \param index The transformed index of the tile.
	 */
	tictactoe::field::tile operator[](tictactoe::field::size_type index) const;

	/**
	 * Makes a move by playing the current players state to the tile with the
	 * given index, taking all active transformations into account.
	 * \param index The transformed index of the tile.
	 * \throw rule_violation_exception in case the move is illegal.
	 * \note You can only call this function once per game state. All
	 *       subsequent calls will be considered a rule violation and throw an
	 *       exception accordingly.
	 */
	void make_move(tictactoe::field::size_type index);

	/**
	 * A reference to the field that is played on. No transformations are
	 * applied.
	 */
	const tictactoe::field &field() const;

	/**
	 * Returns the state the current player plays.
	 * \note A successful call to make_move() will play this state on the
	 *       chosen tile.
	 */
	tictactoe::field::tile current_player() const;

	/**
	 * Returns the state the opponent plays.
	 */
	tictactoe::field::tile opponent_player() const;

	/**
	 * Returns a copy of this interface which applies an additional
	 * transformation, but manipulates the same game state.
	 * \param callback The additional transformation to be applied.
	 * \return The new interface.
	 */
	game_make_move_interface transform(transformation callback) const;

	/**
	 * Returns a copy of this interface which applies an additional
	 * rotation transformation, but manipulates the same game state.
	 * \return The new interface.
	 */
	game_make_move_interface rotate() const;

	/**
	 * Returns a copy of this interface which applies an additional
	 * mirror transformation, but manipulates the same game state.
	 * \return The new interface.
	 */
	game_make_move_interface mirror() const;

private:
	game_state &state;
	transformation transformation_func;
};

/**
 * Start a game with two players.
 * \param player1 The first player. This player will have the first move.
 * \param player2 The second player.
 * \return A pointer to the winning player or nullptr in case of a draw.
 */
player *game(player &player1, player &player2);

}

#endif // TICTACTOE_GAME_HPP_INCLUDED
