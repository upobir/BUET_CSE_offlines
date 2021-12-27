#pragma once

#include "MancalaAgent.hpp"

namespace AI {
    class UserAgent : public MancalaAgent {
    private:
    public:
        UserAgent();

        int getMove(Mancala const& mancala) const override;

        std::string getName() const override;

        ~UserAgent() override = default;
    };
}