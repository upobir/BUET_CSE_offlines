#include "UserAgent.hpp"

#include <cassert>
#include <iostream>

namespace AI {
    UserAgent::UserAgent() {
    }

    int UserAgent::getMove(Mancala const& mancala) {
        std::cout<<"Enter column number for move: "; // TODO loop to get proper move?
        int col;
        std::cin>>col;
        return col;
    }

    std::string UserAgent::getName() const {
        return "user";
    }
}