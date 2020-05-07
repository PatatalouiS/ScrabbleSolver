#ifndef STROKE_HPP
#define STROKE_HPP

#include <string>
#include "spotpos.hpp"
#include "direction.hpp"

struct Stroke {
    std::string word = "";
    SpotPos pos = SpotPos{0, 0};
    Direction direction = Direction::HORIZONTAL;
    unsigned int score = 0;

    bool operator==(const Stroke& other) const;
};

std::ostream& operator <<(std::ostream& out, const Stroke& stroke);

namespace std {
    template<>
    struct hash<Stroke> {
        size_t operator()(const Stroke& s) const;
    };
}

#endif // STROKE_HPP
