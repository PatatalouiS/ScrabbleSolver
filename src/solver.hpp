#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <stack>

#include <unordered_set>
#include "scrabbleconfig.hpp"
#include "gaddag.hpp"

///
/// \brief The Solver class implements all the magic methods to solve the Best
///     Move in Scrabble Game.
/// \author CERBULEAN Ana-Maria, OLIVIE Maxime
///
class Solver { 

    public :

    ///
    /// \brief Solver The constructor of Solver
    /// \param dico the dictionnary in Gaddag form, used to find the best Moves
    /// \param s enable the Suzette checks. One HTTP get request per Move
    ///     should be emitted. See the README.md to enable this option.
    /// \param j enable the Joker mode
    ///
    Solver(const Gaddag& dico, const bool s = false, const bool j = false);

    ///
    /// \brief solveConfig This method take a ScrabbleConfig, compute the best
    ///     Move and show us to console the result.
    /// \param config the ScrabbleConfig to solve
    ///
    void solveConfig(const ScrabbleConfig& config);

    ///
    /// \brief this method run a Scrabble Simulation from an
    ///     empty Board, until there is no more letter to play or when
    ///     no move has been found
    ///
    void solveFromScratch();

    ///
    /// \brief The PlusStatus enum is an inner-class ennumeration wich represents
    ///     the Status of the '+' ( see Gaddag ) during the Solve proccess.
    ///
    enum class PlusStatus {
        USED,
        IN_USE,
        NOT_USED
    };

    ///
    /// \brief The SearchingParams struct is an inner-class structure wich contains
    ///     all the usefull params to have during the search. It's an inner-class structure
    ///     because this struct is used only during the solve process.
    ///     SearchingParams are stacked and unstacked during the solve process.
    ///     It's more a wrapper than a real structure.
    ///     it allows us to not have methods with a too important number of paramaters
    ///
    struct SearchingParams {

        ///
        /// \brief node the current node in Gaddag dictionary during the search
        ///
        const Node* node;

        ///
        /// \brief the current position on the Board during the search
        ///
        SpotPos position;

        ///
        /// \brief the Start position on the board during the search
        ///
        SpotPos startPos;

        ///
        /// \brief the current PlayerBag during the search
        ///     ( The letter of a player )
        ///
        PlayerBag availableLetters;

        ///
        /// \brief the current word constructed during the search.
        ///     the string is in Gaddag representation (with the '+')
        ///
        std::string word;

        ///
        /// \brief the Sttaus of the '+' during the search ( see Gaddag )
        ///
        PlusStatus plusStatus;

        ///
        /// \brief The direction of the search (Horizontal or Vertical)
        ///
        Direction direction;

        ///
        /// \brief The main score during the search? it doesn't contains
        ///     the score of additional words possibly found. It doesn't contains
        ///     the word_factor bonus too
        ///
        unsigned int mainScore;

        ///
        /// \brief the multiplication factor of the main Word during the search.
        ///
        unsigned int mainFactor;

        ///
        /// \brief contains all the additional score of orthogonal words found
        ///     during the search
        ///
        unsigned int additionnalScore;

        ///
        /// \brief nbUsedLetters
        ///
        unsigned int nbUsedLetters;

        /// \brief SearchingParams constructor, used just for construct less
        ///     verbosely the new SearchingParams than C++ bracket initialization
        /// \param n the Node in Gaddag from wich we are going to launch the search
        ///     Typically, the head of a Gaddag
        /// \param start the StartPos on Board from wich we are going to launch the search
        ///     Typically, one of the SpotPos computed by getStartSpots() method.
        /// \param p The PlayerBag used to search the best Move
        /// \param d The direction of the search
        ///
        SearchingParams(const Node* n, const SpotPos& start,
                        const PlayerBag& p, const Direction d);
    };

    private:

    using MovesSet = std::unordered_set<Move>;
    using StartPosSet = std::unordered_set<std::pair<SpotPos, Direction>>;

