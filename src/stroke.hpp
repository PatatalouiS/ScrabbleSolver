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
    std::string word;
    SpotPos pos;
    Direction direction;
    unsigned int score;

    Stroke(const std::string& w, const SpotPos& p, const Direction d, const unsigned int s) :
        word(w), pos(p), direction(d), score(s){
    }

    bool operator==(const Stroke& other) const {
        return (word == other.word) && (pos == other.pos);
    }
};


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


