#include <iostream>
#include <string>
#include <sstream>

#include "suzettecheck.hpp"
#include "httplib.hpp"
#include "json.hpp"
#include "board.hpp"

using namespace std;

namespace {

    string urlencode(const std::string& str) {
        static const char lookup[]= "0123456789abcdef";
        std::stringstream e;
        for(unsigned long i = 0, ix = str.length(); i < ix; i++) {
            const char& c = str[i];
            if ( (48 <= c && c <= 57) ||
                 (65 <= c && c <= 90) ||
                 (97 <= c && c <= 122) ||
                 (c=='-' || c=='_' || c=='.' || c=='~')
            ){
                e << c;
            }
            else{
                e << '%';
                e << lookup[ (c&0xF0)>>4 ];
                e << lookup[ (c&0x0F) ];
            }
        }
        return e.str();
    }

}

pair<unsigned int, Board> Suzette::check(const Board& board,
                                         const PlayerBag& letters) {
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

    if(res && res->status == 200) {
        auto json = nlohmann::json::parse(res->body);
        stringstream ss;
        ss << json["board"];

        return { json["score"], Board(ss) };
    }
    else {
        cout << "/!\\ Problem to join Suzette server, Abort.../!\\" << endl;
        exit(EXIT_FAILURE);
    }
}
