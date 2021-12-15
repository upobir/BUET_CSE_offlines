#pragma once

#include "Mancala.hpp"

#include <string>

namespace AI {
    class Heuristics {
    private:
    protected:
        Heuristics() = default;

    public:
        virtual int getHeuristics(Mancala const& mancala, int extramoves) const = 0;

        virtual std::string getName() const = 0;
    };


    class Heuristics1 : public Heuristics {
    public:
        Heuristics1() = default;

        int getHeuristics(Mancala const& mancala, int extramoves) const override;

        std::string getName() const override;
    };


    class Heuristics2 : public Heuristics {
    public:
        Heuristics2() = default;

        int getHeuristics(Mancala const& mancala, int extramoves) const override;

        std::string getName() const override;
    };


    class Heuristics3 : public Heuristics {
    public:
        Heuristics3() = default;

        int getHeuristics(Mancala const& mancala, int extramoves) const override;

        std::string getName() const override;
    };


    class Heuristics4 : public Heuristics {
    public:
        Heuristics4() = default;

        int getHeuristics(Mancala const& mancala, int extramoves) const override;

        std::string getName() const override;
    };


    class Heuristics5 : public Heuristics {
    public:
        Heuristics5() = default;

        int getHeuristics(Mancala const& mancala, int extramoves) const override;

        std::string getName() const override;
    };


    class Heuristics6 : public Heuristics {
    public:
        Heuristics6() = default;

        int getHeuristics(Mancala const& mancala, int extramoves) const override;

        std::string getName() const override;
    };
}