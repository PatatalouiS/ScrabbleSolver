#include "board.hpp"

#include <iostream>
#include <sstream>
#include <random>
#include <fstream>
#include <cassert>
#include <unordered_map>
#include "letterbag.hpp"
#include "gaddag.hpp"

using namespace std;

const string DICO_PATH = "./data/dico.txt";
const string DICO_PATH_TEST = "./data/dico_test.txt";


void createDicoTest(const string& pathIN, const string& pathOUT, int modulo) {
    ifstream stream(pathIN);

    if(!stream.is_open()) {
        cout << "Error read" << endl;
        exit(EXIT_FAILURE);
    }

    ofstream newfic(pathOUT);

    if(!newfic.is_open()) {
        cout << "Error write" << endl;
        exit(EXIT_FAILURE);
    }

    int i = 0;
    string buffer;

    while(stream >> buffer) {

        if((i % modulo) == 0) {
            newfic << buffer << endl;
        }

        ++i;
    }
    stream.close();
    newfic.close();
}


void test_search(Gaddag& abr) {
    ifstream stream(DICO_PATH_TEST);

    if(!stream.is_open()) {
        cout << "Error read" << endl;
        exit(EXIT_FAILURE);
    }

    string buffer;

    while(stream >> buffer) {
        assert(abr.search(buffer));
    }

    stream.close();
}


int main() {
    // createDicoTest(DICO_PATH, DICO_PATH_TEST, 20);
    Gaddag tree(DICO_PATH_TEST);
    //cout << tree.search("C") << endl;

    test_search(tree);


//  Board b ;

//  std::cout << b << std::endl ;

//  std::stringstream ss ;
//  ss << "..............." << std::endl ;
//  ss << "..............." << std::endl ;
//  ss << "..............." << std::endl ;
//  ss << "..............." << std::endl ;
//  ss << "..............." << std::endl ;
//  ss << "..............." << std::endl ;
//  ss << "..............." << std::endl ;
//  ss << "....PROJET....." << std::endl ;
//  ss << ".......O......." << std::endl ;
//  ss << ".......U......." << std::endl ;
//  ss << ".......E......." << std::endl ;
//  ss << ".......U......." << std::endl ;
//  ss << ".....SCRABBLE.." << std::endl ;
//  ss << "..............." << std::endl ;
//  ss << "..............." << std::endl ;

//  b.load(ss) ;

//  cout << b << endl;


//  LetterBag bag;

//  for(int i = 0; i < 102; ++i) {
//      cout << "NbLetters in bag : " << bag.getNbLetters() << endl;
//      cout << "Random :" << bag.pickRandomLetter() << endl;
//  }

//  for(char a = 'A'; a <= 'Z'; a++) {
//      cout << "Get score : " << LetterBag::getScore(a) << endl;
//  }


  return 0 ;
}
