#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "board.hpp"
#include <vector>
#include <set>


class Solver {

    public:

        using StrokesArray = std::set<std::string>;
        using NeighborsArray = std::vector<SpotPos>;

        Solver(Game& game);

        const Board& solveNext();

    private:

        Game& _game;

        std::unique_ptr<StrokesArray> getAvailableStrokes();

        std::unique_ptr<NeighborsArray> getNeighBors();

        enum class PlusStatus {
            USED,
            IN_USE,
            NOT_USED
        };

        bool computeNextPos(
                const SpotPos& start,
                SpotPos& current,
                const PlusStatus& plusStatus
        );

        struct SearchingParams {
            Node* node;
            SpotPos position;
            PlayerBag availableLetters;
            std::string word;
            PlusStatus plusStatus;
        };
};

#endif // SOLVER_HPP


//stack.push({
//    child,

//    current.availableLetters.pop(letter),
//    current.word += static_cast<char>(letter)
//});
