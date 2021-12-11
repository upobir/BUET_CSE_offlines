#pragma once

#include "Mancala.hpp"

#include <string>

namespace AI {
    class MancalaAgent {
    private:
    protected:
        MancalaAgent() = default;
    public:
        virtual int getMove(Mancala const& mancala) = 0;

        virtual std::string getName() const = 0;
    };
}