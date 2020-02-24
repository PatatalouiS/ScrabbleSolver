#include "node.hpp"

using namespace std;

using ChildsArray = std::array<Node*, 26>;

Node::Node(const char letter, const bool final) {
    this->letter = letter;
    this->final = final;
}

const ChildsArray& Node::getChilds() const {
    return childs;
}

char Node::getLetter() const {
    return letter;
}

Node* Node::getChildByLetter(const char letter) const {
    for(Node* node : childs) {
        if(node->getLetter() == letter) {
            return node;
        }
    }
    return nullptr;
}

unsigned int Node::getChildIndex(const Node* letter) {
    return static_cast<unsigned>(letter->getLetter() - 'A');
}

void Node::addChild(Node* newNode) {
    childs[getChildIndex(newNode)] = newNode;
}

void Node::setFinal(const bool final) {
    this->final = final;
}

bool Node::isFinal() const {
    return final;
}
