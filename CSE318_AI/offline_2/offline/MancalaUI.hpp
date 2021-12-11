#pragma once

#include "Mancala.hpp"
#include "MancalaAgent.hpp"

namespace AI {
    class MancalaUI {
    private:
        Mancala mancala;
        MancalaAgent& firstPlayer;
        MancalaAgent& secondPlayer;
    public:
        MancalaUI(MancalaAgent& _firstPlayer, MancalaAgent& _secondPlayer);

        void run();

        void printBoard();
    };
}