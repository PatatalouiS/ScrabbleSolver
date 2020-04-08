#include "solver.hpp"
#include <iostream>
#include "utils.hpp"
#include "letterbag.hpp"
#include <array>
#include <unordered_set>
#include <stack>
#include <sstream>
#include <cassert>

using namespace std;

Solver::Solver(Game& game) : _game(game) {
}

namespace Functors {
    static auto plusFunc = [](char a, char b) -> char {
        return static_cast<char>(a + b);
    };

    static auto minusFunc = [](char a, char b) -> char {
        return static_cast<char>(a - b);
    };
}

const Board& Solver::solveNext() {
    auto availableStrokes = getAvailableStrokes();

    for(const auto& s : *availableStrokes) {
        if(s.word == "ZEBRAT") {
            stringstream o;
            o << s.pos;
            cout << "Word : " << s.word;
            cout << "| Pos : " << int(s.pos.indexLine)<< " , " << int(s.pos.indexCol);
            cout << "| Direction : " << s.direction;
            cout << "| Score : " << s.score << endl;
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

    char criticalIndex;
    char lineModifier;
    char colModifier;

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



    if(!Utils::validIndex(computedIndex)) {
        return nullopt;
    }

    return SpotPos(op(spotUsed.indexLine, lineModifier),
                   op(spotUsed.indexCol, colModifier));
}

bool Solver::checkOtherWords(const SearchingParams &params, unsigned char candidate) {
    SpotPos copy(params.position);
    Direction direction = params.direction;

    string orthogonalWord({ static_cast<char>(candidate) });
    bool stop = false;

    char& criticalIndex = direction == Direction::HORIZONTAL
            ? copy.indexLine
            : copy.indexCol;

    criticalIndex --;

    while(!stop) {
       unsigned char nextLetter;
       if(Utils::validIndex(criticalIndex)) {
            nextLetter = _game.board(copy).letter;
            if(nextLetter != Spot::EMPTY_SPOT) {
                orthogonalWord += static_cast<char>(nextLetter);
                criticalIndex--;
            }
            else {
                stop = true;
            }
       }
       else {
           stop = true;
       }
    }

    orthogonalWord += (static_cast<char>(LINK_LETTER));
    stop = false;

    if(direction == Direction::HORIZONTAL) {
       criticalIndex = params.position.indexLine + 1;
    }
    else {
        criticalIndex = params.position.indexCol + 1;
    }

    while(!stop) {
        unsigned char nextLetter;
        if(Utils::validIndex(criticalIndex)) {
             nextLetter = _game.board(copy).letter;
             if(nextLetter != Spot::EMPTY_SPOT) {
                 orthogonalWord += static_cast<char>(nextLetter);
                 criticalIndex++;
             }
             else {
                 stop = true;
             }
        }
        else {
            stop = true;
        }
    }

//    if(orthogonalWord.size() > 2 && params.position->indexLine == 13) {
//        cout << " OTHER WORDS : " << "Pos : " << *params.position << " Word : " << orthogonalWord << " D : " << params.direction << endl;
//    }

    return orthogonalWord.size() > 2
            ? _game.dico.searchPrivate(orthogonalWord)
            : true;
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
            Direction::HORIZONTAL,
            0,
            1
        },{
           _game.dico.getHead(),
           startPos,
           _game.playerBag,
           "",
           PlusStatus::NOT_USED,
           Direction::VERTICAL,
           0,
           1
        }});

        while(!stack.empty()) {
            // read top of the stack
            SearchingParams current = stack.top();
            Node* currentNode = current.node;
            SpotPos currentPos = current.position;
            PlayerBag currentLetters = current.availableLetters;
            string currentWord = current.word;
            PlusStatus currentPlusStatus = current.plusStatus;
            Direction currentDirection = current.direction;
            Spot currentSpot;
            unsigned int currentFactor = current.factor;
            unsigned int currentScore = current.score;
            stack.pop();

            //// CASE LIBRE, SPOT COURANT LIBRE ////

            currentSpot = _game.board(currentPos);

            if(currentSpot.letter == Spot::EMPTY_SPOT) {

                // Si le noeud est final
                if(currentNode->isFinal()) {
                    string regular = Utils::toRegularWord(currentWord);
                    array->insert({{ regular, startPos, currentDirection,
                                     currentScore * current.factor }});
                }

                auto newPos = computeNextPos(startPos, currentPos, currentPlusStatus, currentDirection);

                // parcours du tableau de lettre possibles
                for(unsigned char letter : currentLetters.data()) {
                    Node* child = currentNode->getChildByLetter(letter);
                    // l'enfant associé a la lettre existe
                    if(child != Node::NO_NODE) {
                        unsigned int score = LetterBag::getLetterPoints(letter)
                            * currentSpot.bonus.letter_factor;

                        if(child->isFinal() && !newPos) {
                            string regular = Utils::toRegularWord(currentWord += static_cast<char>(letter));
                            array->insert({{ regular, startPos, currentDirection,
                                             (currentScore + score) *
                                             (currentFactor * currentSpot.bonus.word_factor) }});
                        }
                        else if(newPos && checkOtherWords(current, letter)) {
                            stack.push({
                                child,
                                *newPos,
                                currentLetters.pop(letter),
                                string(currentWord) += static_cast<char>(letter),
                                currentPlusStatus,
                                currentDirection,
                                currentScore + score,
                                currentFactor * currentSpot.bonus.word_factor
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

                        if(newPos.has_value()) {
                            stack.push({
                                linkChild,
                                *newPos,
                                currentLetters,
                                string(currentWord) += static_cast<char>(LINK_LETTER),
                                PlusStatus::USED,
                                currentDirection,
                                currentScore,
                                currentFactor
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

                    if(newPos.has_value()) {
                        stack.push({
                            forcedNode,
                            *newPos,
                            currentLetters,
                            string(currentWord) += static_cast<char>(forcedLetter),
                            currentPlusStatus,
                            currentDirection,
                            currentScore + LetterBag::getLetterPoints(forcedLetter),
                            currentFactor
                        });
                    }
                }
            }

        }
    }

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
                        set->insert({{ static_cast<char>(indexLine),
                                       static_cast<char>(indexCol) }});
                    }
                }
            }
        }
    }

    return set;
}
