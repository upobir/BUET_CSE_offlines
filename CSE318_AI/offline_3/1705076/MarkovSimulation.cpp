#include "MarkovSimulation.hpp"

#include <cassert>
#include <chrono>


MarkovSimulation::MarkovSimulation(int rowCount_, int colCount_, std::vector<Position> const& obstacles, double adjProb_, double correctProb_){
    rowCount = rowCount_;
    assert(rowCount >= 0);

    colCount = colCount_;
    assert(colCount >= 0);

    adjProb = adjProb_;
    correctProb = correctProb_;
    
    obstacleMap.resize(rowCount, std::vector<bool>(colCount));

    for(auto [row, col]: obstacles){
        obstacleMap[row][col] = true;
    }

    rng = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    rowDist = std::uniform_int_distribution<int>(0, rowCount-1);
    colDist = std::uniform_int_distribution<int>(0, colCount-1);
    probDist = std::uniform_real_distribution<double>(0.0, 1.0);

    while(true){
        casper = {rowDist(rng), colDist(rng)};
        if( ! obstacleMap[casper.first][casper.second])
            break;
    }
}


void
MarkovSimulation::updatePosition() {
    bool adjacent = probDist(rng) < adjProb; 

    std::vector<Position> candidates;
    for(int dr = -1; dr <= 1; dr++){
        for(int dc = -1; dc <= 1; dc++){
            if(adjacent == (std::abs(dr) + std::abs(dc) == 1)){
                int row = casper.first + dr;
                int col = casper.second + dc;
                
                if(0 <= row && row < rowCount &&
                   0 <= col && col < colCount &&
                   ! obstacleMap[row][col] ){

                    candidates.push_back({row, col});
                }
            }
        }
    }

    casper = candidates[std::uniform_int_distribution<int>(0, int(candidates.size())-1)(rng)];
    return;
}


Position
MarkovSimulation::getRandomPosition() {
    return {rowDist(rng), colDist(rng)};
}


bool
MarkovSimulation::getSensorResult(Position position) {
    bool result = std::abs(casper.first - position.first) <=1 && std::abs(casper.second - position.second) <= 1;
    if(probDist(rng) > correctProb)
        result = ! result;
    return result;
}


Position
MarkovSimulation::getCasperPosition() const {
    return casper;
}
