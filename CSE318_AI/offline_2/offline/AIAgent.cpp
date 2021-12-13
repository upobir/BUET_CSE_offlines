#include "AIAgent.hpp"

namespace AI {
    AIAgent::AIAgent(Heuristics const& _heuristics, int _maxDepth) 
        : heuristics(_heuristics) , maxDepth(_maxDepth) {
    }

    int AIAgent::getMove(Mancala const& mancala) {
        return alphaBetaPrune(mancala, INT_MIN, INT_MAX, 0).second;
    }

    std::string AIAgent::getName() const {
        return "AI with " + heuristics.getName();
    }


    std::pair<int, int> AIAgent::alphaBetaPrune(Mancala const& mancala, int alpha, int beta, int depth) const {
        if(mancala.isEnded()){
            return {mancala.getScore(), -1};
        }
        else if(depth == maxDepth){
            return {heuristics.getHeuristics(mancala), -1};
        }

        std::pair<int, int> result = {(mancala.getCurrentPlayer() == 1? INT_MIN : INT_MAX), 1};

        for(int col = 1; col<=6; col++){ /// TODO change order
            if(!mancala.isValidMove(col))
                continue;

            Mancala newMancala(mancala);
            newMancala.applyMove(col);

            auto returned = alphaBetaPrune(newMancala, alpha, beta, depth+1);

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