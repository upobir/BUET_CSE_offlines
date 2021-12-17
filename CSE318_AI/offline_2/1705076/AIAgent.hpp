#pragma once

#include "MancalaAgent.hpp"
#include "Heuristics.hpp"

#include <utility>
#include <random>

namespace AI {
    class AIAgent : public MancalaAgent {
    private:
        Heuristics const& heuristics;
        int maxDepth;
        mutable std::mt19937 rng;

        std::pair<int, int> alphaBetaPrune(Mancala const& mancala, int alpha, int beta, int depth, int extras) const;


    public:
        AIAgent(Heuristics const& _heuristics, int _maxDepth);

        int getMove(Mancala const& mancala) const override;

        std::string getName() const override;

        ~AIAgent() override = default;
    };
}