#include "solver.hpp"
#include <iostream>
#include "utils.hpp"
#include <array>
#include <unordered_set>
#include <stack>

using namespace std;

Solver::Solver(Game& game) : _game(game) {
}


const Board& Solver::solveNext() {
    auto availableStrokes = getAvailableStrokes();

    for(const auto& s : *availableStrokes) {
        cout << " Word : " << s.word << " Pos : " << int(s.pos.indexLine) << " , " << int(s.pos.indexCol) << endl;
    }

    return _game.board;
}

optional<SpotPos> Solver::computeNextPos(const SpotPos &start,
                            const SpotPos &current,
                            const PlusStatus& plusStatus) {
    if(plusStatus == PlusStatus::IN_USE) {
        if((start.indexCol + 1) >= Board::SIZE) {
            return nullopt;
        }
        return SpotPos(start.indexLine, start.indexCol + 1);
    }

    if(plusStatus == PlusStatus::NOT_USED) {
        if((current.indexCol - 1) < 0) {
            return nullopt;
        }
        return SpotPos(current.indexLine, current.indexCol - 1);
    }
    else {
        if((current.indexCol + 1) >= Board::SIZE) {
            return nullopt;
        }
        return SpotPos(current.indexLine, current.indexCol + 1);
    }
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
            PlusStatus::NOT_USED
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
            stack.pop();

            //// CASE LIBRE, SPOT COURANT LIBRE ////

            if(currentSpot.letter == Spot::EMPTY_SPOT) {

                // SI c'est un noeud final, on ajoute le mot
                if(currentNode->isFinal()) {
                    string regular = Utils::toRegularWord(currentWord);
                    array->insert({{ regular, startPos }});
                }

                auto newPos = computeNextPos(startPos, currentPos, currentPlusStatus);
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
                                currentPlusStatus
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
                        auto newPos = computeNextPos(startPos, currentPos, PlusStatus::IN_USE);

                        if(newPos) {
                            stack.push({
                                linkChild,
                                *newPos,
                                currentLetters,
                                string(currentWord) += static_cast<char>(LINK_LETTER),
                                PlusStatus::USED
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

                    auto newPos = computeNextPos(startPos, currentPos, currentPlusStatus);
                    // si la position générée est valide, on avance
                    if(newPos) {
                        stack.push({
                            forcedNode,
                            *newPos,
                            currentLetters,
                            string(currentWord) += static_cast<char>(forcedLetter),
                            currentPlusStatus
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
