#include <iostream>

#include "Logger.hpp"

#include "MancalaUI.hpp"
#include "UserAgent.hpp"
#include "AIAgent.hpp"
#include "Heuristics.hpp"
#include "AIRounds.hpp"

int main(int argc, char** argv){
    AI::UserAgent user;
    AI::Heuristics1 h1;
    AI::Heuristics2 h2;
    AI::Heuristics3 h3;
    AI::Heuristics4 h4;
    

    AI::AIAgent agents[] = {
        AI::AIAgent(h1, 10),
        AI::AIAgent(h2, 10),
        AI::AIAgent(h3, 10),
        AI::AIAgent(h4, 10),
    };

    int aicount = sizeof(agents)/sizeof(*agents);


    for(int i = 0; i<aicount; i++){
        for(int j = 0; j<i; j++){
            AI::AIRounds rounds(agents[i], agents[j]);
            auto result = rounds.run(5);
            std::cout<<"First player  : "<<agents[i].getName()<<std::endl;
            std::cout<<"Second player : "<<agents[j].getName()<<std::endl;

            std::cout<<"First player won  : "<<std::get<0>(result)<<std::endl;
            std::cout<<"Second player won : "<<std::get<1>(result)<<std::endl;
            std::cout<<"Draws             : "<<std::get<2>(result)<<std::endl;
        }
    }



    //AI::MancalaUI mancala(agents[1], user);
    //mancala.run();

    return 0;
}