#include <algorithm>
#include <numeric>
#include <iostream>

#include "Board.hpp"

namespace AI {
    Board::Board(int _size, std::vector<int> values)
        : size(_size), grid(values) {
        grid.resize(size*size); 
    } 


    Board::Board(int _size)
        : size(_size), grid(size*size) {
        std::iota(grid.begin(), grid.end(), 1); 
    }
    

    Board applyMove(char move) const{
        Board newBoard(*this);
        int index = std::find(newBoard.grid.begin(), newBoard.grid.end(), 0);
        switch(move){
            case 'L':
                break;
            case 'R':
                break;
            case 'U':
                break;
            case 'D':
                break;
            default:
                std::cerr<<"ERROR"<<std::endl;
                break;
        }

        return newBoard;
    }
    

    std::ostream& operator<<(std::ostream& os, Board const& board){
        int k = 0; 
        for(int i = 0; i<board.size; i++){
            for(int j = 0; j<board.size; j++){
                if(board.grid[k] == 0){
                    os << "  ";
                }
                else {
                    os << std::setw(2) << std::setfill(' ')<< board.grid[k] ;
                }
                os << " ";                 
                k++;
            }
            os << std::endl;
        }
        return os;
    }


    bool operator==(Board const& board1, Board const& board2){
        return board1.grid == board2.grid;
    }
    

    bool operator!=(Board const& board1, Board const& board2){
        return !(board1 == board2);
    }


    std::size_t Board::getHash() const {
        std::size_t h = 0;
        for(int x : grid){
            h = ((h<<5)+h) + x;
        }
        return h;
    }


    bool Board::isSolvable() const {
        if(size % 2 == 0){
            return isSolvableEven();
        }
        else{
            return isSolvableOdd();
        }
    }
    

    bool Board::isSolvableEven() const {
        auto it = std::find(grid.begin(), grid.end(), 0);
        int zeroRow = (it - grid.begin())/ size;

        return zeroRow % 2 != getInversionCount() % 2;
    }
    

    bool Board::isSolved() const {
        for(int i = 0; i+1<grid.size(); i++){
            if(i+1 != grid[i])
                return false;
        }
        return grid.back() == 0;
    }
    

    bool Board::isSolvableOdd() const {
        return getInversionCount() % 2 == 0;
    }


    int Board::getInversionCount() const {
        int result = 0;

        for(int i = 0; i<grid.size(); i++){
            if(grid[i] == 0) continue;

            for(int j = 0; j<i; j++){
                if(grid[j] == 0) continue;

                if(grid[j] > grid[i])
                    result++;
            }
        }
        
        return result;            
    }
}
