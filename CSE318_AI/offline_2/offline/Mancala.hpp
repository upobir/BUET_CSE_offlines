#pragma once

#include <tuple>

namespace AI {
    class Mancala{
    private:
        int bowl[2];
        int pockets[2][6];
        int currentPlayer;
    public:
        Mancala();
        Mancala(Mancala const& mancala);

        bool isEnded() const;
        int getCurrentPlayer() const;
        int getWinner() const;
        void setWinConfig() ;

        int getBowlStoneCount(int player) const;
        int getPocketStoneCount(int row, int column) const;

        enum class Result {
            NORMAL,
            BONUS,
            STEAL,
        };

        std::tuple<Result, int, int> applyMove(int column);
    };
}