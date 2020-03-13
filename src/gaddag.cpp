#include "gaddag.hpp"
#include <stack>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

using WordsArray = std::vector<std::string>;

static constexpr unsigned char HEAD_LETTER = '@';
static constexpr unsigned char LINK_LETTER = '+';

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

unique_ptr<WordsArray> Gaddag::getWordsArray(const string& word) const {
    unique_ptr<WordsArray> array = make_unique<WordsArray>();

    string normalString(word);
    auto normalStringIt = normalString.begin() + 1;
    auto normalEnd = normalString.end();

    string reverseString(word);
    reverse(reverseString.begin(), reverseString.end());
    auto reverseStringIt = reverseString.end() - 1;
    auto reverseEnd = reverseString.end();

    string normalPart;
    string reversePart;

    while(reverseStringIt >= reverseString.begin()) {
        reversePart = reverseString;
        reversePart.assign(reverseStringIt, reverseEnd);

        normalPart = normalString;
        normalPart.assign(normalStringIt, normalEnd);

        array->push_back(reversePart.append(1,LINK_LETTER).append(normalPart));

        reverseStringIt--;
        normalStringIt++;
    }

    return array;
}

Gaddag& Gaddag::addWordPrivate(const string& word) {
    const auto lastLetter = word.end()-1;
    auto wordIterator = word.begin();
    bool inserted = false;
    unsigned char currentLetter;
    unsigned char letterToInsert;
    Node* current = head;
    Node* newNode;
    Node* nextNode;
    ChildsArray currentChilds;

    while(!inserted) {
        letterToInsert = static_cast<unsigned char>(*wordIterator);
        currentChilds = current->getChilds();
        currentLetter = current->getLetter();
        nextNode = current->getChildByLetter(letterToInsert);

        if(nextNode == Node::NO_NODE) {
            newNode = new Node(letterToInsert);
            current->addChild(newNode);
            current = newNode;
        }
        else {
            current = nextNode;
        }

        if(wordIterator == lastLetter) {
            current->setFinal();
            inserted = true;
        }
        else {
            wordIterator++;
        }
    }
    return *this;
}

Gaddag& Gaddag::addWord(const std::string &word) {
    if(word == "") {
        return *this;
    }

    unique_ptr<WordsArray> wordsToInsert = getWordsArray(word);

    for(const string& w : *wordsToInsert) {
        addWordPrivate(w);
    }

    return *this;
}

bool Gaddag::search(const string& word) const {
    //bool trouve = false;
    Node* current = head;
    const auto lastLetter = word.end()-1;
    auto wordIterator = word.begin();
    ChildsArray currentChilds;
    Node* nextNode;
    unsigned char letterToSearch;
    unsigned char currentLetter;

    while(true) { //! trouve
        letterToSearch = static_cast<unsigned char>(*wordIterator);
        currentChilds = current->getChilds();
        currentLetter = current->getLetter();
        nextNode = current->getChildByLetter(letterToSearch);

        if(nextNode == Node::NO_NODE) {
            return false;
        }

        current = nextNode;

        if(wordIterator == lastLetter) {
            return current->isFinal()
                ? true
                : false;
        }
        wordIterator++;
    }
}

void Gaddag::print() const {
    stack<WordPair> stack({ make_pair(head, "") });
    WordPair current;
    string currentWord;
    Node* currentNode;

    while(!stack.empty()) {
        current = stack.top();
        currentNode = current.first;
        currentWord = current.second;
        stack.pop();

        if(currentNode->isFinal()) {
            cout << current.second << endl;
        }

        for(Node* node : currentNode->getChilds()) {
            if(node != Node::NO_NODE) {
                string newWord = currentWord + static_cast<char>(node->getLetter());
                stack.push(WordPair(node, newWord));
            }
        }
    }
}

Gaddag::~Gaddag() {
    stack<Node*> stack({ head });
    Node* current;
    int nbDeleted = 0;

    while(!stack.empty()) {
        current = stack.top();
        stack.pop();

        for(Node* node : current->getChilds()) {
            if(node != Node::NO_NODE) {
                stack.push(node);
            }
        }

        delete current;
        nbDeleted++;
    }
    cout << "nbDeleted : " << nbDeleted << endl;
}
