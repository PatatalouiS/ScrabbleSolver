#include <iostream>
#include <string>
#include <sstream>

#include "suzettecheck.hpp"
#include "httplib.hpp"
#include "json.hpp"
#include "board.hpp"
#include "utils.hpp"

using namespace std;

pair<unsigned int, Board> Suzette::check(const Board& board,
                                         const PlayerBag& letters) {
    // Save the board in a string representation
    stringstream boardStream;
    board.save(boardStream);
    // Encode the params before send the GET Query to Suzette Server
    string boardQueryParam = Utils::urlencode(boardStream.str());
    string rackQueryParam = Utils::urlencode(string(letters.data().begin(),
                                             letters.data().end()));
    std::string server = "localhost";
    httplib::Client cli(server, 8080);
    std::string url("/solve");

    url.append("?board=").append(boardQueryParam)
       .append("&rack=").append(rackQueryParam);
    // Send the query
    auto res = cli.Get(url.c_str());
    // If sucess ( http reponse 200 )
    if(res && res->status == 200) {
        // parse the json to a std multimap
        auto json = nlohmann::json::parse(res->body);
        stringstream ss;
        ss << json["board"];

        return { json["score"], Board(ss) };
    }
    else { // Error handling
        cout << "/!\\ Problem to join Suzette server, Abort.../!\\" << endl;
        exit(EXIT_FAILURE);
    }
}
