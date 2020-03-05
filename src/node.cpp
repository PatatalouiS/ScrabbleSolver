#include "node.hpp"
#include "utils.hpp"

using namespace std;

Node::Node(const unsigned char letter, const bool final) {
    this->letter = letter;
    this->final = final;

    for(Node*& node : childs) {
        node = nullptr;
    }
}

const ChildsArray& Node::getChilds() const {
    return childs;
}

unsigned char Node::getLetter() const {
    return letter;
}

Node* Node::getChildByLetter(const unsigned char letter) const {
//    for(Node* node : childs) {
//        if((node != nullptr) && (node->getLetter() == letter)) {
//            return node;
//        }
//    }
//    return nullptr;
    return childs[Utils::charToIndex(letter)];
}

unsigned int Node::getChildIndex(const Node* letter) {
    return Utils::charToIndex(letter->getLetter());
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
