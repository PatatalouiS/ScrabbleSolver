#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <unordered_set>
#include <stack>
#include <optional>

#include "board.hpp"
#include "stroke.hpp"

struct Config {
    PlayerBag playerBag;
    Board board;
};

class Solver {
    public :

        using StrokesSet = std::unordered_set<Stroke>;
        using NeighborsSet = std::unordered_set<SpotPos>;
        struct SearchingParams;

    private:

        static const Gaddag dico;

        enum class PlusStatus {
            USED,
            IN_USE,
            NOT_USED
        };

        static std::pair<std::unique_ptr<StrokesSet>, Stroke> getAvailableStrokes(const Config& conf);

        static std::unique_ptr<NeighborsSet> getNeighBors(const Board& b);

        static std::optional<unsigned int> checkOtherWords(
                             const SearchingParams& params,
                             const unsigned char candidate,
                             const Board& board);

        static SpotPos computeNextPos(const SearchingParams& params);

        static void followForcedRoot(SearchingParams& params,
                              std::stack<SearchingParams>& stack,
                              const Config& config);

        static void followPlusRoot(SearchingParams& params,
                            std::stack<SearchingParams>& stack);

        static void followPlayerBagRoots(SearchingParams& params,
                                  std::stack<SearchingParams>& stack,
                                  const Config& config );

    public:

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
        };

        Solver();

        static Board solveConfig(const Config& config);

        void solveFromScratch();
  };

#endif // SOLVER_HPP
