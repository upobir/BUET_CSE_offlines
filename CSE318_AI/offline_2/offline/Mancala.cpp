#include "Mancala.hpp"

#include <cassert>
#include <algorithm>
#include <numeric>

#include "Logger.hpp"

namespace AI {
    Mancala::Mancala() {
        bowl[0] = bowl[1] = 0;
        for(int row = 0; row < 2; row++){
            for(int col = 0; col < 6; col++){
                pockets[row][col] = 4;
            }
        }
        currentPlayer = 1;
    }


    Mancala::Mancala(Mancala const& mancala) {
        currentPlayer = mancala.currentPlayer;
        bowl[0] = mancala.bowl[0];
        bowl[1] = mancala.bowl[1];
        for(int row = 0; row < 2; row++){
            for(int col = 0; col < 6; col++){
                pockets[row][col] = mancala.pockets[row][col];
            }
        }
    }

    bool Mancala::isEnded() const {
        if(std::count(pockets[0], pockets[0]+6, 0) == 6)
            return true;
        else if(std::count(pockets[1], pockets[1]+6, 0) == 6)
            return true;
        return false;
    }


    int Mancala::getCurrentPlayer() const {
        return currentPlayer;
    }


    void Mancala::setWinConfig() {
        int first = std::accumulate(pockets[1], pockets[1]+6, 0);
        bowl[0] += first;
        int second = std::accumulate(pockets[0], pockets[0]+6, 0);
        bowl[1] += second;

        for(int row  = 0; row < 2; row++){
            for(int col = 0; col<6; col++){
                pockets[row][col] = 0;
            }
        }
    }


    int Mancala::getWinner() const {
        int diff = getScore();

        if(diff == 0){
            return 0;
        }
        else if(diff > 0){
            return 1;
        }
        else{
            return 2;
        }
    }


    int Mancala::getScore() const {
        int first = std::accumulate(pockets[1], pockets[1]+6, 0) + bowl[0];
        int second = std::accumulate(pockets[0], pockets[0]+6, 0) + bowl[1];

        return first - second;
    }


    int Mancala::getBowlStoneCount(int player) const {
        return bowl[player-1];
    }


    int Mancala::getPocketStoneCount(int row, int column) const {
        return pockets[row-1][column-1];
    }


    bool Mancala::isValidMove(int column) const {
        column--;
        int row = (currentPlayer == 1? 1 : 0);
        return pockets[row][column] > 0;
    }


    std::tuple<Mancala::Result, int, int> Mancala::applyMove(int column) {
        column--;
        int row = (currentPlayer == 1? 1 : 0);
        int dir = (row == 0? -1 : 1);
        int stones = pockets[row][column];
        assert(stones > 0);
        pockets[row][column] = 0;

        while(stones > 0){
            int nextcol = column + dir;
            if(nextcol > 5 || nextcol < 0){
                if(row == (currentPlayer == 1? 1 : 0)){
                    bowl[currentPlayer-1]++;
                    stones--;
                }

                row ^= 1;
                dir = -dir;
                column = nextcol;
            }
            else{
                column = nextcol;
                pockets[row][column]++;
                stones--;
            }
        }

        
        if(column < 0 || column > 5){
            return std::make_tuple(Result::BONUS, -1, -1);
        }
        else if(row == (currentPlayer == 1? 1 : 0) && pockets[row][column] == 1 && pockets[row^1][column] > 0){
            bowl[currentPlayer-1] += pockets[row][column] + pockets[row^1][column];
            pockets[row][column] = 0;
            pockets[row^1][column] = 0;
            currentPlayer = (currentPlayer == 1? 2 : 1);
            return std::make_tuple(Result::STEAL, row+1, column+1);
        }
        else{
            currentPlayer = (currentPlayer == 1? 2 : 1);
            return std::make_tuple(Result::NORMAL, row+1, column+1);
        }
    }
}