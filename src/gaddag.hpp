#ifndef GADDAG_HPP
#define GADDAG_HPP

#include "node.hpp"
#include <memory>
#include <vector>

///
/// \brief The Gaddag class represents a Dictionnary, It's a tree structure
///     that allows us to find Scrabble Moves. It's a little similar to a Trie prefix tree.
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
///

class Gaddag {

    public:

    ///
    /// \brief Gaddag Default constructor. construct an empty tree, without any words.
    ///
    Gaddag();

    ///
    /// \brief Gaddag Construct a gaddagg with the file path given.
    /// \param fileName A path to a dictionnary file.
    ///
    Gaddag(const std::string fileName);

    ~Gaddag();

    ///
    /// \brief accessor the head of the Gaddag
    /// \return the head of the Gaddag
    ///
    const Node* getHead() const;

    ///
    /// \brief return the head of the Gaddag
    /// \return the head of the Gaddag
    ///
    Node* getHead();

    ///
    /// \brief Add a new word to the Gaddag, do nothing if empty word or
    ///     if given word already exists.
    /// \param word the word to add
    /// \return a reference to the Gaddagg, for chaining.
    ///
    Gaddag& addWord(const std::string& word);

    ///
    /// \brief search if a given word exists.
    ///     The word must be in Gaddag Representation (with +)
    /// \param word the word to search
    /// \return true if word exists, else false
    ///
    bool search(const std::string& word) const;

    ///
    /// \brief HEAD_LETTER constant represents the symbol of the head node.
    ///
    static constexpr unsigned char HEAD_LETTER = '@';

    ///
    /// \brief LINK_LETTER constant represents the + Symbol in gaddag, this
    ///     symbol represents the separation between the reverse part and
    ///     the normal part of a word in the Gaddag.
    ///
    static constexpr unsigned char LINK_LETTER = '+';

    private:

    using WordsArray = std::vector<std::string>;
    using WordPair = std::pair<Node*, std::string>;

    ///
    /// \brief getWordsArray calculates all the words that must be inserted
    ///     during the addWord function. (all the words with LINK_LETTER)
    /// \param word the word to add (in regular form)
    /// \return a pointer to a string array. It contains all the gaddag representations
    ///     of the regular world given.
    ///
    std::unique_ptr<WordsArray> getWordsArray(const std::string& word) const;

    ///
    /// \brief addWordPrivate is called internally on each call to addWord.
    ///     in fact, this method add all the words computed by getWordsArray().
    /// \param word The word to add
    /// \return a reference to the Gaddag, for chaining
    ///
    Gaddag& addWordPrivate(const std::string& word);

    ///
    /// \brief head the head Node of the Gaddag
    ///
    Node* head;
};

///
/// \brief operator << allows to print a gaddagg on console. for debug.
/// \param out
/// \param g
/// \return
///
std::ostream& operator<<(std::ostream& out, const Gaddag& g) ;

#endif // GADDAG_HPP
