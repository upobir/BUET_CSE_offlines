#pragma once

#include "AIAgent.hpp"

#include <tuple>

namespace AI {
    class AIRounds {
    private:
        AIAgent const& agent1;
        AIAgent const& agent2;

        int runOneRound() const;
    public:
        AIRounds(AIAgent const& _agent1, AIAgent const& _agent2);

        std::tuple<int, int, int> run(int roundCnt) const;
    };
}