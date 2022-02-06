#include <iostream>
#include <iomanip>

#include "MarkovModel.hpp"


void prettyPrint(MarkovModel const& model);


int main(){
    int rowCount, colCount;
    std::cin >> rowCount >> colCount;

    int obstacleCount;
    std::cin >> obstacleCount;

    std::vector<std::pair<int, int>> obstacles(obstacleCount);
    for(int i = 0; i<obstacleCount; i++){
        std::cin >> obstacles[i].first >> obstacles[i].second;
    }

    const double adjProb = 0.9;
    const double correctProb = 0.85;
    MarkovModel model({rowCount, colCount}, obstacles, adjProb, correctProb);

    std::cout << "Initial Probability:" << std::endl;
    prettyPrint(model);

    bool isRunning = true;
    int updateCount = 0;
    while(isRunning){
        char command;
        std::cin >> command;

        switch(command){
            case 'R': {
                int row, col, result;
                std::cin >> row >> col >> result;

                model.updateBelief(row, col, result);
                updateCount++;

                std::cout << "Probality Update (Reading #" << updateCount <<"):" << std::endl;
                prettyPrint(model);

            } break;

            case 'C': {
                auto pos = model.getEstimatedLocation();
                std::cout << "Casper is most probably at (" << pos.first << "," << pos.second << ")" << std::endl;
                std::cout << std::endl;
            } break;

            case 'Q': {
                isRunning = false;
                std::cout << "Bye Casper!" << std::endl;
            } break;

            default: {
                std::cout << "Invalid command !!!" << std::endl;
                std::cout << std::endl;
            } break;
        }
    }

    return 0;
}


void prettyPrint(MarkovModel const& model){
    std::cout << std::fixed << std::setprecision(4);

    for(int row = 0; row<model.getRowCount(); row++){
        for(int col = 0; col<model.getColCount(); col++){
            std::cout << model.getProbability(row, col) << " ";
         }
        std::cout << std:: endl;
    }
    std::cout << std::endl;
    return;
}
