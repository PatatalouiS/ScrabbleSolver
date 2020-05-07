#include "spotpos.hpp"
#include <ostream>

using namespace std;

bool SpotPos::operator==(const SpotPos &other) const {
    return (indexLine == other.indexLine) &&
           (indexCol == other.indexCol);
}

std::ostream& operator<<(std::ostream& out, const SpotPos& sp) {
    return out << "( "
               << int(sp.indexLine)
               << " , "
               << int(sp.indexCol)
               << " )";
}

size_t std::hash<SpotPos>::operator()(const SpotPos& pos) const {
    auto hash = std::hash<char>();
    return hash(pos.indexLine) ^ (hash(pos.indexCol) << 1);
}
