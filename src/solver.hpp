#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "board.hpp"
#include "stroke.hpp"
#include <vector>
#include <set>
#include <stack>
#include <optional>
#include <iostream>
#include <utility>


class Solver {
    public:

        using StrokesSet = std::unordered_set<Stroke>;
        using NeighborsSet = std::unordered_set<SpotPos>;

        Solver(Game& game);

        const Board& solveNext();

    private:

        enum class PlusStatus {
            USED,
            IN_USE,
            NOT_USED
        };

        struct SearchingParams {
            Node* node;
            SpotPos position;
            SpotPos startPos;
            PlayerBag availableLetters;
            std::string word;
            PlusStatus plusStatus;
            Direction direction;
            unsigned int score;
            unsigned int factor;

//            void print() {
//                std::cout << "value : " << node->getLetter() << std::endl;
//                std::cout << "PosL : " << int(position.indexLine) << " PosC : " << int(position.indexCol) << std::endl;
//                std::cout << "Word : " << word << std::endl;
//                availableLetters.print(); std::cout <<std::endl;
//            }
        };

        Game& _game;

        std::unique_ptr<StrokesSet> getAvailableStrokes();

        std::unique_ptr<NeighborsSet> getNeighBors();

        bool checkOtherWords(const SearchingParams& params,
                             unsigned char candidate);

        std::optional<SpotPos> computeNextPos(const SearchingParams& params);

        void followForcedRoot(const SearchingParams& params,
                              std::stack<SearchingParams>& stack);

        void followPlusRoot(SearchingParams& params,
                            std::stack<SearchingParams>& stack);

        void followPlayerBagRoots(SearchingParams& params,
                                  std::stack<SearchingParams>& stack,
                                  std::unique_ptr<StrokesSet>& result);
  };



#endif // SOLVER_HPP




