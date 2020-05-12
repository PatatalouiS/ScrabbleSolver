
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
    future<unsigned int> suzette;

    cout << "Your Board :" << endl
         <<  config.board << endl;

    auto [ availableMoves, bestMove ] = *getAvailableMoves(config);

    // If suzette_check enabled, send a query to get the Suzette Move
    if(suzette_check) {
        suzette = async([&config] () -> unsigned int {
           auto [ score, board ] = Suzette::check(config.board,
                                                    config.playerBag);
           return score;
        });
    }

    if(bestMove.score == 0) {
        cout << "No Move are possible with this configuration !" << endl;
        return;
    }

    cout << Board(config.board).putMove(bestMove);
    cout << "--- BEST Move ---" << endl
         << "With letters : " << config.playerBag << endl
         << "We can play Word  : " << Utils::toRegularWord(bestMove.word)
         << endl << "With a score of : " << bestMove.score << " pts." << endl;

    // If suzette_check enabled, print the score by Suzette
    if(suzette_check) {
        unsigned int suzetteScore = suzette.get();
        cout << "--- SUZETTE ---" << endl
             << "Score By Suzette : " << suzetteScore << " pts.";
    }
}

void Solver::solveFromScratch() {
    LetterBag mainBag(jokers);
    unsigned int totalScore = 0;

    ScrabbleConfig config {
        PlayerBag(mainBag),
        Board()
    };

    // loop while there is no moreletters to play
    while(!mainBag.isEmpty() || !config.playerBag.isEmpty()) {
        auto [ availableMoves, bestMove ] = *getAvailableMoves(config);

        // no Moves find --> end (very rare)
        if(bestMove.score == 0) {
            break;
        }

        totalScore += bestMove.score;

        // copy of before to change(we want too print it)
        PlayerBag lastPlayerBag(config.playerBag);
        Board lastBoard(config.board);

        // Fill the new PlayerBag returned with the main LetterBag
        config.playerBag = PlayerBag(bestMove.playerBag.fillWith(mainBag));

        cout << config.board.putMove(bestMove)
             << "--- BEST Move ---" << endl
             << "With letters : " << lastPlayerBag << endl
             << "We can play Word  : " << Utils::toRegularWord(bestMove.word)
             << endl << "Gaddag style : " << bestMove.word << endl
             << "With a score of : " << bestMove.score << " pts." << endl
             << "Total Score " << totalScore << endl
             << "Letters remaining : " << mainBag.getNbLetters() << endl
             << "Moves found : " << availableMoves.size() << endl
             << "New PlayerBag : " << config.playerBag << endl;

        // If suzette_check enabled, fetch and print the score by Suzette
        if(suzette_check) {
            auto [ suzetteScore, board ] = Suzette::check(lastBoard,
                                                       lastPlayerBag);
            cout << "--- SUZETTE ---" << endl
                 << "Score By Suzette : " << suzetteScore << " pts.";
                 assert(suzetteScore == bestMove.score);
        }

        ofstream file ("./data/laststroke.txt");
        config.board.save(file);
        file.close();
        usleep(100000);
        //clear console, wait.
        //Utils::waitForEnter();
        Utils::clearScreen();
        Utils::printHeader();
    }

    cout << endl << "There is no more Move to play ! "
         << "End of the sequence" << endl;
}

SpotPos Solver::computeNextPos(const SearchingParams& params) {
    PlusStatus plusStatus = params.plusStatus;
    // If we use Plus, we need to compute the new position in relation to start Position
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

    // init the orthogonal word with the currentLetter we want to play
    string orthogonalWord({ static_cast<char>(candidate) });

    // if HORIZONTAL, we move the indexLine, else indexCol
    // So, handle a reference to this index
    char& movingIndex = direction == Direction::HORIZONTAL
            ? pos.indexLine
            : pos.indexCol;

    // copy of the moving index before we change it,
    // useful when we have to search the second part of an orthogonal word(after '+)
    char startMovingIndex = movingIndex;
    movingIndex--;

    // search the first part( Reverse part) of the orthogonal Word, move backward
    while(Utils::validPos(pos) && !board(pos).isEmpty()) {
        unsigned char nextLetter = board(pos).letter;
        orthogonalWord += static_cast<char>(nextLetter);
        score += Utils::getLetterPoints(nextLetter);
        movingIndex--;
    }

    orthogonalWord += static_cast<char>(Gaddag::LINK_LETTER);
    movingIndex = startMovingIndex + 1;

    // search the second part( Normal part) of the orthogonal Word
    // move frontWard
    while(Utils::validPos(pos) && !board(pos).isEmpty()) {
        unsigned char nextLetter = board(pos).letter;
        orthogonalWord += static_cast<char>(nextLetter);
        score += Utils::getLetterPoints(nextLetter);
        movingIndex++;
    }
    // If orthogonalWord is <= 2, The computed word is just the current letter
    // followed by '+' Symbol,
    // CONCLUSION -> There is no orthogonal Word here !
    // So return 0, no additionnal score to add
    if(orthogonalWord.size() <= 2) {
        return 0;
    }
    // If valid orthogonal word, return his score
    if(dico.search(orthogonalWord)) {
        return score * factor;
    }
    //else return none (wrong orthogonal word)
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
    // Empty Board == Search only in the Center Spot in the two directions
    if(board.isEmpty()) {
        set->insert({{ CENTER_OF_BOARD, Direction::VERTICAL },
                     { CENTER_OF_BOARD, Direction::HORIZONTAL }});
    }
    else {
        static const array directionArray = { Direction::VERTICAL,
                                              Direction::HORIZONTAL };
        // for the two directions
        for(const Direction direction : directionArray) {

            // Visit all the Spots
            for(unsigned char line = 0; line < Board::SIZE; line++) {
                for(unsigned char col = 0; col < Board::SIZE; col++) {
                    SpotPos pos { char(line), char(col) };

                    // if spot is empty
                    if(board(pos).isEmpty()) {
                        // if VERTICAL, the movingIndex is the columns, else lines
                        char& movingIndex = direction == Direction::VERTICAL
                                ? pos.indexCol
                                : pos.indexLine;

                        movingIndex++;
                        // the fisrt orthogonal neighbor
                        SpotPos neighbor1(pos);
                        movingIndex -= 2;
                        // the second orthogonal neighbor
                        SpotPos neighbor2(pos);
                        movingIndex++;

                        // if one of the two Neighbors is not empty, add the current pos
                        // to the Set of positions
                        if((validPos(neighbor1) && !board(neighbor1).isEmpty()) ||
                           (validPos(neighbor2) && !board(neighbor2).isEmpty())) {
                            set->insert({ pos, direction });
                        }
                    }
                    else {
                        // if VERTICAL, the Moving index is the lines, else columns
                        char& movinIndex = direction == Direction::VERTICAL
                                ? pos.indexLine
                                : pos.indexCol;

                        movinIndex++;
                         // The Neighbor just after pos, in the same direction
                        SpotPos neighbor(pos);
                        movinIndex--;
                        // If the Neighbor is Empty, add the current pos !
                        if(!validPos(neighbor) || board(neighbor).isEmpty()) {
                            set->insert({ pos, direction });
                        }
                    }
                }
            }
        }
    }
    return set;
}
