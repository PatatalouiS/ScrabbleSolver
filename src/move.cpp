
#include "move.hpp"
#include "utils.hpp"

using namespace std;

bool Move::operator==(const Move &other) const {
    return (word == other.word) &&
           (pos == other.pos) &&
           (direction == other.direction);
}

std::ostream& operator <<(std::ostream& out, const Move& stroke) {
    return out
        << "Word : " << stroke.word
        << " | Pos : " << stroke.pos
        << " | Direction : " << stroke.direction
        << " | Score : " << stroke.score;
}

size_t std::hash<Move>::operator()(const Move& s) const {
    //Use XOR and bit shifting for good hashes
    return ((std::hash<std::string>()(s.word)
             ^ (std::hash<SpotPos>()(s.pos) << 1)) >> 1)
             ^ (std::hash<Direction>()(s.direction) << 1);
}
