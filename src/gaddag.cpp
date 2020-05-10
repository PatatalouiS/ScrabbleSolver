
#include <fstream>

#include "gaddag.hpp"
#include "utils.hpp"

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

    string normalString(word);
    auto normalStringIt = normalString.begin() + 1;
    auto normalEnd = normalString.end();

    string reverseString(word);
    reverse(reverseString.begin(), reverseString.end());
    auto reverseStringIt = reverseString.end() - 1;
    auto reverseEnd = reverseString.end();

    while(reverseStringIt >= reverseString.begin()) {
        string reversePart = reverseString;
        reversePart.assign(reverseStringIt, reverseEnd);

        string normalPart = normalString;
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
    Node* current = head;

    while(!inserted) {
        unsigned char letterToInsert = static_cast<unsigned char>(*wordIterator);
        Node* nextNode = current->getChildByLetter(letterToInsert);

        if(nextNode == nullptr) {
            Node* newNode = new Node(letterToInsert);
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
    Node* current = head;
    const auto lastLetter = word.end()-1;
    auto wordIterator = word.begin();

    while(true) {
        unsigned char letterToSearch = static_cast<unsigned char>(*wordIterator);
        Node* nextNode = current->getChildByLetter(letterToSearch);

        if(nextNode == nullptr) {
            return false;
        }

        current = nextNode;

        if(wordIterator == lastLetter) {
            return current->isFinal();
        }
        wordIterator++;
    }
}


//bool Gaddag::search(const std::string &word) const {
//    if(word.length() <= 1) {
//        return false;
//    }

//    Node* current = head;
//    const unsigned char firstLetter = static_cast<unsigned char>(word[0]);
//    Node* nextNode;

//    nextNode = current->getChildByLetter(firstLetter);
//    if(nextNode == Node::NO_NODE) {
//        return false;
//    }

//    current = nextNode;

//    nextNode = current->getChildByLetter(LINK_LETTER);
//    if(nextNode == Node::NO_NODE) {
//        return false;
//    }

//    return searchPrivate(word.substr(1, word.length() - 1),  nextNode);
//}

void Gaddag::print() const {
    stack<WordPair> stack({ make_pair(head, "") });
    WordPair current;

    while(!stack.empty()) {
        auto [ currentNode, currentWord ] = stack.top();
        stack.pop();

        if(currentNode->isFinal()) {
            cout << currentWord << endl;
        }

        for(Node* node : currentNode->getChilds()) {
            if(node != nullptr) {
                string newWord = currentWord
                        + static_cast<char>(node->getLetter());
                stack.push({ node, newWord });
            }
        }
    }
}

Gaddag::~Gaddag() {
    delete head;
}
