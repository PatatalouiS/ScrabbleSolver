#ifndef PLAYERBAG_HPP
#define PLAYERBAG_HPP

#include <array>
#include "letterbag.hpp"

///
/// \brief The PlayerBag class represents the letters of a player in the
///     Scrabble Game
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
///
class PlayerBag {

    using PlayerLetters = std::vector<unsigned char>;

    public:

    ///
    /// \brief PlayerBag default constructor, It constructs an empty PlayerBag
    ///
    PlayerBag();

    ///
    /// \brief PlayerBag constructor, fill the new Playerbag with letters from
    ///     a LetterBag, the main bag in Scrabble Game.
    /// \param mainLetterBag the mainLetterBag in wich to take the letters
    ///
    PlayerBag(LetterBag& mainLetterBag);

    ///
    /// \brief PlayerBag copy constructor
    /// \param bag the PlayerBag to clone
    ///
    PlayerBag(const PlayerBag& bag);

    ///
    /// \brief PlayerBag constructor wich take an array of letters
    /// \param letters an array of letters to construct the PlayerBag (MAX 7)
    ///
    PlayerBag(const PlayerLetters& letters);

    ///
    /// \brief getter for the private member playerLetters
    /// \return a reference to the array of letters of the PlayerBag
    ///
    const PlayerLetters& data() const;

    ///
    /// \brief getter for the private member playerLetters
    /// \return a reference to the array of letters of the PlayerBag
    ///
    PlayerLetters& data();

    ///
    /// \brief isEmpty checks if PlayerBag is empty
    /// \return true if empty, else false
    ///
    bool isEmpty() const;

    ///
    /// \brief size returns the size of the PlayerBag
    /// \return the size
    ///
    unsigned long size() const;

    ///
    /// \brief Method for fill the current PlayerBag with a main LetterBag
    ///     (see Scrabble rules)
    /// \param mainLetterBag the main LetterBag in wich to take the letters
    /// \return a reference to the current PlayerBag, now filled
    ///
    PlayerBag& fillWith(LetterBag& mainLetterBag);

    ///
    /// \brief This method return a new PlayerBag without the given letter.
    ///     current PlayerBag isn't modified. The letter must be present in the
    ///     Bag !
    /// \param letter the letter to remove in the returned PlayerBag
    /// \return a New PlayerBag without ONE given letter
    ///
    PlayerBag pop(const unsigned char letter) const;

    ///
    /// \brief this constant sets the Maximum number of letters a player can have
    ///     in his PlayerBag. In Scrabble, this number is 7
    ///
    static constexpr unsigned int MAX_SIZE = 7;

    private :

    ///
    /// \brief The array for store the letters of a player
    ///
    PlayerLetters playerLetters;
};

///
/// \brief operator << Prints the PlayerBag to console
/// \param stream
/// \param pb
/// \return
///
std::ostream& operator << (std::ostream& stream, const PlayerBag& pb);

#endif // PLAYERBAG_HPP
