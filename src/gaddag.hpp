#ifndef GADDAG_HPP
#define GADDAG_HPP

#include <string>

#include "node.hpp"

class Gaddag {

    public:

        Gaddag();

        Gaddag(const std::string fileName);

        ~Gaddag();

        const Node* getHead() const;

        Gaddag& addWord(const std::string& word);

        bool search(const std::string& word) const;

        void print() const;

    private:

        using WordPair = std::pair<Node*, std::string>;

        Node* head;

        void addWordPrivate(std::string& word);
};

#endif // GADDAG_HPP
