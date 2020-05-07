#include "suzettecheck.hpp"

#include "httplib.hpp"
#include "json.hpp"
#include "board.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

namespace {

    std::string urlencode(const std::string &s) {
        static const char lookup[]= "0123456789abcdef";
        std::stringstream e;
        for(unsigned long i = 0, ix = s.length(); i < ix; i++)
        {
            const char& c = s[i];
            if ( (48 <= c && c <= 57) ||
                 (65 <= c && c <= 90) ||
                 (97 <= c && c <= 122) ||
                 (c=='-' || c=='_' || c=='.' || c=='~')
            )
            {
                e << c;
            }
            else
            {
                e << '%';
                e << lookup[ (c&0xF0)>>4 ];
                e << lookup[ (c&0x0F) ];
            }
        }
        return e.str();
    }

}

void Suzette::check(const Board& board, const PlayerBag& letters) {
    stringstream boardStream;
    board.save(boardStream);

    string boardQueryParam = urlencode(boardStream.str());
    string rackQueryParam = urlencode(string(letters.data().begin(),
                                             letters.data().end()));

    std::string server = "localhost";
    httplib::Client cli(server, 8080);
    std::string url("/solve");
    url.append("?board=").append(boardQueryParam)
       .append("&rack=").append(rackQueryParam);

    auto res = cli.Get(url.c_str());

    auto j3 = nlohmann::json::parse(res->body);
    std::cout << "Score by Suzette : " << j3["score"] << std::endl;
    //std::cout << "New Board by Suzette : " << j3["board"] << std:: endl;
}
