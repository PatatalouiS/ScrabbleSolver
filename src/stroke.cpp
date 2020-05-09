
#include "stroke.hpp"
#include "utils.hpp"

using namespace std;

bool Stroke::operator==(const Stroke &other) const {
    return (word == other.word) &&
           (pos == other.pos) &&
           (direction == other.direction);
}

std::ostream& operator <<(std::ostream& out, const Stroke& stroke) {
    return out
        << "Word : " << stroke.word
        << " | Pos : " << stroke.pos
        << " | Direction : " << stroke.direction
        << " | Score : " << stroke.score;
}

//size_t std::hash<Stroke>::operator()(const Stroke& s) const {
//    return (std::hash<std::string>()(Utils::toRegularWord(s.word))
//            ^ (std::hash<SpotPos>()(s.pos) << 1));
//}

size_t std::hash<Stroke>::operator()(const Stroke& s) const {
    return ((std::hash<std::string>()(s.word)
             ^ (std::hash<SpotPos>()(s.pos) << 1)) >> 1)
             ^ (std::hash<Direction>()(s.direction) << 1);
}
