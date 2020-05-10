#include "spotpos.hpp"
#include <ostream>

using namespace std;

bool SpotPos::operator==(const SpotPos &other) const {
    return (indexLine == other.indexLine) &&
           (indexCol == other.indexCol);
}

ostream& operator<<(ostream& out, const SpotPos& sp) {
    return out << "( " << int(sp.indexLine)
               << " , " << int(sp.indexCol) << " )";
}

size_t hash<SpotPos>::operator()(const SpotPos& pos) const {
    auto hash = std::hash<char>();
    return hash(pos.indexLine) ^ (hash(pos.indexCol) << 1);
}

size_t hash<pair<SpotPos, Direction>>::operator()(const pair<SpotPos,
                                                  Direction>& p) const {
    return hash<SpotPos>()(p.first)
           ^ (hash<Direction>()(p.second) << 1);
}
