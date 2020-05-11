#ifndef UTILS_HPP
#define UTILS_HPP

#include "solver.hpp"

///
/// \namespace Utils
/// \brief Wraps All little useful methods tp parse args, check validity
///     of some values, print in console...
///
namespace Utils {

    ///
    /// \brief The Options struct is here just for set the Options of the
    ///     program after a call to Utils::parseArgs(); No constructor, all
    ///     values are false by default, can be easily set with C++
    ///     bracket initialization
    ///
    struct Options {
        ///
        /// \brief the --loop option, Launch Solver::solveFromScratch()
        ///     at launching
        ///
        bool loop = false;
        ///
        /// \brief the --jokers option.
        ///
        bool jokers = false;
        ///
        /// \brief the --suzette-check option. See the README.md (gitlab) to
        ///     enable.
        ///
        bool suzette_check = false;
    };

    ///
    /// \brief a little Method to parse the command line Args.
    /// \param argc The arg count
    /// \param argv The arg list
    /// \return An instance of struct Options sets by the command line args
    ///
    Options parseArgs(int argc, char ** argv);

    ///
    /// \brief compute the index related to a given letter
    ///     for letter 'A' to 'Z', it return 0 to 25.
    /// \param letter the letter to convert in index
    /// \return the conversion of the letter into unsigned int index
    ///
    unsigned int charToIndex(const unsigned char letter);

    ///
    /// \brief compute the letter related to a given index
    ///     for index '0' to '25', it return 'A' to 'Z'.
    /// \param index the index to convert in letter
    /// \return the conversion of the index into letter
    ///
    unsigned char indexTochar(const unsigned int index);

    ///
    /// \brief getLetterPoints compute the score of a letter in
    ///     the Scrabble game.
    /// \param letter the letter for wich we want the score
    /// \return
    ///
    unsigned int getLetterPoints(const unsigned char letter);

    ///
    /// \brief compute the regular word with the word given,
    ///     wich is in Gaddag representation
    /// \param str the Gaddag word to convert to regular word
    /// \return the regular word computed
    ///
    std::string toRegularWord(const std::string& str);

    ///
    /// \brief Check if a given index is Valid in a Scrabble Board
    /// \param index the index to check
    /// \return true if valid, else false
    ///
    bool validIndex(const char index);

    ///
    /// \brief Check if a given SpotPos is Valid in a Scrabble Board
    /// \param spot the SpotPos to check
    /// \return true if valid, else false
    ///
    bool validPos(const SpotPos& spot);

    ///
    /// \brief This method compute some checks to ensure that a ScrabbleConfig
    ///     is Valid (like PlayerBag Size, number of Jokers..)
    /// \param config the ScrabbleConfig to check
    /// \param jokers a flag that signals if jokers are enabled or not.
    /// \return true if valid, else false
    ///
    bool validConfig(const ScrabbleConfig& config, const bool jokers);

    ///
    /// \brief This function compute the SpotPos related to the first letter
    ///     of a Move. Useful for put a Move in the Scrabble Board.
    /// \param move the Move with wich to compute the first letter SpotPos
    /// \return the computed SpotPos
    ///
    SpotPos startPosMove(const Move& move);

    ///
    /// \brief Clear the console.
    ///
    void clearScreen();

    ///
    /// \brief Print the name of the software and add a little bit presentation.
    ///
    void printHeader();

    ///
    /// \brief Print the Options used ( --loop, --jokers --suzette-check)
    /// \param opt the Options to print
    ///
    void printOptions(const Options& opt);

    ///
    /// \brief Wait ENTER key from the User
    ///
    void waitForEnter();

    ///
    /// \brief Encode a string to a valid format. Useful for sending GET Queries
    ///     to Suzette Server with httplib.hpp.
    /// \param str the string to encode
    /// \return the encoded string
    ///
    std::string urlencode(const std::string& str);
};

#endif // UTILS_HPP
