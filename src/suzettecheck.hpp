#ifndef SUZETTECHECK_HPP
#define SUZETTECHECK_HPP

#include <string>

#include "board.hpp"
#include "playerbag.hpp"

namespace Suzette {
    ///
    /// \brief this function Check if a Move is the best Move but with send
    ///     a request to the Suzette Bot of Mr NIVOLIERS, wich run on a Server at UCBL1.
    ///     it can help to check the validity of the program.
    ///     Please refer to the README.md (gitlab) for enalble this option.
    ///     You should run a little script to go behind the university firewall.
    ///     This function Use two third-parties librairies in header-only form.
    ///     These librairies are in the Project (httplib.hpp and json.hpp).
    ///     You don't need to install them
    /// \param board The Board to solve with Suzette
    /// \param letters The PlayerBag winch contains available letters for the move
    /// \return a pair : first is the Score find by Suzette and second is the Board
    ///     by Suzette
    ///
    std::pair<unsigned int, Board> check(const Board& board,
                                         const PlayerBag& letters);
}

#endif // SUZETTECHECK_HPP
