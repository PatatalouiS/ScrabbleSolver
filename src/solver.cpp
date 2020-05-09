
#include <chrono>
#include <sstream>
#include <fstream>
#include <unistd.h>

#include "utils.hpp"
#include "globals.hpp"
#include "suzettecheck.hpp"
#include "algorithm"

using namespace std;

Solver::Solver(const Gaddag& dico) : dico(dico) {}

Solver::SearchingParams::SearchingParams(const Node* n, const SpotPos& start,
                                         const PlayerBag& p, const Direction d):
    node(n), position(start), startPos(start), availableLetters(p), word(""),
    plusStatus(PlusStatus::NOT_USED), direction(d), mainScore(0), mainFactor(1),
    additionnalScore(0), nbUsedLetters(0) {}


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

    bool validStrokeCandidate(const Solver::SearchingParams& params) {
        return params.node->isFinal() &&
               params.nbUsedLetters != 0;
    }

    const Stroke& addStroke(Solver::StrokesSet& result,
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

        return *(result.emplace(toAdd).first);
    }
}

void Solver::solveConfig(const ScrabbleConfig& config) {
    cout << "Your Configuration :" << endl
         <<  config.board << endl;

    auto availableStrokes = *getAvailableStrokes(config);
    Stroke bestStroke = availableStrokes.second;

    cout << Board(config.board).putStroke(bestStroke);
    cout << "--- BEST STROKE ---" << endl
         << "With letters : " << config.playerBag << endl
         << "We can play Word  : " << Utils::toRegularWord(bestStroke.word)
         << endl << "With a score of : " << bestStroke.score << " pts." << endl;

    if(SUZETTE_CHECK) {
        auto [ score, board ] = Suzette::check(config.board, config.playerBag);
        cout << "Score By Suzette : " << score << " pts." << endl;
    }
}

