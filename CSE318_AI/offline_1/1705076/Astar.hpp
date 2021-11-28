#pragma once

#include <string>
#include <unordered_map>

#include "Board.hpp"

namespace AI {
    enum class Heuristics {
        HammingDistance,
        ManhattanDistance,
        LinearConflict,
    };


    class Astar {
    public:
        Astar(Heuristics _heuristics);

        std::pair<std::string, int> solve(Board board) const;
        
    private: 
        Heuristics heuristics;

        struct Node {
            Board board;
            int g;
            int h;
            char lastMove;
        };
        
        friend bool operator<(Node const& node1, Node const& node2); 
        
        std::unordered_map<Board, char> search(Board const& board) const;
    };
}
