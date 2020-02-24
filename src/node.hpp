#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <array>
#include <optional>

class Node;

using ChildsArray = std::array<Node*, 26>;

class Node {


    private:

    static inline unsigned int getChildIndex(const Node* node);

    public:

    static constexpr Node* const NO_NODE = nullptr;

    Node(const char letter, const bool final = false);

    char getLetter() const;

    const ChildsArray& getChilds() const;

    Node* getChildByLetter(const char letter) const;

    void addChild(Node* newNode);

    void setFinal(const bool final = true);

    bool isFinal() const;

    private:

    char letter;

    bool final;

    ChildsArray childs;
};

#endif // NODE_HPP
