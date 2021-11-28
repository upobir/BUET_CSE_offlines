#include <algorithm>
#include <queue>

#include "Astar.hpp"

namespace AI {
    Astar::Astar(Heuristics _heuristics)
        : heuristics(_heuristics) {
    } 

      
    std::pair<std::string, int> Astar::solve(Board board) const {
        auto explored = search(board);

        std::string moves;
        Board target(board.size);

        while(target != board){
            moves.push_back(explored[target]);

            switch(moves.back()){
                case 'L':
                    target = target.applyMove('R');
                    break;
                case 'R':
                    target = target.applyMove('L');
                    break;
                case 'U':
                    target = target.applyMove('D');
                    break;
                case 'D':
                    target = target.applyMove('U');
                    break;
                default:
                    std::cerr<<"ERROR"<<std::endl;
                    break;
            }
        }
        std::reverse(moves.begin(), moves.end());

        return make_pair(moves, explored.size());
    }


    bool operator<(Astar::Node const& node1, Astar::Node const& node2){
        return node1.g+node1.h < node2.g +node2.h;
    }



    std::unordered_map<Board, char> Astar::search(Board const& board) const {
        std::unordered_map<Board, char> table;
        std::priority_queue<Node> pq;
        pq.push({board, 0, 0, 0});
        
        while(!pq.empty()){
            auto node = pq.top();
            pq.pop();

            if(table.count(node.board) > 0)
                continue;

            table[node.board] = node.lastMove;

            if(node.board.isSolved())
                break;
            
            for(char move: {'L', 'R', 'U', 'D'}){
                auto newBoard = node.board.applyMove(move);
                if(table.count(newBoard) > 0)
                    continue;

                pq.push({newBoard, node.g+1, 0, move});
            }
        }

        return table;
    }
}
