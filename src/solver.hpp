#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <stack>

#include <unordered_set>
#include "scrabbleconfig.hpp"
#include "gaddag.hpp"

class Solver { 
    public :

        using StrokesSet = std::unordered_set<Stroke>;
        //using NeighborsSet = std::unordered_set<SpotPos>;
        using NeighborsSet = std::unordered_set<std::pair<SpotPos, Direction>>;

        Solver(const Gaddag& dico, const bool s = false, const bool j = false);

        void solveConfig(const ScrabbleConfig& config);

        void solveFromScratch();

        enum class PlusStatus {
            USED,
            IN_USE,
            NOT_USED
        };

        struct SearchingParams {
            const Node* node;
            SpotPos position;
            SpotPos startPos;
            PlayerBag availableLetters;
            std::string word;
            PlusStatus plusStatus;
            Direction direction;
            unsigned int mainScore;
            unsigned int mainFactor;
            unsigned int additionnalScore;
            unsigned int nbUsedLetters;

            SearchingParams(const Node* n, const SpotPos& start,
                            const PlayerBag& p, const Direction d);
        };

    private:

        std::unique_ptr<std::pair<StrokesSet, Stroke>> getAvailableStrokes(
                            const ScrabbleConfig& conf);

        //std::unique_ptr<NeighborsSet> getStartSpots(const Board& b);

        std::unique_ptr<NeighborsSet> getStartSpots(const Board& board);

        std::optional<unsigned int> checkOtherWords(
                             const SearchingParams& params,
                             const unsigned char candidate,
                             const Board& board);

        SpotPos computeNextPos(const SearchingParams& params);

        void followForcedRoot(SearchingParams& params,
                              std::stack<SearchingParams>& stack,
                              const ScrabbleConfig& config);

        void followPlusRoot(SearchingParams& params,
                            std::stack<SearchingParams>& stack);

        void followPlayerBagRoots(SearchingParams& params,
                                  std::stack<SearchingParams>& stack,
                                  const ScrabbleConfig& config );

        void followJokerRoots(SearchingParams& params,
                               std::stack<SearchingParams>& stack,
                               const ScrabbleConfig& config);

        const Gaddag& dico;

        bool suzette_check;

        bool jokers;
  };

#endif // SOLVER_HPP
