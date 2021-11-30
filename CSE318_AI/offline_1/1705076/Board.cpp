#include <algorithm>
#include <numeric>
#include <iostream>
#include <utility>

#include "Board.hpp"

namespace AI {
    Board::Board(int _size, std::vector<int> values)
        : size(_size), grid(values) {
        grid.resize(size*size); 
    } 


    Board::Board(int _size)
        : size(_size), grid(_size*_size) {
        std::iota(grid.begin(), grid.end(), 1); 
        grid.back() = 0;
   }
    
    
    bool Board::isMoveValid(char move) const{
        auto position = getBlankPosition();
        switch(move){
            case 'L':
                return position.second < size-1;
                break;
            case 'R':
                return position.second > 0;
                break;
            case 'U':
                return position.first < size-1;
                break;
            case 'D':
                return position.first > 0;
                break;
            default:
                std::cerr<<"ERROR: illegal move"<<std::endl;
                return false;
                break;
        }
    }


    Board Board::applyMove(char move) const{
        Board newBoard(*this);
        int index = std::find(newBoard.grid.begin(), newBoard.grid.end(), 0) - newBoard.grid.begin();
        switch(move){
            case 'L':
                std::swap(newBoard.grid[index], newBoard.grid[index+1]);
                break;
            case 'R':
                std::swap(newBoard.grid[index], newBoard.grid[index-1]);
                break;
            case 'U':
                std::swap(newBoard.grid[index], newBoard.grid[index+size]);
                break;
            case 'D':
                std::swap(newBoard.grid[index], newBoard.grid[index-size]);
                break;
            default:
                std::cerr<<"ERROR: illegal move"<<std::endl;
                break;
        }

        return newBoard;
    }


    std::pair<int, int> Board::getBlankPosition() const{
        int k = 0;
        for(int i = 0; i<size; i++){
            for(int j = 0; j<size; j++){
                if(grid[k] == 0){
                    return {i, j};
                }
                k++;
            }
        }
        std::cerr<<"ERROR: cannot find gap"<<std::endl;
        return {-1, -1};
    }
    

    std::ostream& operator<<(std::ostream& os, Board const& board){
        int k = 0; 
        for(int i = 0; i<board.size; i++){
            std::cout<<"+--";
        }
        std::cout<<"+"<<std::endl;

        for(int i = 0; i<board.size; i++){
            std::cout<<"|";
            for(int j = 0; j<board.size; j++){
                if(board.grid[k] == 0){
                    os << "  ";
                }
                else {
                    os << std::setw(2) << std::setfill(' ')<< board.grid[k] ;
                }
                os << "|";                 
                k++;
            }
            os << std::endl;

            for(int i = 0; i<board.size; i++){
                std::cout<<"+--";
            }
            std::cout<<"+"<<std::endl;
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