    ///
    /// \brief all the Magic is in this method. It Takes a Scrabble Config
    ///     (see Board and PlayerBag ) compute all the possibles Moves, and
    ///     the best move too
    /// \param conf The ScrabbleConfig to solve
    /// \return a pointer to a pair. first of the pair is a set of all the possible strokes.
    ///     second of the pair is the Best Move.
    ///
    std::unique_ptr<std::pair<MovesSet, Move>> getAvailableMoves(
                        const ScrabbleConfig& conf);

    ///
    /// \brief This Method compute the set of SpotPos from wich
    ///     we will launch the research. We avec code a more powerfull algorithm than
    ///     the easy way of the statement ( The easy way consists to select all the
    ///     empty Spots near another non-empty spot).
    ///     This algorithm is detailed in our rapport.
    /// \param board the Board in wich the start cells will be computed
    /// \return
    ///
    static std::unique_ptr<StartPosSet> getStartSpots(const Board& board);

    ///
    /// \brief this method Compute the next SpotPos to visit during the search process
    ///     in function of the SearchingParams given.
    /// \param params the SearchingParams used to compute the next position on the Board.
    /// \return the computed next position. Warning : this SpotPos could be valid or not.
    ///     You can check the validity with Utils::validPos()
    ///
    static SpotPos computeNextPos(const SearchingParams& params);

    ///
    /// \brief This method check if other orthogonal words exists
    ///     during the search process and if they are valid.
    /// \param params The actual SearchingParams
    /// \param candidate the letter we want to play, but who could build another word
    /// \param board The ScrabbleBoard used.
    /// \param jokerUsed a flag to signal if the candidate letter is played as a joker
    /// \return an optional type. if a bad orthogonal word is detected, we return nullopt.
    ///     else, if no orthogonal word is detected or if the orthogonal word is valid, returns
    ///     the additionnal score of this orthogonal word. (0 is no word detected).
    ///
    std::optional<unsigned int> checkOtherWords(
                         const SearchingParams& params,
                         const unsigned char candidate,
                         const Board& board,
                         const bool jokerUsed = false);
    ///
    /// \brief this Method is used to follow a forced root during the search process
    ///     (When current SpotPos is not an empty Spot)
    /// \param params The actual SearchingParams
    /// \param stack the stack of SearchingParams (the key element of the algorithm)
    /// \param config The ScrabbleConfig to solve
    ///
    void followForcedRoot(SearchingParams& params,
                          std::stack<SearchingParams>& stack,
                          const ScrabbleConfig& config);
    ///
    /// \brief this Method is used to follow a '+' root in our gaddag dictionary.
    /// \param params The actual SearchingParams
    /// \param stack the stack of SearchingParams (the key element of the algorithm)
    ///
    void followPlusRoot(SearchingParams& params,
                        std::stack<SearchingParams>& stack);
    ///
    /// \brief this Method is used to follow all the possible roots wich
    ///     correspond to the letters available in the PlayberBag
    /// \param params The actual SearchingParams
    /// \param stack the stack of SearchingParams (the key element of the algorithm)
    /// \param config The ScrabbleConfig to solve
    ///
    void followPlayerBagRoots(SearchingParams& params,
                              std::stack<SearchingParams>& stack,
                              const ScrabbleConfig& config );
    ///
    /// \brief This Method is used to follow a Joker Root. It used only when
    ///     the jokers option is set to true. It tries all the letters of the
    ///     Alphabet. See the Rules of the Scrabble for more details.
    /// \param params The actual SearchingParams
    /// \param stack the stack of SearchingParams (the key element of the algorithm)
    /// \param config The ScrabbleConfig to solve
    ///
    void followJokerRoots(SearchingParams& params,
                           std::stack<SearchingParams>& stack,
                           const ScrabbleConfig& config);
    ///
    /// \brief The Gaddag dictionary used by the Solver
    ///
    const Gaddag& dico;

    ///
    /// \brief enables the suzette-checks on the university server.
    ///     One HTTP get request per Move should be emitted.
    ///     See the README.md to enable this option.
    ///
    bool suzette_check;

    ///
    /// \brief enables the Joker mode.
    ///
    bool jokers;
};

#endif // SOLVER_HPP
