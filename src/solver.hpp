#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <unordered_set>
#include <stack>
#include <optional>

#include "board.hpp"
#include "stroke.hpp"

class Solver {

    public :

        using StrokesSet = std::unordered_set<Stroke>;
        using NeighborsSet = std::unordered_set<SpotPos>;
        struct SearchingParams;

    private:

        enum class PlusStatus {
            USED,
            IN_USE,
            NOT_USED
        };

        Game& _game;

        std::unique_ptr<StrokesSet> getAvailableStrokes();

        std::unique_ptr<NeighborsSet> getNeighBors();

        std::optional<unsigned int> checkOtherWords(
                             const SearchingParams& params,
                             const unsigned char candidate);

        std::optional<SpotPos> computeNextPos(const SearchingParams& params);

        void followForcedRoot(SearchingParams& params,
                              std::stack<SearchingParams>& stack);

        void followPlusRoot(SearchingParams& params,
                            std::stack<SearchingParams>& stack);

        void followPlayerBagRoots(SearchingParams& params,
                                  std::stack<SearchingParams>& stack,
                                  std::unique_ptr<StrokesSet>& result);

    public:

        struct SearchingParams {
            Node* node;
            SpotPos position;
            SpotPos startPos;
            PlayerBag availableLetters;
            std::string word;
            PlusStatus plusStatus;
            Direction direction;
            unsigned int mainScore;
            unsigned int mainFactor;
            unsigned int additionnalScore;
        };

        Solver(Game& game);

        const Board& solveNext();
  };

#endif // SOLVER_HPP

//            void print() {
//                std::cout << "value : " << node->getLetter() << std::endl;
//                std::cout << "PosL : " << int(position.indexLine) << " PosC : " << int(position.indexCol) << std::endl;
//                std::cout << "Word : " << word << std::endl;
//                availableLetters.print(); std::cout <<std::endl;
//            }



