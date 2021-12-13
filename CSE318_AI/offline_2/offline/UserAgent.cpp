#include "UserAgent.hpp"

#include <cassert>
#include <iostream>

namespace AI {
    UserAgent::UserAgent() {
    }

    int UserAgent::getMove(Mancala const& mancala) {
        int col;
        do{
            std::cout<<">> Enter column number for move: ";
            std::cin>>col;
        } while (1 > col || col > 6 || !mancala.isValidMove(col));
        
        return col;
    }

    std::string UserAgent::getName() const {
        return "user";
    }
}