
#include <chrono>

#include "solver.hpp"
#include "utils.hpp"

using namespace std;

namespace Functors {
    auto plusFunc = [](const char a, const char b) -> char {
        return static_cast<char>(a + b);
    };

    auto minusFunc = [](const char a, const char b) -> char {
        return static_cast<char>(a - b);
    };
}

namespace {

    constexpr unsigned int SCRABBLE_BONUS = 50;

    const Stroke& addStroke(unique_ptr<Solver::StrokesSet>& result,
                          const Solver::SearchingParams& params) {
        unsigned int totalScore = (params.mainScore * params.mainFactor)
                + params.additionnalScore;

        if(params.availableLetters.empty()) {
            totalScore += SCRABBLE_BONUS;
        }

        string regular = Utils::toRegularWord(params.word);

        Stroke toAdd({ regular,
                        params.startPos,
                        params.direction,
                        totalScore });

        return *(result->emplace(toAdd).first);
    }
}

namespace std {
    template<>
    const Stroke& max(const Stroke& a, const Stroke& b) {
        return b.score > a.score
                ? b
                : a;
    }
}

Solver::Solver(Game& game) : _game(game) {
}

const Board& Solver::solveNext() { 
    auto start = chrono::high_resolution_clock::now();
    auto availableStrokes = getAvailableStrokes();
    auto end = chrono::high_resolution_clock::now();

    cout << "Exec Time : " <<  chrono::duration_cast<chrono::milliseconds>(end-start).count() << endl;

//    for(const auto& s : *availableStrokes) {
//        if(s.score > best.score) {
//            best = s;
//        }
//    }

    cout << "The best stroke is : " << availableStrokes.second << endl;

    return _game.board;
}

optional<SpotPos> Solver::computeNextPos(const SearchingParams& params) {
    PlusStatus plusStatus = params.plusStatus;

    const SpotPos& spotUsed = plusStatus == PlusStatus::IN_USE
            ? params.startPos
            : params.position;

    auto op = plusStatus == PlusStatus::NOT_USED
            ? Functors::minusFunc
            : Functors::plusFunc;

    //char criticalIndex;
    char lineModifier;
    char colModifier;

    if(params.direction == Direction::HORIZONTAL) {
        //criticalIndex = spotUsed.indexCol;
        lineModifier = 0;
        colModifier = 1;
    }
    else {
        //criticalIndex = spotUsed.indexLine;
        lineModifier = 1;
        colModifier = 0;
    }

    //char computedIndex = op(criticalIndex, 1);

//    if(!Utils::validIndex(computedIndex)) {
//        return nullopt;
//    }

    return SpotPos(op(spotUsed.indexLine, lineModifier),
                   op(spotUsed.indexCol, colModifier));
}

optional<unsigned int> Solver::checkOtherWords(const SearchingParams &params,
                             const unsigned char candidate) {
    SpotPos copy(params.position);
    Direction direction = params.direction;
    unsigned int score = LetterBag::getLetterPoints(candidate)
            * _game.board(params.position).bonus.letter_factor;
    unsigned int factor = _game.board(params.position).bonus.word_factor;

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
                score += LetterBag::getLetterPoints(nextLetter);
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
                 score += LetterBag::getLetterPoints(nextLetter);
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

    if(orthogonalWord == "RELBBARCS+") {
        cout << "coucou : " << score * factor << endl;
    }

    if(orthogonalWord.size() <= 2) {
        return 0;
    }

    if(_game.dico.searchPrivate(orthogonalWord)) {
        return score * factor;
    }

    return nullopt;
}

