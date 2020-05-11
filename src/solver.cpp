
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <cassert>

#include "utils.hpp"
#include "suzettecheck.hpp"
#include "algorithm"

using namespace std;

namespace {
    using MovesSet = std::unordered_set<Move>;

    // Value of the Scrabble Bonus (7 letter on the board)
    constexpr unsigned int SCRABBLE_BONUS = 50;

    // Represent the Center Position in Board
    constexpr SpotPos CENTER_OF_BOARD = SpotPos{ 7, 7 };

    // Some useful functors, permits to compute indexes less
    // verbosely than too If/else statements with using a little
    // bit of functionnal programming (see computeNextPos() method)
    auto plusFunc = [](const char a, const char b) -> char {
        return a + b;
    };
    auto minusFunc = [](const char a, const char b) -> char {
        return a - b;
    };

    // The Max between two Moves
    const Move& max(const Move& a, const Move& b) {
        return b.score > a.score
            ? b
            : a;
    }
    // Is this SearchingParams instance is a playable Move ?
    bool validMoveCandidate(const Solver::SearchingParams& params) {
        return params.node->isFinal() &&
               (params.nbUsedLetters != 0);
    }
    // Add a good SearchingParams instance to the set of possible Moves
    const Move& addMove(MovesSet& result,
                        const Solver::SearchingParams& params) {
        unsigned int totalScore = (params.mainScore * params.mainFactor)
                + params.additionnalScore;

        if(params.nbUsedLetters == PlayerBag::MAX_SIZE) {
            totalScore += SCRABBLE_BONUS;
        }

        string regular = Utils::toRegularWord(params.word);

        Move toAdd({ params.word,
                        params.startPos,
                        params.direction,
                        PlayerBag(params.availableLetters.data()),
                        totalScore });

        return *(result.emplace(toAdd).first);
    }
}

Solver::Solver(const Gaddag& dico, const bool s, const bool j)
    : dico(dico), suzette_check(s), jokers(j) {}

Solver::SearchingParams::SearchingParams(const Node* n, const SpotPos& start,
                                         const PlayerBag& p, const Direction d):
    node(n), position(start), startPos(start), availableLetters(p), word(""),
    plusStatus(PlusStatus::NOT_USED), direction(d), mainScore(0), mainFactor(1),
    additionnalScore(0), nbUsedLetters(0) {}

void Solver::solveConfig(const ScrabbleConfig& config) {
    cout << "Your Configuration :" << endl
         <<  config.board << endl;

    auto availableMoves = *getAvailableMoves(config);
    Move bestMove = availableMoves.second;

    cout << Board(config.board).putMove(bestMove);
    cout << "--- BEST Move ---" << endl
         << "With letters : " << config.playerBag << endl
         << "We can play Word  : " << Utils::toRegularWord(bestMove.word)
         << endl << "With a score of : " << bestMove.score << " pts." << endl;
    // If suzette_check enabled, show the score by Suzette !
    if(suzette_check) {
        auto [ score, board ] = Suzette::check(config.board, config.playerBag);
        cout << "--- SUZETTE ---" << endl
             << "Score By Suzette : " << score << " pts." << endl;
    }
}

void Solver::solveFromScratch() {
    LetterBag mainBag(jokers);
    unsigned int totalScore = 0;
    unsigned int suzetteScore = 0;

    ScrabbleConfig config {
        PlayerBag(mainBag),
        Board()
    };
    // loop while there is no moreletters to play
    while(!mainBag.isEmpty() || !config.playerBag.isEmpty()) {
        auto availableMoves = *getAvailableMoves(config);
        Move bestMove = availableMoves.second;
        unsigned int suzetteScore = 0;
        // no Moves find (very rare)
        if(bestMove.score == 0) {
            break;
        }
        // If suzette_check enabled, show the score by Suzette !
        if(suzette_check) {
            auto [ score, board ] = Suzette::check(config.board, config.playerBag);
            assert(bestMove.score == score);
            suzetteScore = score;
        }

        // Increment total score
        totalScore += bestMove.score;
        // copy of the lastPlayerBag(we want too print it)
        PlayerBag lastPlayerBag(config.playerBag);
        // Fill the new PlayerBag returned with the main LetterBag
        config.playerBag = PlayerBag(bestMove.playerBag.fillWith(mainBag));
        // Place move on the board
        config.board.putMove(bestMove);

        cout << config.board
             << "--- BEST Move ---" << endl
             << "With letters : " << lastPlayerBag << endl
             << "We can play Word  : " << Utils::toRegularWord(bestMove.word)
             << endl << "Gaddag style : " << bestMove.word << endl
             << "With a score of : " << bestMove.score << " pts." << endl
             << "Total Score " << totalScore << endl
             << "Letters remaining : " << mainBag.getNbLetters() << endl
             << "Moves founded : " << availableMoves.first.size() << endl
             << "New PlayerBag : " << config.playerBag << endl;
        // If suzette_check enabled, show the score by Suzette !
        if(suzette_check) {
            cout << "--- SUZETTE ---" << endl
                 << "Score By Suzette : " << suzetteScore << " pts.";
        }

        ofstream file("./data/lastMove.txt");
        config.board.save(file);
        file.close();
        //usleep(500000);
         //clear console, wait..
        Utils::waitForEnter();
        Utils::clearScreen();
        Utils::printHeader();
    }

    cout << endl << "There is no more Move to play ! "
         << "End of the series" << endl;
}

