#pragma once
#include <vector>
#include <iomanip>
#include <iostream>

namespace AI {
    class Board {
    public:
        Board(int size, std::vector<int> values);

        bool isSolvable() const;

        friend std::ostream& operator<<(std::ostream& os, Board const& board);
        
    private:
        std::vector<int> grid ;
        int size;

        bool isSolvableEven() const;

        bool isSolvableOdd() const;

        int getInversionCount() const;
    };
}
