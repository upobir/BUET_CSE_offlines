#pragma once

#include <utility>
#include <vector>


using ProbTable = std::vector<std::vector<double>>;
using ObstacleMap = std::vector<std::vector<bool>>;
using Position = std::pair<int, int>;


class MarkovModel {
public:
    MarkovModel(std::pair<int, int> bounds, std::vector<Position> const& obstacles, double adjProb, double correctProb);

    int getRowCount() const;
    int getColCount() const;

    Position getEstimatedLocation() const;

    double getProbability(int row, int col) const;

    void updateBelief(int row, int col, bool result);

private:
    int rowCount;
    int colCount;
    ProbTable belief;
    ProbTable transition;
    ProbTable confidence[2];

    ObstacleMap createObstacleMap(std::vector<Position> const& obstacles);

    void initBelief(ObstacleMap const& obstacleMap);
    void initTransition(ObstacleMap const& obstacleMap, double adjProb);
    void initConfidence(ObstacleMap const& obstalceMap, double correctProb);

    int mapToIndex(int row, int col) const;
};