SpotPos Solver::computeNextPos(const SearchingParams& params) {
    PlusStatus plusStatus = params.plusStatus;

    // If we use Plus, we need to compute start Position
    // else we need to compute with current position
    const SpotPos& spotUsed = plusStatus == PlusStatus::IN_USE
            ? params.startPos
            : params.position;

    // If plusStatus is not used, we are going to decrement an index(minus Functor)
    // else we need to increment (plus Functor)
    auto op = plusStatus == PlusStatus::NOT_USED
            ? minusFunc
            : plusFunc;

    char lineModifier;
    char colModifier;

    // HORIZONTAL ? we need to apply functor on the Column index
    if(params.direction == Direction::HORIZONTAL) {
        lineModifier = 0;
        colModifier = 1;
    } //VERTICAL ? we need to apply functor on the Line index
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

    // get the score of the SpotPos to check
    unsigned int score = Utils::getLetterPoints(candidate)
            * board(params.position).bonus.letter_factor;
    unsigned int factor = board(params.position).bonus.word_factor;

    string orthogonalWord({ static_cast<char>(candidate) });

    // if HORIZONTAL, we move the indexLine, else indexCol
    char& movingIndex = direction == Direction::HORIZONTAL
            ? pos.indexLine
            : pos.indexCol;

    // copy of the moving index before we change it,
    // useful when we have to search the second part of an orthogonal word
    char startMovingIndex = movingIndex;
    movingIndex--;

    // search the first part( Reverse part) of the orthogonal Word
    while(Utils::validPos(pos) && !board(pos).isEmpty()) {
        unsigned char nextLetter = board(pos).letter;
        orthogonalWord += static_cast<char>(nextLetter);
        score += Utils::getLetterPoints(nextLetter);
        movingIndex--;
    }

    orthogonalWord += static_cast<char>(Gaddag::LINK_LETTER);
    movingIndex = startMovingIndex + 1;

    // search the second part( Normal part) of the orthogonal Word
    while(Utils::validPos(pos) && !board(pos).isEmpty()) {
        unsigned char nextLetter = board(pos).letter;
        orthogonalWord += static_cast<char>(nextLetter);
        score += Utils::getLetterPoints(nextLetter);
        movingIndex++;
    }
    // If orthogonalWord is <= 2, The computed word is just the current letter
    // followed by '+' Symbol,
    // CONCLUSION -> There is no orthogonal Word here ! return 0
    if(orthogonalWord.size() <= 2) {
        return 0;
    }
    // If valid orthogonal word, return his score
    if(dico.search(orthogonalWord)) {
        return score * factor;
    }
    //else return none
    return nullopt;
}

