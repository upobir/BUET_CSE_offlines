#pragma once

#include "MancalaAgent.hpp"

namespace AI {
    class UserAgent : public MancalaAgent {
    private:
    public:
        UserAgent();

        int getMove(Mancala const& mancala) override;

        std::string getName() const override;
    };
}