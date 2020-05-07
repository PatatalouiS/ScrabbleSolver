
#include <chrono>
#include <sstream>
#include <fstream>

#include "utils.hpp"
#include "globals.hpp"
#include "suzettecheck.hpp"

using namespace std;

const Gaddag Solver::dico(DICO_PATH);

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

    const Stroke& max(const Stroke& a, const Stroke& b) {
        return b.score > a.score
            ? b
            : a;
    }

    const Stroke& addStroke(unique_ptr<Solver::StrokesSet>& result,
                          const Solver::SearchingParams& params) {
        unsigned int totalScore = (params.mainScore * params.mainFactor)
                + params.additionnalScore;

        if(params.nbUsedLetters == 7) {
            totalScore += SCRABBLE_BONUS;
        }

        string regular = Utils::toRegularWord(params.word);

        Stroke toAdd({ params.word,
                        params.startPos,
                        params.direction,
                        PlayerBag(params.availableLetters.data()),
                        totalScore });

        return *(result->emplace(toAdd).first);
    }
}

Solver::Solver() {}

Board Solver::solveConfig(const Config& config) {
    cout << config.board << endl;
    auto availableStrokes = getAvailableStrokes(config);
    Stroke bestStroke = availableStrokes.second;

    cout << "--- BEST STROKE ---" << endl
         << "With letters : " << config.playerBag << endl
         << "We can play Word  : " << Utils::toRegularWord(bestStroke.word)
         << endl << "With a score of : " << bestStroke.score << " pts." << endl;
        Suzette::check(config.board, config.playerBag);

    return Board(config.board).putStroke(bestStroke);
}

void Solver::solveFromScratch() {
    LetterBag mainBag;
    unsigned int totalScore = 0;

    Config config {
        PlayerBag(mainBag),
        Board()
    };

    while(!mainBag.isEmpty() || !config.playerBag.isEmpty()) {
        auto availableStrokes = getAvailableStrokes(config);
        Stroke bestStroke = availableStrokes.second;
        totalScore += bestStroke.score;
        PlayerBag lastPlayerBag(config.playerBag);
        Suzette::check(config.board, config.playerBag);
        config.playerBag = PlayerBag(bestStroke.playerBag.fillWith(mainBag));

        cout << "--- BEST STROKE ---" << endl
             << "With letters : " << lastPlayerBag << endl
             << "We can play Word  : " << Utils::toRegularWord(bestStroke.word)
             << endl << "Gaddag style : " << bestStroke.word << endl
             << "With a score of : " << bestStroke.score << " pts." << endl
             << "Total Score " << totalScore << endl
             << "Letters remaining : " << mainBag.getNbLetters() << endl
             << "Strokes founded : " << availableStrokes.first->size() << endl
             << "New PlayerBag : " << config.playerBag << endl;

        config.board.putStroke(bestStroke);
        cout << config.board << endl;

        ofstream file("./data/lastStroke");
        config.board.save(file);
        file.close();

        string wait;

        getline(cin, wait);
    }

    cout << endl << "There is no more Stroke to play ! End of the series" << endl;
}

SpotPos Solver::computeNextPos(const SearchingParams& params) {
    PlusStatus plusStatus = params.plusStatus;

    const SpotPos& spotUsed = plusStatus == PlusStatus::IN_USE
            ? params.startPos
            : params.position;

    auto op = plusStatus == PlusStatus::NOT_USED
            ? Functors::minusFunc
            : Functors::plusFunc;

    char lineModifier;
    char colModifier;

    if(params.direction == Direction::HORIZONTAL) {
        lineModifier = 0;
        colModifier = 1;
    }
    else {
        lineModifier = 1;
        colModifier = 0;
    }

    return SpotPos{ op(spotUsed.indexLine, lineModifier),
                    op(spotUsed.indexCol, colModifier) };
}

