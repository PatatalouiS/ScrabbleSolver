#ifndef NODE_HPP
#define NODE_HPP

#include <array>

class Node;

using ChildsArray = std::array<Node*, 27>;

///
/// \brief The Node class represents a Node in the Gaddagg tree
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
///
class Node {

    public:

    ///
    /// \brief Node Contructor
    /// \param letter the letter to be affected to the Node
    /// \param final the final flag of the node, false by default
    ///
    Node(const char unsigned letter, const bool final = false);

    ///
    /// \brief getter for letter private mmeber
    /// \return the letter of Node
    ///
    unsigned char getLetter() const;

    ///
    /// \brief getter for the array of child Nodes
    /// \return a reference to the array of child Nodes
    ///
    const ChildsArray& getChilds() const;

    ///
    /// \brief getter for the array of child Nodes
    /// \return a reference to the array of child Nodes
    ///
    ChildsArray& getChilds();

    ///
    /// \brief Getter for a child Node selected by his letter
    /// \param letter the letter of the child Node
    /// \return a pointer to the selected child Node
    ///
    Node* getChildByLetter(const unsigned char letter) const;

    ///
    /// \brief add a child Node to the current Node
    /// \param newNode the Node to add
    ///
    void addChild(Node* newNode);

    ///
    /// \brief setter for private member final.
    /// \param final the new value of final member.
    ///
    void setFinal(const bool final = true);

    ///
    /// \brief getter for final private member
    /// \return true if Node is final, else false
    ///
    bool isFinal() const;

    ~Node();

    private:

    ///
    /// \brief get the index Of a Node in the ChildsArray of his parent.
    /// \param node the child Node
    /// \return index of the childNode
    ///
    static unsigned int getChildIndex(const Node* node);

    ///
    /// \brief The letter of the Node
    ///
    unsigned char letter;

    ///
    /// \brief The flag wich signals if Node is final
    ///
    bool final;

    ///
    /// \brief an array of child Nodes, of size 27 (26 letters + 1 LINK_LETTER)
    ///
    ChildsArray childs;
};

#endif // NODE_HPP
