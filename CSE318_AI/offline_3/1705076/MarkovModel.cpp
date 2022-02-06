#include "MarkovModel.hpp"

#include <cassert>
#include <cmath>
#include <iostream>


MarkovModel::MarkovModel(std::pair<int, int> bounds, std::vector< std::pair<int, int> > const& obstacles, double adjProb, double correctProb){
    rowCount = bounds.first;
    assert(rowCount > 0);

    colCount = bounds.second;
    assert(colCount > 0);

    auto obstacleMap = createObstacleMap(obstacles);
    initBelief(obstacleMap);
    initTransition(obstacleMap, adjProb);
    initConfidence(obstacleMap, correctProb);
}


int
MarkovModel::mapToIndex(int row, int col) const {
    return row * colCount + col;
}


ObstacleMap 
MarkovModel::createObstacleMap(std::vector<Position> const& obstacles){
    ObstacleMap obstacleMap(rowCount, std::vector<bool>(colCount) );

    for(auto [row, col] : obstacles){
        assert(0 <= row && row < rowCount);
        assert(0 <= col && col < colCount);

        obstacleMap[row][col] = true;
    }
    return obstacleMap;
}


void
MarkovModel::initBelief(ObstacleMap const& obstacleMap) {

    int freeCellCount = 0;
    for(int row = 0; row < rowCount; row++){
        for(int col = 0; col < colCount; col++){
            if(!obstacleMap[row][col])
                freeCellCount++;
        }
    }

    belief.resize(rowCount, std::vector<double>(colCount));
    for(int row = 0; row < rowCount; row++){
        for(int col = 0; col < colCount; col++){
            if(!obstacleMap[row][col])
                belief[row][col] = 1.0/double(freeCellCount);
        }
    }
    return;
}


void
MarkovModel::initTransition(ObstacleMap const& obstacleMap, double adjProb){
    int cellCount = rowCount * colCount;
    transition.resize(cellCount, std::vector<double>(cellCount)); 

    for(int row = 0; row < rowCount; row++){
        for(int col = 0; col < colCount; col++){
            if(obstacleMap[row][col])
                continue;

            std::vector<Position> adjacents;
            std::vector<Position> nonadjacents;

            for(int dr = -1; dr <= 1; dr++){
                for(int dc = -1; dc <=1; dc++){
                    if(row+dr < 0 || row+dr >= rowCount)
                        continue;
                    if(col+dc < 0 || col+dc >= colCount)
                        continue;
                    if(obstacleMap[row+dr][col+dc])
                        continue;

                    if(std::abs(dr) + std::abs(dc) == 1)
                        adjacents.emplace_back(row+dr, col+dc);
                    else
                        nonadjacents.emplace_back(row+dr, col+dc);
                }
            }

            assert(adjacents.size() != 0);

            for(auto [row2, col2]: adjacents){
                transition[mapToIndex(row, col)][mapToIndex(row2, col2)] = adjProb/double(adjacents.size());
            }

            for(auto [row2, col2]: nonadjacents){
                transition[mapToIndex(row, col)][mapToIndex(row2, col2)] = (1.0-adjProb)/double(nonadjacents.size());
            }
        }
    }

    /*for(int i = 0; i<transition.size(); i++){
        for(int j = 0;j<transition.size(); j++){
            std::cout<<transition[i][j]<<" ";
        }
        std::cout<<std::endl;
    }*/

    return;
}


void 
MarkovModel::initConfidence(ObstacleMap const& obstalceMap, double correctProb){
    int cellCount = rowCount * colCount;
    confidence[0].resize(cellCount, std::vector<double>(cellCount));
    confidence[1].resize(cellCount, std::vector<double>(cellCount));

    for(int row = 0; row < rowCount; row++){
        for(int col = 0; col<colCount; col++){
            for(int row2 = 0; row2 < rowCount;row2++){
                for(int col2 = 0; col2 < colCount; col2++){
                    if(std::abs(row-row2) <= 1 && std::abs(col-col2) <= 1){
                        confidence[1][mapToIndex(row, col)][mapToIndex(row2, col2)] = correctProb;
                        confidence[0][mapToIndex(row, col)][mapToIndex(row2, col2)] = 1.0-correctProb;
                    }
                    else{
                        confidence[1][mapToIndex(row, col)][mapToIndex(row2, col2)] = 1.0-correctProb;
                        confidence[0][mapToIndex(row, col)][mapToIndex(row2, col2)] = correctProb;
                    }
                }
            }
        }
    }

    /*for(int i = 0; i<confidence[0].size(); i++){
        for(int j = 0;j<confidence[0].size(); j++){
            std::cout<<confidence[0][i][j]<<" ";
        }
        std::cout<<std::endl;
    }*/

    return;
}


int 
MarkovModel::getRowCount() const {
    return rowCount;
}


int 
MarkovModel::getColCount() const {
    return colCount;
}


std::pair<int, int> 
MarkovModel::getEstimatedLocation() const {
    Position pos;
    for(int row = 0; row < rowCount; row++){
        for(int col = 0; col < colCount; col++){
            if(belief[row][col] > belief[pos.first][pos.second])
                pos = {row, col};
        }
    }
    return pos;
}


double
MarkovModel::getProbability(int row, int col) const {
    assert(0 <= row && row < rowCount);
    assert(0 <= col && col < colCount);

    return belief[row][col];
}


void
MarkovModel::updateBelief(int sensorRow, int sensorCol, bool result){
    ProbTable temp(rowCount, std::vector<double>(colCount));
    for(int row = 0; row < rowCount; row++){
        for(int col = 0; col < colCount; col++){
            for(int row2 = 0; row2 < rowCount; row2++){
                for(int col2 = 0; col2 < colCount; col2++){
                    temp[row][col] += belief[row2][col2] * transition[mapToIndex(row2, col2)][mapToIndex(row, col)];
                }
            }
        }
    }
    
    double sum = 0;
    for(int row = 0; row < rowCount; row++){
        for(int col = 0; col < colCount; col++){
            belief[row][col] = confidence[result][mapToIndex(sensorRow, sensorCol)][mapToIndex(row, col)] * temp[row][col];
            sum += belief[row][col];
        }
    }

    for(int row = 0; row < rowCount; row++){
        for(int col = 0; col < colCount; col++){
            belief[row][col] /= sum;
        }
    }
    return;
}
