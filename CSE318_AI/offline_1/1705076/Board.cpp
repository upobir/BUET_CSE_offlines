#include <algorithm>

#include "Board.hpp"

namespace AI {
    Board::Board(int _size, std::vector<int> values)
        : size(_size), grid(values) {
        grid.resize(size*size); 
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
