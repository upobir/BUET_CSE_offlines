#include <iostream>

#include "Logger.hpp"

#include "MancalaUI.hpp"
#include "UserAgent.hpp"
#include "AIAgent.hpp"
#include "Heuristics.hpp"
#include "AIRounds.hpp"

int main(int argc, char** argv){
    AI::Heuristics1 h1;
    AI::Heuristics2 h2;
    AI::Heuristics3 h3;
    AI::Heuristics4 h4;
    AI::Heuristics5 h5;
    AI::Heuristics6 h6;

    std::cout<<"Play ai tournament? :";
    char cmd;
    std::cin>>cmd;

    if(tolower(cmd) == 'y'){
        AI::AIAgent agents[] = {
            AI::AIAgent(h1, 10),
            AI::AIAgent(h2, 10),
            AI::AIAgent(h3, 10),
            AI::AIAgent(h4, 10),
            AI::AIAgent(h5, 10),
            AI::AIAgent(h6, 10),
        };

    const int aicount = sizeof(agents)/sizeof(*agents);
        char wintable[aicount][aicount];

        for(int i = 0; i<aicount; i++){
            for(int j = 0; j<aicount; j++){
                AI::AIRounds rounds(agents[i], agents[j]);
                auto result = rounds.run(50);
                std::cout<<"First player  : "<<agents[i].getName()<<std::endl;
                std::cout<<"Second player : "<<agents[j].getName()<<std::endl;

                std::cout<<"First player won  : "<<std::get<0>(result)<<std::endl;
                std::cout<<"Second player won : "<<std::get<1>(result)<<std::endl;
                std::cout<<"Draws             : "<<std::get<2>(result)<<std::endl;

                if(std::get<0>(result) > 0){
                    wintable[i][j] = 'W';
                }
                else if(std::get<1>(result) > 0){
                    wintable[i][j] = 'L';
                }
                else{
                    wintable[i][j] = 'D';
                }
            }
        }

        std::cout<<' ';
        for(int i = 0; i<aicount; i++){
            std::cout<<' '<<i+1;
        }
        std::cout<<std::endl;

        for(int i = 0; i<aicount; i++){
            std::cout<<i+1;
            for(int j = 0; j<aicount; j++){
                std::cout<<' '<<wintable[i][j];
            }
            std::cout<<std::endl;
        }
    }
    else{
        auto getAgent = [&](){
            std::cout<<"1. User"<<std::endl;
            std::cout<<"2. AI with heuristcs 1"<<std::endl;
            std::cout<<"3. AI with heuristcs 2"<<std::endl;
            std::cout<<"4. AI with heuristcs 3"<<std::endl;
            std::cout<<"5. AI with heuristcs 4"<<std::endl;
            std::cout<<"6. AI with heuristcs 5"<<std::endl;
            std::cout<<"7. AI with heuristcs 6"<<std::endl;
            std::cout<<"> ";

            int type;
            do{
                std::cin>>type;
            } while(type < 1 || type > 7);
            AI::MancalaAgent* agent;

            switch (type) {
            case 1 :
                agent = new AI::UserAgent;
                break;
            case 2 :
                agent = new AI::AIAgent(h1, 10);
                break;
            case 3 :
                agent = new AI::AIAgent(h2, 10);
                break;
            case 4 :
                agent = new AI::AIAgent(h3, 10);
                break;
            case 5 :
                agent = new AI::AIAgent(h4, 10);
                break;
            case 6 :
                agent = new AI::AIAgent(h5, 10);
                break;
            case 7 :
                agent = new AI::AIAgent(h6, 10);
                break;
            }

            return agent;
        };

        std::cout<<"Enter first player type:"<<std::endl;
        auto player1 = getAgent();

        std::cout<<"Enter second player type:"<<std::endl;
        auto player2 = getAgent();

        AI::MancalaUI mancala(*player1, *player2);
        mancala.run();

        delete player1;
        delete player2;
    }

    return 0;
}