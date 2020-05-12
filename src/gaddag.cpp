
#include <fstream>
#include <algorithm>

#include "gaddag.hpp"
#include "utils.hpp"
#include <chrono>
#include <thread>
#include <condition_variable>
#include <queue>

using namespace std;

using WordsArray = std::vector<std::string>;


Gaddag::Gaddag() {
    head = new Node(HEAD_LETTER);
}

Gaddag::Gaddag(const string fileName) : Gaddag() {  
    ifstream stream(fileName);
    try {
        if(!stream.is_open()) {
            throw string("Error when openning File : " + fileName);
        }
    }
    catch(const string& error) {
        cerr << error << endl;
        exit(EXIT_FAILURE);
    }
    string buffer;
    while(stream >> buffer) {
        addWord(buffer);
    }
    stream.close();
}

const Node* Gaddag::getHead() const {
    return head;
}

Node* Gaddag::getHead() {
    return head;
}

unique_ptr<WordsArray> Gaddag::getWordsArray(const string& word) const {
    unique_ptr<WordsArray> array = make_unique<WordsArray>();
    //Just a copy of the word
    string normalString(word);
    auto normalStringIt = normalString.begin() + 1;
    auto normalEnd = normalString.end();
    //The word, but reversed
    string reverseString(word);
    reverse(reverseString.begin(), reverseString.end());
    auto reverseStringIt = reverseString.end() - 1;
    auto reverseEnd = reverseString.end();
    // Until we haven't covered all the word, construt combination
    // Seems little complicated, but after a little execution-time, it is faster
    // than the classic double-for algorithm (more than 1 second gain on our computer)
    while(reverseStringIt >= reverseString.begin()) {
        // take the part of the reversed Word
        string reversePart = reverseString;
        reversePart.assign(reverseStringIt, reverseEnd);
        // take the part of the normla word
        string normalPart = normalString;
        normalPart.assign(normalStringIt, normalEnd);
        //push in array (reversepart)+(normal part)
        array->push_back(reversePart
                            .append(1,LINK_LETTER)
                            .append(normalPart));
        //Move the iterators
        reverseStringIt--;
        normalStringIt++;
    }
    return array;
}

Gaddag& Gaddag::addWordPrivate(const string& word) {
    Node* current = head;

    for(char letter : word) {
        unsigned char letterToInsert = static_cast<unsigned char>(letter);
        Node* nextNode = current->getChildByLetter(letterToInsert);
        // if nextNode not exists, create it
        if(nextNode == nullptr) {
            Node* newNode = new Node(letterToInsert);
            current->addChild(newNode);
            current = newNode;
        }
        // continue to visit
        else {
            current = nextNode;
        }
    }
    current->setFinal();
    return *this;
}

Gaddag& Gaddag::addWord(const std::string &word) {
    //empty string case
    if(word == "") {
        return *this;
    }
    unique_ptr<WordsArray> wordsToInsert = getWordsArray(word);
    // add all the gaddagged representation of the word to Gaddag
    for(const string& w : *wordsToInsert) {
        addWordPrivate(w);
    }
    return *this;
}

bool Gaddag::search(const string& word) const {
    Node* current = head;
    const auto lastLetter = word.end()-1;
    auto wordIterator = word.begin();

    while(true) {
        unsigned char letterToSearch = static_cast<unsigned char>(*wordIterator);
        Node* nextNode = current->getChildByLetter(letterToSearch);
        // if node not exists, search fails
        if(nextNode == nullptr) {
            return false;
        }
        current = nextNode;
        // if there is no more letter tos test, return the state of the current Node
        if(wordIterator == lastLetter) {
            return current->isFinal();
        }
        wordIterator++;
    }
}

Gaddag::~Gaddag() {
    delete head;
}

std::ostream& operator<<(std::ostream& out, const Gaddag& g) {
    using WordPair = std::pair<const Node*, std::string>;
    stack<WordPair> stack({{ g.getHead(), "" }});
    WordPair current;

    while(!stack.empty()) {
        auto [ currentNode, currentWord ] = stack.top();
        stack.pop();

        if(currentNode->isFinal()) {
            out << currentWord << endl;
        }
        for(Node* node : currentNode->getChilds()) {
            if(node != nullptr) {
                string newWord = currentWord
                        + static_cast<char>(node->getLetter());
                stack.push({ node, newWord });
            }
        }
    }
    return out;
}
