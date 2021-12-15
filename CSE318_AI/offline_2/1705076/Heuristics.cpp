#include "Heuristics.hpp"

#include <cassert>

namespace AI {
    int Heuristics1::getHeuristics(Mancala const& mancala, int extras) const {
        return mancala.getBowlStoneCount(1) - mancala.getBowlStoneCount(2);
    }


    std::string Heuristics1::getName() const {
        return "heuristics 1";
    }


    int Heuristics2::getHeuristics(Mancala const& mancala, int extras) const {
        const int W1 = 10;
        const int W2 = 1;
        int term1 = mancala.getBowlStoneCount(1) - mancala.getBowlStoneCount(2);
        int term2 = 0;
        for(int col = 1; col<=6; col++){
            term2 += mancala.getPocketStoneCount(2, col) - mancala.getPocketStoneCount(1, col);
        }

        return W1*term1 + W2*term2;
    }


    std::string Heuristics2::getName() const {
        return "heuristics 2";
    }


    int Heuristics3::getHeuristics(Mancala const& mancala, int extras) const {
        const int W1 = 10;
        const int W2 = 2;
        const int W3 = 1;
        int term1 = mancala.getBowlStoneCount(1) - mancala.getBowlStoneCount(2);
        int term2 = 0;
        for(int col = 1; col<=6; col++){
            term2 += mancala.getPocketStoneCount(2, col) - mancala.getPocketStoneCount(1, col);
        }

        return W1*term1 + W2*term2 + W3*extras;
    }
    std::string Heuristics3::getName() const {
        return "heuristics 3";
    }


    int Heuristics4::getHeuristics(Mancala const& mancala, int extras) const {
        int score = 0;
        int w[] = {10, 20, 30, 40, 50, 60};
        for(int col = 6, i = 0; col >= 1; col--, i++){
            score += mancala.getPocketStoneCount(2, col) * w[i];
        }

        for(int col = 1, i = 0; col <= 6; col++, i++){
            score += mancala.getPocketStoneCount(1, col) * w[i];
        }

        return score + mancala.getBowlStoneCount(1) - mancala.getBowlStoneCount(2);
    }

    std::string Heuristics4::getName() const {
        return "heuristics 4";
    }


    int Heuristics5::getHeuristics(Mancala const& mancala, int extras) const {
        int score = 0;
        int w[] = {60, 50, 40, 30, 20, 10};
        for(int col = 6, i = 0; col >= 1; col--, i++){
            score += mancala.getPocketStoneCount(2, col) * w[i];
        }

        for(int col = 1, i = 0; col <= 6; col++, i++){
            score += mancala.getPocketStoneCount(1, col) * w[i];
        }

        return score + mancala.getBowlStoneCount(1) - mancala.getBowlStoneCount(2);
    }


    std::string Heuristics5::getName() const {
        return "heuristics 5";
    }


    int Heuristics6::getHeuristics(Mancala const& mancala, int extras) const {
        int score = 0;
        for(int col = 6, i = 0; col >=1; col--, i++){
            int x = mancala.getPocketStoneCount(2, col);
            x -= i;
            if(x > 0){
                score += 1;
                x--;
                score += x/13;
            }
        }


        for(int col = 1, i = 0; col <=6; col++, i++){
            int x = mancala.getPocketStoneCount(2, col);
            x -= i;
            if(x > 0){
                score -= 1;
                x--;
                score -= x/13;
            }
        }

        return score + mancala.getBowlStoneCount(1) - mancala.getBowlStoneCount(2);
    }
    std::string Heuristics6::getName() const {
        return "heuristics 6";
    }
}