void Solver::solveFromScratch() {
    LetterBag mainBag;
    unsigned int totalScore = 0;
    unsigned int suzetteScore = 0;

    ScrabbleConfig config {
        PlayerBag(mainBag),
        Board()
    };

    while(!mainBag.isEmpty() || !config.playerBag.isEmpty()) {
        auto availableStrokes = *getAvailableStrokes(config);
        Stroke bestStroke = availableStrokes.second;

        if(bestStroke.score == 0) {
            break;
        }

        if(SUZETTE_CHECK) {
            suzetteScore = Suzette::check(config.board, config.playerBag).first;
            assert(bestStroke.score >= suzetteScore);
        }

        totalScore += bestStroke.score;
        PlayerBag lastPlayerBag(config.playerBag);
        config.playerBag = PlayerBag(bestStroke.playerBag.fillWith(mainBag));
        config.board.putStroke(bestStroke);

        cout << config.board << endl;
        cout << "--- BEST STROKE ---" << endl
             << "With letters : " << lastPlayerBag << endl
             << "We can play Word  : " << Utils::toRegularWord(bestStroke.word)
             << endl << "Gaddag style : " << bestStroke.word << endl
             << "With a score of : " << bestStroke.score << " pts." << endl
             << "Total Score " << totalScore << endl
             << "Letters remaining : " << mainBag.getNbLetters() << endl
             << "Strokes founded : " << availableStrokes.first.size() << endl
             << "New PlayerBag : " << config.playerBag << endl;

        if(SUZETTE_CHECK) {
             cout << "Score By Suzette : " << suzetteScore << " pts." << endl;
        }

        ofstream file("./data/lastStroke.txt");
        config.board.save(file);
        file.close();

//        string wait;
//        getline(cin, wait);
        usleep(500000);
        Utils::clearScreen();
        Utils::printHeader();
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

    unsigned int score = Utils::getLetterPoints(candidate)
            * board(params.position).bonus.letter_factor;
    unsigned int factor = board(params.position).bonus.word_factor;

    string orthogonalWord({ static_cast<char>(candidate) });

    char& movingIndex = direction == Direction::HORIZONTAL
            ? pos.indexLine
            : pos.indexCol;

    char startMovingIndex = movingIndex;
    movingIndex--;

    while(Utils::validPos(pos) && !board(pos).isEmpty()) {
        unsigned char nextLetter = board(pos).letter;
        orthogonalWord += static_cast<char>(nextLetter);
        score += Utils::getLetterPoints(nextLetter);
        movingIndex--;
    }

    orthogonalWord += static_cast<char>(LINK_LETTER);
    movingIndex = startMovingIndex + 1;

    while(Utils::validPos(pos) && !board(pos).isEmpty()) {
        unsigned char nextLetter = board(pos).letter;
        orthogonalWord += static_cast<char>(nextLetter);
        score += Utils::getLetterPoints(nextLetter);
        movingIndex++;
    }

    if(orthogonalWord.size() <= 2) {
        return 0;
    }

    if(dico.searchPrivate(orthogonalWord)) {
        return score * factor;
    }

    return nullopt;
}

unique_ptr<pair<Solver::StrokesSet, Stroke>>
        Solver::getAvailableStrokes(const ScrabbleConfig& config) {

    auto result = make_unique<pair<StrokesSet, Stroke>>();
    auto& [strokesArray, bestStroke] = *result;
    auto startCellsArray = getStartSpots2(config.board);
    stack<SearchingParams> stack;

    for(const auto& [ startPos, direction ] : *startCellsArray) {
        stack.push({ dico.getHead(), startPos, config.playerBag, direction });

        while(!stack.empty()) {
            // read top of the stack
            SearchingParams currentParams = stack.top();
            stack.pop();

            if(Utils::validPos(currentParams.position)) {
                Spot currentSpot = config.board(currentParams.position);
                // there is no letter in the board at currentSpot
                if(currentSpot.isEmpty()) {
                    if(validStrokeCandidate(currentParams)) {
                        bestStroke = max(bestStroke, addStroke(strokesArray,
                                                               currentParams));
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
            else if(validStrokeCandidate(currentParams)){
                bestStroke = max(bestStroke, addStroke(strokesArray,
                                                       currentParams));
            }
        }
    }
    return result;
}

void Solver::followPlayerBagRoots(SearchingParams &params,
                                  std::stack<SearchingParams> &stack,
                                  const ScrabbleConfig& config) {
    const Spot& currentSpot = config.board(params.position);
    PlayerBag currentLetters = params.availableLetters;
    // parcours du tableau de lettre possibles
    for(unsigned char letter : params.availableLetters.data()) {
        Node* child = params.node->getChildByLetter(letter);
        // l'enfant associé a la lettre existe
        if(child != Node::NO_NODE) {
            unsigned int score = Utils::getLetterPoints(letter)
                * currentSpot.bonus.letter_factor;

            SearchingParams nextParams(params);
            nextParams.node = child;
            nextParams.availableLetters = currentLetters.pop(letter);
            nextParams.word += static_cast<char>(letter);
            nextParams.mainScore += score;
            nextParams.mainFactor *= currentSpot.bonus.word_factor;
            nextParams.nbUsedLetters++;

            auto additionnalScore = checkOtherWords(params, letter, config.board);
            // if orthogonal word not exists or is valid
            if(additionnalScore.has_value()) {
                nextParams.additionnalScore += *additionnalScore;
                nextParams.position = computeNextPos(params);
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
                              const ScrabbleConfig& config) {

    unsigned char forcedLetter = config.board(params.position).letter;
    Node* forcedNode = params.node->getChildByLetter(forcedLetter);
    // if ForcedRoot exists
    if(forcedNode != Node::NO_NODE) {
        params.node = forcedNode;
        params.position = computeNextPos(params);
        params.word += static_cast<char>(forcedLetter);
        params.mainScore += Utils::getLetterPoints(forcedLetter);
        stack.push(params);
    }
}

unique_ptr<Solver::NeighborsSet> Solver::getStartSpots(const Board& board) {
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

unique_ptr<Solver::NeighborsSet2> Solver::getStartSpots2(const Board& board) {
    using namespace Utils;
    unique_ptr set = make_unique<NeighborsSet2>();

    if(board.isEmpty()) {
        set->insert({{ CENTER_OF_BOARD, Direction::VERTICAL },
                     { CENTER_OF_BOARD, Direction::HORIZONTAL }});
    }
    else {
        static const array dArray = { Direction::VERTICAL,
                                      Direction::HORIZONTAL };

        for(const Direction d : dArray) {
            for(unsigned char line = 0; line < Board::SIZE; line++) {
                for(unsigned char col = 0; col < Board::SIZE; col++) {
                    SpotPos pos { char(line), char(col) };

                    if(board(pos).isEmpty()) {
                        char& movingIndex = d == Direction::VERTICAL
                                ? pos.indexCol
                                : pos.indexLine;

                        movingIndex++;
                        SpotPos neighbor1(pos);
                        movingIndex -= 2;
                        SpotPos neighbor2(pos);
                        movingIndex++;

                        if((validPos(neighbor1) && !board(neighbor1).isEmpty()) ||
                           (validPos(neighbor2) && !board(neighbor2).isEmpty())) {
                            set->insert({ pos, d });
                        }
                    }
                    else {
                        char& movinIndex = d == Direction::VERTICAL
                                ? pos.indexLine
                                : pos.indexCol;

                        movinIndex++;
                        SpotPos neighbor(pos);
                        movinIndex--;

                        if(!validPos(neighbor) || board(neighbor).isEmpty()) {
                            set->insert({ pos, d });
                        }
                    }
                }
            }
        }
    }
    return set;
}

