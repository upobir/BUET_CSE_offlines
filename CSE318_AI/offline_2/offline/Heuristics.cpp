#include "Heuristics.hpp"

#include <cassert>

namespace AI {
    int Heuristics1::getHeuristics(Mancala const& mancala) const {
        return mancala.getBowlStoneCount(1) - mancala.getBowlStoneCount(2);
    }

    std::string Heuristics1::getName() const {
        return "heuristics 1";
    }
}