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

    for(auto& word : *availableStrokes) {
        cout << word << endl;
    }


    return _game.board;
}

bool Solver::computeNextPos(const SpotPos &start,
                            SpotPos &current,
                            const PlusStatus& plusStatus) {
    if(plusStatus == PlusStatus::IN_USE) {
        if((start.indexCol + 1) >= Board::SIZE) {
            return false;
        }

        current.indexCol = start.indexCol + 1;
        current.indexLine = start.indexLine;
        return true;
    }

    if(plusStatus == PlusStatus::NOT_USED) {
        if((current.indexCol - 1) < 0) {
            return false;
        }

        current.indexCol--;
        return true;
    }
    else {
        if((current.indexCol + 1) >= Board::SIZE) {
            return false;
        }

        current.indexCol++;
        return true;
    }
}

unique_ptr<Solver::StrokesArray> Solver::getAvailableStrokes() {
    // vector for pushing valid stokes
    unique_ptr<StrokesArray> array = make_unique<StrokesArray>();
    // array of start cells, to begin serach
    unique_ptr<NeighborsArray> startCellsArray = getNeighBors();

    for(SpotPos& position : *startCellsArray) {
        SpotPos startPos(position.indexLine, position.indexCol);

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
                    array->insert(currentWord);
                }

                // parcours du tableau de lettre possibles
                for(unsigned char letter : currentLetters.data()) {
                    Node* child = currentNode->getChildByLetter(letter);

                    // l'enfant associé a la lettre existe
                    if(child != Node::NO_NODE) {

                        // Si prochaine position valide, On insère dans la pile
                        if(computeNextPos(startPos, currentPos, currentPlusStatus)) {
                            stack.push({
                                child,
                                currentPos,
                                currentLetters.pop(letter),
                                currentWord += static_cast<char>(letter),
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
                        // et que l aposition générée est valide, on l'ajoute
                        if(computeNextPos(startPos, currentPos, PlusStatus::IN_USE)) {
                            stack.push({
                                linkChild,
                                currentPos,
                                currentLetters,
                                currentWord += static_cast<char>(LINK_LETTER),
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

                    // si la position générée est valide, on avance
                    if(computeNextPos(startPos, currentPos, currentPlusStatus)) {
                        stack.push({
                            forcedNode,
                            currentPos,
                            currentLetters,
                            currentWord += static_cast<char>(forcedLetter),
                            currentPlusStatus
                        });
                    }
                }
            }
        } // while stack
    } // loop on each empty

    return array;
}



unique_ptr<Solver::NeighborsArray> Solver::getNeighBors() {
    static const auto comparator = [](SpotPos a, SpotPos b) -> bool {
        return (a.indexLine != b.indexLine) || (a.indexCol != b.indexCol);
    };

    set<SpotPos, decltype(comparator)> set(comparator);

    for(unsigned char line = 0; line < Board::SIZE; line++) {
        for(unsigned char col = 0; col < Board::SIZE; col++) {
            Spot current = _game.board(line, col);
            if(current.letter != 0) {

                std::array<SpotPos, 4> neighborsToTest ({
                    SpotPos((line - 1) < 0 ? 0 : (line - 1), col),
                    SpotPos((line + 1) >=15 ? 14 : (line + 1), col),
                    SpotPos(line, (col + 1) >= 15 ? 14 : (col + 1)),
                    SpotPos(line, (col - 1) < 0 ? 0 : (col -1))
                });

                for(SpotPos& s : neighborsToTest) {
                    unsigned char indexLine = s.indexLine;
                    unsigned char indexCol = s.indexCol;

                    if(_game.board(indexLine, indexCol).letter == Spot::EMPTY_SPOT) {
                        //cout << int(indexLine) << " , " << int(indexCol) << endl;
                        set.insert({{ indexLine, indexCol }});
                    }
                }
            }
        }
    }

    return make_unique<NeighborsArray>(set.begin(), set.end());
}
