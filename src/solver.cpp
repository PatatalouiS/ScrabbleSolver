#include "solver.hpp"
#include <iostream>
#include "utils.hpp"
#include <array>
#include <unordered_set>
#include <stack>
#include <sstream>

using namespace std;

Solver::Solver(Game& game) : _game(game) {
}

namespace Functors {
    static auto plusFunc = [](unsigned char a, unsigned char b) -> char {
        return static_cast<char>(a + b);
    };

    static auto minusFunc = [](unsigned char a, unsigned char b) -> char {
        return static_cast<char>(a - b);
    };
}

const Board& Solver::solveNext() {
    auto availableStrokes = getAvailableStrokes();

    for(const auto& s : *availableStrokes) {

        stringstream o;
        o << s.pos;

        if(s.word == "JOUEZ") {
            cout << "Word : " << s.word;
            cout << "| Pos : " << int(s.pos.indexLine)<< " , " << int(s.pos.indexCol);
            cout << "| Direction : " << s.direction << endl;
        }
    }

    return _game.board;
}

optional<SpotPos> Solver::computeNextPos(const SpotPos &start,
                            const SpotPos &current,
                            const PlusStatus& plusStatus,
                            const Direction direction) {

    const SpotPos& spotUsed = plusStatus == PlusStatus::IN_USE
            ? start
            : current;

    auto op = plusStatus == PlusStatus::NOT_USED
            ? Functors::minusFunc
            : Functors::plusFunc;

    unsigned char criticalIndex;
    unsigned char lineModifier;
    unsigned char colModifier;

    if(direction == Direction::HORIZONTAL) {
        criticalIndex = spotUsed.indexCol;
        lineModifier = 0;
        colModifier = 1;
    }
    else {
        criticalIndex = spotUsed.indexLine;
        lineModifier = 1;
        colModifier = 0;
    }

    char computedIndex = op(criticalIndex, 1);

    if((unsigned(computedIndex) >= Board::SIZE) || (computedIndex < 0)) {
        return nullopt;
    }

    return SpotPos(static_cast<unsigned char>((op(start.indexLine, lineModifier))),
                   static_cast<unsigned char>((op(start.indexCol, colModifier))));
}

unique_ptr<Solver::StrokesSet> Solver::getAvailableStrokes() {
    // vector for pushing valid stokes
    unique_ptr<StrokesSet> array = make_unique<StrokesSet>();
    // array of start cells, to begin serach
    unique_ptr<NeighborsSet> startCellsArray = getNeighBors();

    for(const SpotPos& position : *startCellsArray) {
        SpotPos startPos(position.indexLine, position.indexCol);

        cout << "//////////////////////  CASE : " << int(position.indexLine) << " " << int(position.indexCol) << endl;

        stack<SearchingParams> stack ({{
            _game.dico.getHead(),
            startPos,
            _game.playerBag,
            "",
            PlusStatus::NOT_USED,
            Direction::HORIZONTAL
        },{
           _game.dico.getHead(),
           startPos,
           _game.playerBag,
           "",
           PlusStatus::NOT_USED,
           Direction::VERTICAL
        }});

        while(!stack.empty()) {
            // read top of the stack
            SearchingParams current = stack.top();
            //current.print();
            Node* currentNode = current.node;
            SpotPos currentPos = current.position;
            Spot currentSpot = _game.board(currentPos);
            PlayerBag currentLetters = current.availableLetters;
            string currentWord = current.word;
            PlusStatus currentPlusStatus = current.plusStatus;
            Direction currentDirection = current.direction;
            stack.pop();

            //// CASE LIBRE, SPOT COURANT LIBRE ////

            if(currentSpot.letter == Spot::EMPTY_SPOT) {

                // SI c'est un noeud final, on ajoute le mot
                if(currentNode->isFinal()) {
                    string regular = Utils::toRegularWord(currentWord);
                    array->insert({{ regular, startPos, currentDirection }});
                }

                auto newPos = computeNextPos(startPos, currentPos, currentPlusStatus, currentDirection);
                // Si la prochaine position de curseur est valide
                if(newPos) {
                    // parcours du tableau de lettre possibles
                    for(unsigned char letter : currentLetters.data()) {
                        Node* child = currentNode->getChildByLetter(letter);

                        // l'enfant associé a la lettre existe
                        if(child != Node::NO_NODE) {
                            // On insère
                            stack.push({
                                child,
                                *newPos,
                                currentLetters.pop(letter),
                                string(currentWord) += static_cast<char>(letter),
                                currentPlusStatus,
                                currentDirection
                            });
                        }
                    }
                }

                // si le plus n'a pas été utilisé
                if(currentPlusStatus == PlusStatus::NOT_USED) {
                    Node* linkChild = currentNode->getChildByLetter(LINK_LETTER);
                    // et qu'il n'est pas nullptr
                    if(linkChild != Node::NO_NODE) {
                        // et que la position générée est valide, on l'ajoute
                        auto newPos = computeNextPos(startPos, currentPos, PlusStatus::IN_USE, currentDirection);

                        if(newPos) {
                            stack.push({
                                linkChild,
                                *newPos,
                                currentLetters,
                                string(currentWord) += static_cast<char>(LINK_LETTER),
                                PlusStatus::USED,
                                currentDirection
                            });
                        }
                    }
                }
            }
            //// CASE OCCUPEE, SPOT PRIS ////

            else {
                unsigned char forcedLetter = currentSpot.letter;
                Node* forcedNode = currentNode->getChildByLetter(forcedLetter);
                // lettre forcée compatible dans le Gaddagg
                if(forcedNode != Node::NO_NODE) {

                    auto newPos = computeNextPos(startPos, currentPos, currentPlusStatus, currentDirection);
                    // si la position générée est valide, on avance
                    if(newPos) {
                        stack.push({
                            forcedNode,
                            *newPos,
                            currentLetters,
                            string(currentWord) += static_cast<char>(forcedLetter),
                            currentPlusStatus,
                            currentDirection
                        });
                    }
                }
            }
        } // while stack
    } // loop on each empty

    return array;
}


unique_ptr<Solver::NeighborsSet> Solver::getNeighBors() {
    unique_ptr set = make_unique<NeighborsSet>();

    for(unsigned char line = 0; line < Board::SIZE; line++) {
        for(unsigned char col = 0; col < Board::SIZE; col++) {
            const Spot current = _game.board(line, col);
            if(current.letter != 0) {

                const std::array<SpotPos, 4> neighborsToTest ({
                    SpotPos((line - 1) < 0 ? 0 : (line - 1), col),
                    SpotPos((line + 1) >=15 ? 14 : (line + 1), col),
                    SpotPos(line, (col + 1) >= 15 ? 14 : (col + 1)),
                    SpotPos(line, (col - 1) < 0 ? 0 : (col -1))
                });

                for(const SpotPos& s : neighborsToTest) {
                    const unsigned char indexLine = s.indexLine;
                    const unsigned char indexCol = s.indexCol;

                    if(_game.board(indexLine, indexCol).letter == Spot::EMPTY_SPOT) {
                        set->insert({{ indexLine, indexCol }});
                    }
                }
            }
        }
    }

    return set;
}
