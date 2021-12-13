#include <iostream>

#include "Logger.hpp"

#include "MancalaUI.hpp"
#include "UserAgent.hpp"
#include "AIAgent.hpp"
#include "Heuristics.hpp"

int main(int argc, char** argv){
    AI::UserAgent user;
    AI::Heuristics1 h;
    AI::AIAgent ai(h, 6);

    AI::MancalaUI mancala(ai, user);

    mancala.run();

    return 0;
}