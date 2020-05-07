
#include "stroke.hpp"

using namespace std;

bool Stroke::operator==(const Stroke &other) const {
    return (word == other.word) && (pos == other.pos);
}

std::ostream& operator <<(std::ostream& out, const Stroke& stroke) {
    return out
        << "Word : " << stroke.word
        << " | Pos : " << stroke.pos
        << " | Direction : " << stroke.direction
        << " | Score : " << stroke.score;
}

size_t std::hash<Stroke>::operator()(const Stroke& s) const {
    return (std::hash<std::string>()(s.word)
            ^ (std::hash<SpotPos>()(s.pos) << 1));
}
