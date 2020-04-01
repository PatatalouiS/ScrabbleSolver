#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "board.hpp"
#include <vector>
#include <set>
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

        Game& _game;

        std::unique_ptr<StrokesSet> getAvailableStrokes();

        std::unique_ptr<NeighborsSet> getNeighBors();

        enum class PlusStatus {
            USED,
            IN_USE,
            NOT_USED
        };

        enum class Direction {
            HORIZONTAL,
            VERTICAL
        };

        std::optional<SpotPos> computeNextPos(
                const SpotPos& start,
                const SpotPos& current,
                const PlusStatus& plusStatus
        );

        struct SearchingParams {
            Node* node;
            SpotPos position;
            PlayerBag availableLetters;
            std::string word;
            PlusStatus plusStatus;

            void print() {
                std::cout << "value : " << node->getLetter() << std::endl;
                std::cout << "PosL : " << int(position.indexLine) << " PosC : " << int(position.indexCol) << std::endl;
                std::cout << "Word : " << word << std::endl;
                availableLetters.print(); std::cout <<std::endl;
            }
        };     
  };

#endif // SOLVER_HPP
