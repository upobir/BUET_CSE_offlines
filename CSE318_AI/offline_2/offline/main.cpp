#include <iostream>

#include "Logger.hpp"

#include "MancalaUI.hpp"
#include "UserAgent.hpp"

int main(int argc, char** argv){
    AI::UserAgent agent1, agent2;
    AI::MancalaUI mancala(agent1, agent2);

    mancala.run();

    return 0;
}