unique_ptr<pair<Solver::MovesSet, Move>>
        Solver::getAvailableMoves(const ScrabbleConfig& config) {
    // The result
    auto result = make_unique<pair<MovesSet, Move>>();
    // get references to result pair
    auto& [MovesArray, bestMove] = *result;
    auto startCellsArray = getStartSpots(config.board);
    stack<SearchingParams> stack;
    // For all the computed StartCells
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
                    //If a vaid Move exists, add it ! (and set the bestMove)
                    if(validMoveCandidate(currentParams)) {
                        bestMove = max(bestMove, addMove(MovesArray,
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
            }// We are on the edge of the Board, and moving backward
            else if(currentParams.plusStatus == PlusStatus::NOT_USED) {
                followPlusRoot(currentParams, stack);
            }// we are on the edge of the Board, and moving frontward
            else if(validMoveCandidate(currentParams)){
                bestMove = max(bestMove, addMove(MovesArray,
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
    const PlayerBag& currentLetters = params.availableLetters;
    // For all the letters available in the PlayerBag
    for(unsigned char letter : params.availableLetters.data()) {
        // if joker mode is enabled, try all the letters of alphabet !
        if(jokers && (letter == JOKER_SYMBOL)) {
            followJokerRoots(params, stack, config);
        }
        else {
            Node* child = params.node->getChildByLetter(letter);
            // if related child to letter exists
            if(child != nullptr) {
                // compute score, we can play a letter
                unsigned int score = Utils::getLetterPoints(letter)
                    * currentSpot.bonus.letter_factor;
                // checking for other words
                auto additionnalScore = checkOtherWords(params, letter,
                                                        config.board);
                // if orthogonal word not exists or is valid, add new Params !
                if(additionnalScore.has_value()) {
                    SearchingParams nextParams(params);
                    nextParams.node = child;
                    nextParams.availableLetters = currentLetters.pop(letter);
                    nextParams.word += static_cast<char>(letter);
                    nextParams.mainScore += score;
                    nextParams.mainFactor *= currentSpot.bonus.word_factor;
                    nextParams.nbUsedLetters++;
                    nextParams.additionnalScore += *additionnalScore;
                    nextParams.position = computeNextPos(params);
                    stack.emplace(nextParams);
                }
            }
        }
    }
}

void Solver::followJokerRoots(SearchingParams &params,
                               std::stack<SearchingParams> &stack,
                               const ScrabbleConfig& config) {

    const Spot& currentSpot = config.board(params.position);
    const PlayerBag& currentLetters = params.availableLetters;
    // playing joker = Follow all the child Nodes of current node !
    for(const Node* child : params.node->getChilds()) {
        if(child != nullptr && child->getLetter() != Gaddag::LINK_LETTER) {
            unsigned char letter2 = child->getLetter();
            auto additionnalScore = checkOtherWords(params, letter2,
                                                    config.board);
            if(additionnalScore.has_value()) {
                SearchingParams nextParams(params);
                nextParams.node = child;
                nextParams.availableLetters = currentLetters.pop(JOKER_SYMBOL);
                nextParams.word += static_cast<char>(letter2);
                nextParams.mainFactor *= currentSpot.bonus.word_factor;
                nextParams.nbUsedLetters++;
                nextParams.additionnalScore += *additionnalScore;
                nextParams.position = computeNextPos(params);
                stack.emplace(nextParams);
            }
        }
    }
}

void Solver::followPlusRoot(SearchingParams &params,
                            stack<SearchingParams> &stack) {

    Node* linkChild = params.node->getChildByLetter(Gaddag::LINK_LETTER);
    // if PlusRoot exists
    if(linkChild != nullptr) {
        params.plusStatus = PlusStatus::IN_USE;
        params.node = linkChild;
        params.position = computeNextPos(params);
        params.word += static_cast<char>(Gaddag::LINK_LETTER);
        params.plusStatus = PlusStatus::USED;
        stack.emplace(params);
    }
}

void Solver::followForcedRoot(SearchingParams &params,
                              stack<SearchingParams>& stack,
                              const ScrabbleConfig& config) {

    unsigned char forcedLetter = config.board(params.position).letter;
    Node* forcedNode = params.node->getChildByLetter(forcedLetter);
    // if ForcedRoot exists
    if(forcedNode != nullptr) {
        params.node = forcedNode;
        params.position = computeNextPos(params);
        params.word += static_cast<char>(forcedLetter);
        params.mainScore += Utils::getLetterPoints(forcedLetter);
        stack.emplace(params);
    }
}

unique_ptr<Solver::StartPosSet> Solver::getStartSpots(const Board& board) {
    using namespace Utils;
    unique_ptr set = make_unique<StartPosSet>();
    // Empty Board == Search only in the Center Spot
    if(board.isEmpty()) {
        set->insert({{ CENTER_OF_BOARD, Direction::VERTICAL },
                     { CENTER_OF_BOARD, Direction::HORIZONTAL }});
    }
    else {
        static const array dArray = { Direction::VERTICAL,
                                      Direction::HORIZONTAL };
        // for the two directions
        for(const Direction d : dArray) {
            // Visit all the Spots
            for(unsigned char line = 0; line < Board::SIZE; line++) {
                for(unsigned char col = 0; col < Board::SIZE; col++) {
                    SpotPos pos { char(line), char(col) };
                    // if spot is empty
                    if(board(pos).isEmpty()) {
                        // if VERTICAL, the movingIndex is the columns, else lines
                        char& movingIndex = d == Direction::VERTICAL
                                ? pos.indexCol
                                : pos.indexLine;

                        movingIndex++;
                        // One of the orthogonal neighbor
                        SpotPos neighbor1(pos);
                        movingIndex -= 2;
                        // teh second othogonal neighbor
                        SpotPos neighbor2(pos);
                        movingIndex++;
                        // if one of teh two Neighbors is not empty, add the current pos !
                        if((validPos(neighbor1) && !board(neighbor1).isEmpty()) ||
                           (validPos(neighbor2) && !board(neighbor2).isEmpty())) {
                            set->insert({ pos, d });
                        }
                    }
                    else {
                        // if VERTICAL, the Moving index is the lines, else columns
                        char& movinIndex = d == Direction::VERTICAL
                                ? pos.indexLine
                                : pos.indexCol;

                        movinIndex++;
                         // The Neighbor just after pos, in the same direction
                        SpotPos neighbor(pos);
                        movinIndex--;
                        // If the Neighbor is Empty, add the current pos !
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
