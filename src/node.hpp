#ifndef NODE_HPP
#define NODE_HPP

#include <array>

class Node;

using ChildsArray = std::array<Node*, 27>;

class Node {

    private:

        static unsigned int getChildIndex(const Node* node);

    public:

        static constexpr Node* const NO_NODE = nullptr;

        Node(const char unsigned letter, const bool final = false);

        unsigned char getLetter() const;

        const ChildsArray& getChilds() const;

        Node* getChildByLetter(const unsigned char letter) const;

        void addChild(Node* newNode);

        void setFinal(const bool final = true);

        bool isFinal() const;

    private:

        unsigned char letter;

        bool final;

        ChildsArray childs;
};

#endif // NODE_HPP
