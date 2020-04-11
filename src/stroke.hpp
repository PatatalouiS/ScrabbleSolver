#ifndef DIRECTION
#define DIRECTION

#include <iostream>

enum class Direction {
    HORIZONTAL,
    VERTICAL
};

inline std::ostream& operator<<(std::ostream& out, enum Direction d) {
    return out << (d == Direction::HORIZONTAL
           ? "HORIZONTAL"
           : "VERTICAL");
}

#endif

#ifndef STROKE_HPP
#define STROKE_HPP

#include <string>
#include "spot.hpp"

struct Stroke {
    std::string word = "";
    SpotPos pos = SpotPos(0, 0);
    Direction direction = Direction::HORIZONTAL;
    unsigned int score = 0;

    bool operator==(const Stroke& other) const {
        return (word == other.word) && (pos == other.pos);
    }
};

inline std::ostream& operator <<(std::ostream& out, const Stroke& stroke) {
    return out
        << "Word : " << stroke.word
        << " | Pos : " << stroke.pos
        << " | Direction : " << stroke.direction
        << " | Score : " << stroke.score;
}

namespace std {
    template<>
    struct hash<Stroke> {
        size_t operator()(const Stroke& s) const {
            return (std::hash<std::string>()(s.word)
                    ^ (std::hash<SpotPos>()(s.pos) << 1));
        }
    };
}

#endif // STROKE_HPP
