#pragma once

#include "Mancala.hpp"

#include <string>

namespace AI {
    class Heuristics {
    private:
    protected:
        Heuristics() = default;

    public:
        virtual int getHeuristics(Mancala const& mancala) const = 0;

        virtual std::string getName() const = 0;
    };


    class Heuristics1 : public Heuristics {
    public:
        Heuristics1() = default;

        int getHeuristics(Mancala const& mancala) const override;

        std::string getName() const override;
    };
}