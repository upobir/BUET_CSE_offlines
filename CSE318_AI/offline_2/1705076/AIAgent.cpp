#include "AIAgent.hpp"

#include <string>
#include <climits>
#include <algorithm>
#include <chrono>

namespace AI {

    AIAgent::AIAgent(Heuristics const& _heuristics, int _maxDepth) 
        : heuristics(_heuristics) , maxDepth(_maxDepth), rng(std::chrono::system_clock::now().time_since_epoch().count()) {
        rng();
    }

    int AIAgent::getMove(Mancala const& mancala) const {
        return alphaBetaPrune(mancala, INT_MIN, INT_MAX, 0, 0).second;
    }

    std::string AIAgent::getName() const {
        return "AI with " + heuristics.getName() + " with max depth " + std::to_string(maxDepth);
    }


    std::pair<int, int> AIAgent::alphaBetaPrune(Mancala const& mancala, int alpha, int beta, int depth, int extras) const {
        if(mancala.isEnded()){
            return {mancala.getScore(), -1};
        }
        else if(depth == maxDepth){
            return {heuristics.getHeuristics(mancala, extras), -1};
        }

        std::pair<int, int> result = {(mancala.getCurrentPlayer() == 1? INT_MIN : INT_MAX), 1};

        int start = (mancala.getCurrentPlayer() == 1? 6 : 1);
        int dir = (mancala.getCurrentPlayer() == 1? -1 : +1);
        int finish = (mancala.getCurrentPlayer() == 1? 0 : 7);
        for(int col = start; col != finish; col+=dir){

        // int perm[] = {1, 2, 3, 4, 5, 6};
        // std::shuffle(perm, perm+6, rng);
        // for(int i = 0; i<6; i++){
        //     int col = perm[i];
        
            if(!mancala.isValidMove(col))
                continue;

            Mancala newMancala(mancala);
            newMancala.applyMove(col);

            int newExtras = extras;
            if(mancala.getCurrentPlayer() == newMancala.getCurrentPlayer()){
                newExtras += (mancala.getCurrentPlayer() == 1? +1 : -1);
            }

            auto returned = alphaBetaPrune(newMancala, alpha, beta, depth+1, newExtras);

            if(mancala.getCurrentPlayer() == 1){
                if(returned.first > result.first)
                    result = {returned.first, col};
                alpha = (alpha > result.first? alpha : result.first);
            }
            else{
                if(returned.first < result.first)
                    result = {returned.first, col};
                beta = (beta < result.first? beta : result.first);
            }

            if(beta <= alpha) break;
        }

        return result;
    }
}