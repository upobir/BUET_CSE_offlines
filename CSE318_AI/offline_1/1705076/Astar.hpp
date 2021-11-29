#pragma once

#include <string>
#include <unordered_map>

#include "Board.hpp"

namespace AI {
    enum class Heuristics {
        None,
        HammingDistance,
        ManhattanDistance,
        LinearConflict,
    };


    class Astar {
    public:
        Astar(Heuristics _heuristics);

        std::pair<std::string, int> solve(Board const& board) const;
        
    private: 
        Heuristics heuristics;

        struct Node {
            Board board;
            int g;
            int h;
            char lastMove;
        };
        
        friend bool operator<(Node const& node1, Node const& node2); 
        
        friend bool operator>(Node const& node1, Node const& node2); 

        std::unordered_map<Board, char> search(Board const& board) const;

        int getH(Board const& board) const;
    };
}