optional<unsigned int> Solver::checkOtherWords(const SearchingParams &params,
                                               const unsigned char candidate,
                                               const Board& board) {
    SpotPos pos = params.position;
    Direction direction = params.direction;

    unsigned int score = LetterBag::getLetterPoints(candidate)
            * board(params.position).bonus.letter_factor;

    unsigned int factor = board(params.position).bonus.word_factor;

    string orthogonalWord({ static_cast<char>(candidate) });

    char& movingIndex = direction == Direction::HORIZONTAL
            ? pos.indexLine
            : pos.indexCol;

    char startMovingIndex = movingIndex;
    movingIndex--;

//    if(params.direction == Direction::HORIZONTAL) {
//        if(params.startPos.indexLine == 12 && params.startPos.indexCol == 7) {
//            if(params.position.indexLine == 12 && params.position.indexCol == 7) {
//                cout << pos << endl << board(pos).isEmpty();
//            }
//        }
//    }


    while(Utils::validPos(pos) && !board(pos).isEmpty()) {
        unsigned char nextLetter = board(pos).letter;
        orthogonalWord += static_cast<char>(nextLetter);
        score += LetterBag::getLetterPoints(nextLetter);
        movingIndex--;
    }

    orthogonalWord += static_cast<char>(LINK_LETTER);
//    movingIndex = direction == Direction::HORIZONTAL
//            ? params.position.indexLine + 1
//            : params.position.indexCol + 1;

    movingIndex = startMovingIndex + 1;

    while(Utils::validPos(pos) && !board(pos).isEmpty()) {
        unsigned char nextLetter = board(pos).letter;
        orthogonalWord += static_cast<char>(nextLetter);
        score += LetterBag::getLetterPoints(nextLetter);
        movingIndex++;
    }

//    if(params.direction == Direction::HORIZONTAL) {
//        if(params.startPos.indexLine == 12 && params.startPos.indexCol == 7) {
//            if(orthogonalWord.size() > 2) {
//                cout <<endl << params.word << candidate << " " << orthogonalWord << endl;
//            }
//        }
//    }

    if(orthogonalWord.size() <= 2) {
        return 0;
    }

    if(dico.searchPrivate(orthogonalWord)) {
        return score * factor;
    }

    return nullopt;
}

