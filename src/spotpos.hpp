#ifndef SPOTPOS_HPP
#define SPOTPOS_HPP

#include <functional>
#include <iostream>
#include "direction.hpp"

struct SpotPos {
    char indexLine;
    char indexCol;

    bool operator==(const SpotPos& other) const;
};

const SpotPos CENTER_OF_BOARD = SpotPos{ 7, 7 };

std::ostream& operator<<(std::ostream& out, const SpotPos& sp);

namespace std {
    template<>
    struct hash<SpotPos> {
        size_t operator()(const SpotPos& pos) const;
    };

    template<>
    struct hash<std::pair<SpotPos, Direction>> {
        size_t operator()(const pair<SpotPos, Direction>& p) const;
    };
}

#endif // SPOTPOS_HPP
