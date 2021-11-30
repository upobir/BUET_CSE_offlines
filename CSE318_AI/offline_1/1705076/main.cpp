#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "Board.hpp"
#include "Astar.hpp"

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

    //TODO temporary
    //AI::Astar temp(AI::Heuristics::None);
    //std::cout<<"hamming: "<<temp.getHammingDistance(board)<<std::endl;
    //std::cout<<"manhattan: "<<temp.getManhattanDistance(board)<<std::endl; 
    //std::cout<<"conflicts: "<<temp.getLinearConflict(board)<<std::endl;

    if(board.isSolvable()){
        std::cout << "Board is solvable"<<std::endl;

        AI::Heuristics heuristics[] = {
            AI::Heuristics::None,
            AI::Heuristics::HammingDistance,
            AI::Heuristics::ManhattanDistance,
            AI::Heuristics::LinearConflict,
        };

        std::string names[] = {
            "no",
            "hamming distance",
            "manhattan distance",
            "linear conflict",
        };
 
        for(int i = 0; i<4; i++){
            std::string temp;
            std::cout<<"Run A* with "<<names[i]<<" heuristic (y/n)?: ";
            std::cin>>temp;
            if(temp[0] != 'y')
                continue;

            auto t1 = std::chrono::high_resolution_clock::now();

            AI::Astar astar(heuristics[i]);
            auto solution = astar.solve(board);
            auto moves = std::get<0>(solution);
            auto explored = std::get<1>(solution);
            auto expanded = std::get<2>(solution);

            std::cout<<"Explored node count: "<<explored<<std::endl;
            std::cout<<"Expanded node count: "<<expanded<<std::endl;
            std::cout<<std::endl;

            std::cout<<"Move count: "<<moves.size()<<std::endl;
            std::cout<<"Moves: "<<moves<<std::endl;
            std::cout<<std::endl;

            auto t2 = std::chrono::high_resolution_clock::now();

            std::cout<<"Time taken: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<" milliseconds"<<std::endl;
            
            std::cout<<"show solution simulation (y/n)?: ";
            std::cin>>temp;
            if(temp[0] != 'y')
                continue;

            AI::Board current = board;

            std::cout<<current;
            for(auto move: moves){
                std::cout<<std::endl;
                std::cout<<"-> "<<move<<std::endl;
                std::cout<<std::endl;

                current = current.applyMove(move);
                std::cout<<current;
            }
       }
    }
    else{
        std::cout<< "Board is not solvable"<<std::endl;
    }

    return 0;
}
