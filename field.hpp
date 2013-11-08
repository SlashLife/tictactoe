#ifndef TICTACTOE_FIELD_HPP_INCLUDED
#define TICTACTOE_FIELD_HPP_INCLUDED

#include <functional>
#include <ostream>
#include <vector>

namespace tictactoe {

struct field {
	/**
	 * Different states for each tile of the play field.
	 */
	enum class tile : unsigned {
		empty,
		player1,
		player2
	};

	typedef std::vector<tile> storage_type;
	typedef storage_type::size_type size_type;

	/**
	 * Create an empty field.
	 */
	field();

	/**
	 * Create a field from a list of states.
	 * \param init_tiles A list of the initial tile states; init_tiles.size()
	 *        must be sqr(this->order())
	 */
	field(std::initializer_list<tile> init_tiles);

	/**
	 * Access a tile.
	 * \param index The flat index of the tile.
	 * \return A reference to the tile.
	 */
	inline tile &operator[](size_type index) { return tiles.at(index); }

	/**
	 * Access a tile.
	 * \param index The flat index of the tile.
	 * \return The state of the tile.
	 */
	inline const tile operator[](size_type index) const { return tiles.at(index); }

	/**
	 * Checks whether the tile at a certain index is involved in a
	 * winning condition.
	 * \param index The flat index of the tile to be checked.
	 * \return true iff the tile at index is part of a full row.
	 */
	bool check_win_condition(const size_type index) const;

	/**
	 * Checks whether playing a certain state on a tile would result in a
	 * winning condition.
	 * \param index The flat index of the tile to be checked.
	 * \param state The state that is supposedly played.
	 * \return true iff the tile at index would become part of a full row.
	 */
	bool check_win_condition(const size_type index, tile state) const;

	/**
	 * A callback type for generating empty tile captions for
	 * print(std::ostream&, empty_tile_caption_callback).
	 * \param The expected length of the returned string.
	 * \param The flat index for the tile the caption is generated for.
	 */
	typedef std::function<std::string(std::string::size_type, size_type)> empty_tile_caption_callback;

	/**
	 * Prints the field.
	 * \param os The output stream to print to.
	 */
	void print(std::ostream &os) const;

	/**
	 * Prints the field using a given callback to generate captions for
	 * empty tiles.
	 * \param os The output stream to print to.
	 * \param callback A function that generates captions for empty tiles.
	 */
	void print(std::ostream &os, empty_tile_caption_callback callback) const;

	/**
	 * Returns the order of the field.
	 */
	inline size_type order() const noexcept { return 3; /* Tic-Tac-Toe is a 3x3 playfield */ }

	/**
	 * Returns the total number of tiles.
	 */
	inline size_type size() const noexcept { return order() * order(); }

	/**
	 * Returns whether all tiles on the field are empty.
	 */
	bool empty() const noexcept;

private:
	storage_type tiles;
};

}

#endif // TICTACTOE_FIELD_HPP_INCLUDED
