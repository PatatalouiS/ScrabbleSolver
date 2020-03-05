#include "gaddag.hpp"
#include <algorithm>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const unsigned char HEAD_LETTER = '@';

Gaddag::Gaddag() {
    head = new Node(HEAD_LETTER);
}

Gaddag::Gaddag(const string fileName) : Gaddag() {
    ifstream stream(fileName);

    if(!stream.is_open()) {
        cerr << "Error when openning File : " << fileName << endl;
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

Gaddag& Gaddag::addWord(const string& word) {
    if(word == "") {
        return *this;
    }

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

bool Gaddag::search(const string& word) const {
    bool trouve = false;
    Node* current = head;
    const auto lastLetter = word.end()-1;
    auto wordIterator = word.begin();
    ChildsArray currentChilds;
    Node* nextNode;
    unsigned char letterToSearch;
    unsigned char currentLetter;

    while(!trouve) {
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
    return false;
}

void Gaddag::print() const {
    WordPair(head, "");
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
