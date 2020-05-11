#ifndef LETTERBAG_HPP
#define LETTERBAG_HPP

#include <vector>
#include <random>

///
/// \brief JOKER_SYMBOL constant wich store the representation of a Joker in Scrabble
///     Here, this is the '?' symbol.
///
constexpr unsigned char JOKER_SYMBOL = '?';

///
/// \brief The LetterBag class represent the main bag of letters in Scrabble Game.
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
///
class LetterBag {

    public :

    ///
    /// \brief LetterBag constructor, the use of Jokers is off by default.
    /// \param useJokers indicates if we use Jokers or not
    ///
    LetterBag(const bool useJokers = false);

    ///
    /// \brief pickRandomLetter remove a random letter present in the LetterBag.
    ///     O(1) complexity.
    /// \return the removed letter
    ///
    unsigned char pickRandomLetter();

    ///
    /// \brief getNbLetters return the number of letters remaining in the LetterBag.
    /// \return the number of letters remianing in the LetterBag.
    ///
    unsigned int getNbLetters() const;

    ///
    /// \brief isEmpty check if LetterBag is empty
    /// \return true if empty, else false
    ///
    bool isEmpty() const;

    ///
    /// \brief data allows access to the array of letter
    /// \return a reference to the std::vector of letters
    ///
    const std::vector<unsigned char>& data() const;

    private :

    ///
    /// \brief NB_SYMBOLS is number of letters in the Alphabet.
    ///     Set to 26 in the French Alphabet.
    ///
    static const unsigned int NB_SYMBOLS = 26;

    ///
    /// \brief NB_JOKERS is the number of Jokers in a Scrabble Game.
    ///     Set to 2 Jokers.
    ///
    static const unsigned int NB_JOKERS = 2;

    ///
    /// \brief nbLetters the number of letters in the LetterBag.
    ///     Warning : It's not equal the size of the bag private member !
    ///
    unsigned int nbLetters = 0;

    ///
    /// \brief bag A vector of char wich contains all our letters.
    ///
    std::vector<unsigned char> bag;

    ///
    /// \brief generator a Mersenne Twister generator,
    /// especially to take a random Letter with an uniform distribution.
    ///
    static std::mt19937 generator;
};

///
/// \brief operator << print the LetterBag content
/// \param out
/// \param lb
/// \return
///
std::ostream& operator<<(std::ostream& out, const LetterBag& lb);

#endif // LETTERBAG_HPP




