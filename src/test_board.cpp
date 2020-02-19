#include "board.hpp"

#include <iostream>
#include <sstream>
#include <random>
#include "letterbag.hpp"

using namespace std;

int main() {
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


  LetterBag bag;

  for(int i = 0; i < 102; ++i) {
      cout << "NbLetters in bag : " << bag.getNbLetters() << endl;
      cout << "Random :" << bag.pickRandomLetter() << endl;
  }

  for(char a = 'A'; a <= 'Z'; a++) {
      cout << "Get score : " << LetterBag::getScore(a) << endl;
  }

  return 0 ;
}
