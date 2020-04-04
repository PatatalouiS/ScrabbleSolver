#ifndef GADDAG_HPP
#define GADDAG_HPP

#include <string>
#include <vector>
#include <memory>
#include <optional>

#include "node.hpp"

static constexpr unsigned char HEAD_LETTER = '@';
static constexpr unsigned char LINK_LETTER = '+';

class Gaddag {

    public:

        Gaddag();

        Gaddag(const std::string fileName);

        ~Gaddag();

        const Node* getHead() const;

        Node* getHead();

        Gaddag& addWord(const std::string& word);

        std::unique_ptr<
            std::vector<
                std::string>> containsArray(const unsigned char c) const;

        bool search(const std::string& word) const;

        bool searchPrivate(const std::string& word, Node* start = nullptr) const;

        void print() const;

    private:

        using WordsArray = std::vector<std::string>;

        using WordPair = std::pair<Node*, std::string>;

        std::unique_ptr<WordsArray> getWordsArray(const std::string& word) const;

        Gaddag& addWordPrivate(const std::string& word);

        Node* head;
};

#endif // GADDAG_HPP
