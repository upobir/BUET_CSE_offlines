#pragma once

#include <random>
#include <utility>
#include <vector>

#include "MarkovModel.hpp"

class MarkovSimulation{
public:
    MarkovSimulation(int rowCount_, int colCount_, std::vector<Position> const& obstacles, double adjProb_, double correctProb_);

    void updatePosition();
    Position getRandomPosition();
    bool getSensorResult(Position position) ;
    Position getCasperPosition() const;

private:
    int rowCount;
    int colCount;
    ObstacleMap obstacleMap;
    Position casper;
    double adjProb;
    double correctProb;

    std::mt19937 rng;
    std::uniform_int_distribution<int> rowDist;
    std::uniform_int_distribution<int> colDist;
    std::uniform_real_distribution<double> probDist;
};
