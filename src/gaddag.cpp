#include <stack>
#include <iostream>
#include <fstream>
#include <chrono>

#include "gaddag.hpp"
#include "utils.hpp"

using namespace std;

using WordsArray = std::vector<std::string>;

Gaddag::Gaddag() {
    head = new Node(HEAD_LETTER);
}

Gaddag::Gaddag(const string fileName) : Gaddag() {
    auto start = chrono::high_resolution_clock::now();
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
    auto end = chrono::high_resolution_clock::now();

    cout << "Exec Time Constructor : " <<
        chrono::duration_cast<chrono::milliseconds>(end-start).count() << endl;
}

const Node* Gaddag::getHead() const {
    return head;
}

Node* Gaddag::getHead() {
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

        array->push_back(reversePart
                            .append(1,LINK_LETTER)
                            .append(normalPart));

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

bool Gaddag::searchPrivate(const string& word, Node* start) const {
    //bool trouve = false;
    Node* current = start == nullptr
            ? head
            : start;

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
            return current->isFinal();
        }
        wordIterator++;
    }
}

unique_ptr<vector<string>> Gaddag::containsArray(const unsigned char c) const {
    unique_ptr<vector<string>> tab = make_unique<vector<string>>();

    Node* current = head->getChildByLetter(c);
    if(current == Node::NO_NODE) return tab;

    stack<WordPair> stack({{ current,
                             string().append(1, static_cast<char>(c)) }});
    WordPair strPair;
    string currentWord;
    Node* currentNode;

    while(!stack.empty()) {
        strPair = stack.top();
        currentNode = strPair.first;
        currentWord = strPair.second;
        stack.pop();

        if(currentNode->isFinal()) {
            tab->push_back(Utils::toRegularWord(strPair.second));
        }

        for(Node* node : currentNode->getChilds()) {
            if(node != Node::NO_NODE) {
                string newWord = currentWord + static_cast<char>(node->getLetter());
                stack.push({ node, newWord });
            }
        }
    }


    return tab;
}

bool Gaddag::search(const std::string &word) const {
    if(word.length() <= 1) {
        return false;
    }

    Node* current = head;
    const unsigned char firstLetter = static_cast<unsigned char>(word[0]);
    Node* nextNode;

    nextNode = current->getChildByLetter(firstLetter);
    if(nextNode == Node::NO_NODE) {
        return false;
    }

    current = nextNode;

    nextNode = current->getChildByLetter(LINK_LETTER);
    if(nextNode == Node::NO_NODE) {
        return false;
    }

    return searchPrivate(word.substr(1, word.length() - 1),  nextNode);
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
                stack.push({ node, newWord });
            }
        }
    }
}

Gaddag::~Gaddag() {
    auto start = chrono::high_resolution_clock::now();
    delete head;
    auto end = chrono::high_resolution_clock::now();

    cout << "Exec Time Destructor : " <<
        chrono::duration_cast<chrono::milliseconds>(end-start).count() << endl;
}
