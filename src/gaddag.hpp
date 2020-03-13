#ifndef GADDAG_HPP
#define GADDAG_HPP

#include <string>
#include <vector>
#include <memory>

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

        using WordsArray = std::vector<std::string>;

        using WordPair = std::pair<Node*, std::string>;

        std::unique_ptr<WordsArray> getWordsArray(const std::string& word) const;

        Gaddag& addWordPrivate(const std::string& word);

        Node* head;
};

#endif // GADDAG_HPP
