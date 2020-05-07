#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <iostream>

enum class Direction {
    HORIZONTAL,
    VERTICAL
};

std::ostream& operator<<(std::ostream& out, enum Direction d);

#endif // DIRECTION_HPP
