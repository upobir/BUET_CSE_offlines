#include <iostream>
#include <vector>
#include <string>

#include "Board.hpp"

int main(){
    
    std::cout<<"Enter grid size: ";
    int k;
    std::cin>>k;

    int n = k*k;
    std::vector<int> grid(n);

    for(int i = 0; i<n; i++){
        std::string cellValue; 
        std::cin>>cellValue;

        int value;
        if(cellValue == "*"){
            value = 0;
        }
        else {
            value = std::stoi(cellValue);
        }
        grid[i] = value;
    }

    AI::Board board(k, grid);

    std::cout<< "Board: "<<std::endl;
    std::cout<< board<< std::endl;

    if(board.isSolvable()){
        std::cout << "Board is solvable"<<std::endl;
    }
    else{
        std::cout<< "Board is not solvable"<<std::endl;
    }

    return 0;
}
