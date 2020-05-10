#ifndef GADDAG_HPP
#define GADDAG_HPP

#include "node.hpp"
#include <memory>
#include <vector>

class Gaddag {

    public:
        // Methods

        Gaddag();

        Gaddag(const std::string fileName);

        ~Gaddag();

        const Node* getHead() const;

        Node* getHead();

        Gaddag& addWord(const std::string& word);

        bool search(const std::string& word) const;

        void print() const;

        // Members

        static constexpr unsigned char HEAD_LETTER = '@';

        static constexpr unsigned char LINK_LETTER = '+';

    private:
        // Type definitions

        using WordsArray = std::vector<std::string>;

        using WordPair = std::pair<Node*, std::string>;

        // Methods

        std::unique_ptr<WordsArray> getWordsArray(const std::string& word) const;

        Gaddag& addWordPrivate(const std::string& word);

        // Members

        Node* head;
};

#endif // GADDAG_HPP