pair<unique_ptr<Solver::StrokesSet>, Stroke> Solver::getAvailableStrokes() {
    Stroke bestStroke;
    // vector for pushing valid stokes
    unique_ptr<StrokesSet> array = make_unique<StrokesSet>();
    // array of start cells, to begin serach
    unique_ptr<NeighborsSet> startCellsArray = getNeighBors();
    // stack for store states of research
    stack<SearchingParams> stack;

    for(const SpotPos& position : *startCellsArray) {
        SpotPos startPos(position.indexLine, position.indexCol);
        //cout << "//////////////////////  CASE : " << int(position.indexLine)
        //<< " " << int(position.indexCol) << endl;
        SearchingParams startParams ({
             _game.dico.getHead(),
             startPos,
             startPos,
             _game.playerBag,
             "",
             PlusStatus::NOT_USED,
             Direction::HORIZONTAL,
             0,
             1,
             0
        });

        // push horizontal reqsearch start params
        stack.push(startParams);
        startParams.direction = Direction::VERTICAL;
        // push vertical research start params
        stack.push(startParams);

        while(!stack.empty()) {
            // read top of the stack
            SearchingParams currentParams = stack.top();
            stack.pop();

            if(Utils::validSpot(currentParams.position)) {
                Spot currentSpot = _game.board(currentParams.position);

                // there is no letter in the board at currentSpot
                if(currentSpot.letter == Spot::EMPTY_SPOT) {
                    if(currentParams.node->isFinal()) {
                        bestStroke = max(bestStroke, addStroke(array, currentParams));
                    }

                    followPlayerBagRoots(currentParams, stack);
                    // Use the Plus root if it's not already used
                    if(currentParams.plusStatus == PlusStatus::NOT_USED) {
                        followPlusRoot(currentParams, stack);
                    }
                }
                // There is a letter in the the board at currentSpot
                else {
                    followForcedRoot(currentParams, stack);
                }
            }
            else if(currentParams.plusStatus == PlusStatus::NOT_USED) {
                followPlusRoot(currentParams, stack);
            }
            else if(currentParams.node->isFinal()){
                bestStroke = max(bestStroke, addStroke(array, currentParams));
            }
        }
    }
    return { move(array), bestStroke };
}

void Solver::followPlayerBagRoots(SearchingParams &params,
                                  std::stack<SearchingParams> &stack) {
    Spot& currentSpot = _game.board(params.position);
    PlayerBag currentLetters = params.availableLetters;
    auto newPos = computeNextPos(params);

    // parcours du tableau de lettre possibles
    for(unsigned char letter : params.availableLetters.data()) {
        Node* child = params.node->getChildByLetter(letter);
        // l'enfant associé a la lettre existe
        if(child != Node::NO_NODE) {
            unsigned int score = LetterBag::getLetterPoints(letter)
                * currentSpot.bonus.letter_factor;

            SearchingParams nextParams(params);
            nextParams.node = child;
            nextParams.availableLetters = currentLetters.pop(letter);
            nextParams.word += static_cast<char>(letter);
            nextParams.mainScore += score;
            nextParams.mainFactor *= currentSpot.bonus.word_factor;

            auto additionnalScore = checkOtherWords(params, letter);

            // if orthogonal word not exists or is valid
            if(additionnalScore.has_value()) {
                nextParams.additionnalScore += *additionnalScore;
                nextParams.position = *newPos;
                stack.push(nextParams);
            }
        }
    }
}

void Solver::followPlusRoot(SearchingParams &params,
                            stack<SearchingParams> &stack) {
    Node* linkChild = params.node->getChildByLetter(LINK_LETTER);
    // if PlusRoot exists
    if(linkChild != Node::NO_NODE) {
        params.plusStatus = PlusStatus::IN_USE;
        auto newPos = computeNextPos(params);

        //if(newPos.has_value()) {
            params.node = linkChild;
            params.position = *newPos;
            params.word += static_cast<char>(LINK_LETTER);
            params.plusStatus = PlusStatus::USED;
            stack.push(params);
        //}
    }
}

void Solver::followForcedRoot(SearchingParams &params,
                              stack<SearchingParams>& stack) {
    unsigned char forcedLetter = _game.board(params.position).letter;
    Node* forcedNode = params.node->getChildByLetter(forcedLetter);
    // if ForcedRoot exists
    if(forcedNode != Node::NO_NODE) {
        auto newPos = computeNextPos(params);

        //if(newPos.has_value()) {
            params.node = forcedNode;
            params.position = *newPos;
            params.word += static_cast<char>(forcedLetter);
            params.mainScore += LetterBag::getLetterPoints(forcedLetter);
            stack.push(params);
        //}
    }
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
