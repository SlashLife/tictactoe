#include <cassert>

#include <algorithm>

#include "field.hpp"

namespace {
    constexpr unsigned num_digits(unsigned val) {
        return (9 < val)
            ? 1 + num_digits(val/10)
            : 1;
    }

    std::string repeat_string(const std::string repeatable, size_t repeats) {
        std::string result;
        while(repeats--) {
            result += repeatable;
        }
        return result;
    }
}

tictactoe::field::field()
: tiles(size(), tile::empty) {}

tictactoe::field::field(std::initializer_list<tile> init_tiles)
: tiles(init_tiles) {
	// check whether the number of tiles equals the expected number
	assert(tiles.size() == size());
}

bool tictactoe::field::check_win_condition(const size_type index) const {
	return check_win_condition(index, (*this)[index]);
}

bool tictactoe::field::check_win_condition(const size_type index, tile state) const {
	(*this)[index];
	// op[] checks the index and throws in case of an invalid index
	// so from here on, index is guaranteed to be valid

	// check a winning line using this->order() elements, starting with the i
	// index passed for begin and then increasing by skip steps - this model can
	// handle all four (see below) distinct winning conditions
	auto check_winning_line = [this, index, state](size_type begin, const size_type skip) {
		unsigned num_elements = order();
		for(; num_elements--; begin += skip) {
			if (
				// this is the tile index that is currently checked:
				// *assume* equality for it without checking for the actual value!
				begin != index &&

				// other tiles in the range much contain the given state
				tiles[begin] != state
			) {
				return false;
			}
		}
		return true;
	};

	// possible point for optimization, however: no premature optimization!
	// algorithmic complexity (time: O(this->order()), mem: O(1)) couldn't be
	// improved anyway - the rest are integer arithmetic and comparisons:
	// in any case well enough for a quick tictactoe writeup
	return
		// row
		check_winning_line(index - index % order(), 1) ||
		// column
		check_winning_line(index % order(), order()) ||
		// diagonal (\)
		check_winning_line(0, order() + 1) ||
		// diagonal (/)
		check_winning_line(order() - 1, order() - 1);
}

void tictactoe::field::print(std::ostream &os) const {
	print(
		os,
		[this](std::string::size_type length, size_type /* index; unused */) -> std::string {
			return std::string(length, ' ');
		}
	);
}

void tictactoe::field::print(std::ostream &os, empty_tile_caption_callback on_empty) const {
    const std::string::size_type
        caption_length = num_digits(this->size()),
        player_state_padding_length = caption_length / 2; // truncation by int division is intentional
    const std::string
        player_state_padding = std::string(player_state_padding_length, ' '),
        X(player_state_padding + ((caption_length % 2) ? "X" : "><") + player_state_padding),
        O(player_state_padding + ((caption_length % 2) ? "O" : "()") + player_state_padding),

        row_separator("\n +" + repeat_string(std::string(2 + caption_length, '-') + "+", order()) + " \n");
    const char
        * const field_separator(" | ");

    for(size_type index=0; index < size(); ++index) {
        if (0 == index % order()) {
            // first field in a new row
            os << row_separator << field_separator;
        }

        switch(tiles[index]) {
        case tile::empty:
            os << on_empty(caption_length, index);
            break;
        case tile::player1:
            os << X;
            break;
        case tile::player2:
            os << O;
            break;
        // no default case - issue warning if cases are not specifically handled
        }
        os << field_separator;
    }
    os << row_separator;
}

bool tictactoe::field::empty() const noexcept {
	return std::all_of(
		tiles.begin(),
		tiles.end(),
		[](const tile value) {
			return value == tile::empty;
		}
	);
}
