#pragma once

#include "MancalaAgent.hpp"
#include "Heuristics.hpp"

#include <utility>

namespace AI {
    class AIAgent : public MancalaAgent {
    private:
        Heuristics const& heuristics;
        int maxDepth;

        std::pair<int, int> alphaBetaPrune(Mancala const& mancala, int alpha, int beta, int depth) const;
    public:
        AIAgent(Heuristics const& _heuristics, int _maxDepth);

        int getMove(Mancala const& mancala) override;

        std::string getName() const override;
    };
}