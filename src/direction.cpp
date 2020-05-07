
#include "direction.hpp"

using namespace std;

ostream& operator<<(ostream& out, enum Direction d) {
    return out << (d == Direction::HORIZONTAL
           ? "HORIZONTAL"
           : "VERTICAL");
}
