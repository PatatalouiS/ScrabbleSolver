#ifndef SUZETTECHECK_HPP
#define SUZETTECHECK_HPP

#include <string>

#include "board.hpp"
#include "playerbag.hpp"
#include "scrabbleconfig.hpp"

namespace Suzette {

    std::pair<unsigned int, Board> check(const Board& board,
                                         const PlayerBag& letters);
}

#endif // SUZETTECHECK_HPP