pair<unique_ptr<Solver::StrokesSet>, Stroke> Solver::getAvailableStrokes(const Config& config) {
    Stroke bestStroke;
    // vector for pushing valid stokes
    unique_ptr<StrokesSet> array = make_unique<StrokesSet>();
    // array of start cells, to begin serach
    unique_ptr<NeighborsSet> startCellsArray = getNeighBors(config.board);

    // stack for store states of research
    stack<SearchingParams> stack;

    for(const SpotPos& position : *startCellsArray) {
        SpotPos startPos{position.indexLine, position.indexCol};
        //cout << "//////////////////////  CASE : " << int(position.indexLine)
        //<< " " << int(position.indexCol) << endl;
        SearchingParams startParams {
             dico.getHead(),
             startPos,
             startPos,
             config.playerBag,
             "",
             PlusStatus::NOT_USED,
             Direction::HORIZONTAL,
             0,
             1,
             0,
             0
        };

        // push horizontal reqsearch start params
        stack.push(startParams);
        startParams.direction = Direction::VERTICAL;
        // push vertical research start params
        stack.push(startParams);

        while(!stack.empty()) {
            // read top of the stack
            SearchingParams currentParams = stack.top();
            stack.pop();

//            if(startParams.direction == Direction::VERTICAL) {
//                if(startPos.indexLine == 0 && startPos.indexCol == 10) {
//                    cout << currentParams.word << endl;
//                    if(currentParams.position.indexLine == 1 && currentParams.position.indexCol == 10) {
//                        cout << currentParams.word << endl;
//                    }
//                }
//            }

            if(Utils::validPos(currentParams.position)) {
                Spot currentSpot = config.board(currentParams.position);

                // there is no letter in the board at currentSpot
                if(currentSpot.isEmpty()) {
                    if(currentParams.node->isFinal()) {
                        bestStroke = max(bestStroke,
                                         addStroke(array, currentParams));
                    }

                    followPlayerBagRoots(currentParams, stack, config);
                    // Use the Plus root if it's not already used
                    if(currentParams.plusStatus == PlusStatus::NOT_USED) {
                        followPlusRoot(currentParams, stack);
                    }
                }
                // There is a letter in the the board at currentSpot
                else {
                    followForcedRoot(currentParams, stack, config);
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
                                  std::stack<SearchingParams> &stack,
                                  const Config& config) {
    const Spot& currentSpot = config.board(params.position);
    PlayerBag currentLetters = params.availableLetters;

//    if(params.direction == Direction::VERTICAL) {
//        if(params.startPos.indexLine == 0 && params.startPos.indexCol == 10) {
//            //cout << params.word << endl << "coucou";
//            if(params.position.indexLine == 1 && params.position.indexCol == 10) {
//                cout << params.word << endl;
//            }
//        }
//    }

    // parcours du tableau de lettre possibles
    for(unsigned char letter : params.availableLetters.data()) {
        Node* child = params.node->getChildByLetter(letter);
        // l'enfant associé a la lettre existe
        if(child != Node::NO_NODE) {
            unsigned int score = LetterBag::getLetterPoints(letter)
                * currentSpot.bonus.letter_factor;

//            if(params.direction == Direction::VERTICAL) {
//                if(params.startPos.indexLine == 0 && params.startPos.indexCol == 10) {
//                    //cout << params.word << letter <<endl << "coucou";
//                    if(params.position.indexLine == 1 && params.position.indexCol == 10) {
//                       // cout << params.word << letter<< endl;
//                    }
//                }
//            }

            SearchingParams nextParams(params);
            nextParams.node = child;
            nextParams.availableLetters = currentLetters.pop(letter);
            nextParams.word += static_cast<char>(letter);
            nextParams.mainScore += score;
            nextParams.mainFactor *= currentSpot.bonus.word_factor;
            nextParams.nbUsedLetters++;

            auto additionnalScore = checkOtherWords(params, letter, config.board);

//            if(params.direction == Direction::VERTICAL) {
//                if(params.startPos.indexLine == 0 && params.startPos.indexCol == 10) {
//                    cout <<endl << nextParams.word << " " << *additionnalScore << endl;
//                    if(params.position.indexLine == 1 && params.position.indexCol == 10) {
//                        cout << params.word << endl;
//                    }
//                }
//            }

            // if orthogonal word not exists or is valid
            if(additionnalScore.has_value()) {
                nextParams.additionnalScore += *additionnalScore;
                nextParams.position = computeNextPos(params);

//                if(params.direction == Direction::VERTICAL) {
//                    if(params.startPos.indexLine == 0 && params.startPos.indexCol == 10) {
//                        cout <<endl << nextParams.word << " " << *additionnalScore << endl;
//                        if(params.position.indexLine == 1 && params.position.indexCol == 10) {
//                            cout << params.word << endl;
//                        }
//                    }
//                }

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
        params.node = linkChild;
        params.position = computeNextPos(params);
        params.word += static_cast<char>(LINK_LETTER);
        params.plusStatus = PlusStatus::USED;
        stack.push(params);
    }
}

void Solver::followForcedRoot(SearchingParams &params,
                              stack<SearchingParams>& stack,
                              const Config& config) {

    unsigned char forcedLetter = config.board(params.position).letter;
    Node* forcedNode = params.node->getChildByLetter(forcedLetter);

    // if ForcedRoot exists
    if(forcedNode != Node::NO_NODE) {
        params.node = forcedNode;
        params.position = computeNextPos(params);
        params.word += static_cast<char>(forcedLetter);
        params.mainScore += LetterBag::getLetterPoints(forcedLetter);
        stack.push(params);
    }
}

unique_ptr<Solver::NeighborsSet> Solver::getNeighBors(const Board& board) {
    unique_ptr set = make_unique<NeighborsSet>();

    if(board.isEmpty()) {
        set->insert(CENTER_OF_BOARD);
    }
    else {
        for(char line = 0; line < char(Board::SIZE); line++) {
            for(char col = 0; col < char(Board::SIZE); col++) {
                Spot current = board({ line, col });
                if(current.letter != 0) {

                    array<SpotPos, 4> neighborsToTest{{
                        { char(line - 1), col },
                        { char(line + 1), col },
                        { line, char(col + 1) },
                        { line, char(col - 1) }
                    }};

                    for(const SpotPos& s : neighborsToTest) {
                        if(Utils::validPos(s) && board(s).isEmpty()) {
                            set->insert(s);
                        }
                    }
                }
            }
        }
    }

    return set;
}
