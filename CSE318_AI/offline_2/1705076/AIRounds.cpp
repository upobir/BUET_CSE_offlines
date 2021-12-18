#include "AIRounds.hpp"
#include "Mancala.hpp"
#include "Logger.hpp"

#include <cassert>

namespace AI {
    int cnt = 0;

    AIRounds::AIRounds(AIAgent const& _agent1, AIAgent const& _agent2) 
        : agent1(_agent1), agent2(_agent2) {
    }


    std::tuple<int, int, int> AIRounds::run(int roundCnt) const {
        std::tuple<int, int, int> result;
        for(int i = 0; i<roundCnt; i++){
            int gameResult = runOneRound();
            debug(cnt);
            cnt++;
            switch(gameResult){
                case 1:
                    std::get<0>(result)++;
                    break;
                case 2:
                    std::get<1>(result)++;
                    break;
                case 0:
                    std::get<2>(result)++;
                    break;
                default:
                    assert(!"illegal code path");
                    break;
            }
        }

        return result;
    }


    int AIRounds::runOneRound() const {
        Mancala mancala;

        while(!mancala.isEnded()){
            int move;
            if(mancala.getCurrentPlayer() == 1){
                move = agent1.getMove(mancala);
            }
            else{
                move = agent2.getMove(mancala);
            }

            mancala.applyMove(move);
        }

        return mancala.getWinner();
    }
}