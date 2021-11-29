#pragma once

#include <vector>
#include <iomanip>
#include <iostream>
#include <utility>

namespace AI {
    class Astar;


    class Board {
    public:
        Board(int size, std::vector<int> values);

        Board(int size);

        bool isSolvable() const;

        bool isSolved() const;
        
        Board applyMove(char move) const;

        bool isMoveValid(char move) const;

        friend std::ostream& operator<<(std::ostream& os, Board const& board);

        std::size_t getHash() const;

        friend class Astar;
        
        friend bool operator==(Board const& board1, Board const& board2);

        friend bool operator!=(Board const& board1, Board const& board2);
        
    private:
        std::vector<int> grid ;
        int size;

        bool isSolvableEven() const;

        bool isSolvableOdd() const;

        int getInversionCount() const;

        std::pair<int, int> getBlankPosition() const;
    };
}


namespace std {
    template<>
    struct hash<AI::Board> {
        std::size_t operator()(AI::Board const& board) const {
            return board.getHash();
        }
    };
}
