#include <algorithm>
#include <queue>
#include <utility>
#include <vector>

#include "Astar.hpp"

namespace AI {
    Astar::Astar(Heuristics _heuristics)
        : heuristics(_heuristics) {
    } 

      
    std::pair<std::string, int> Astar::solve(Board const& board) const {
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
                    std::cerr<<"ERROR: recosntruction failed"<<std::endl;
                    break;
            }
        }
        std::reverse(moves.begin(), moves.end());

        return make_pair(moves, explored.size());
    }


    bool operator<(Astar::Node const& node1, Astar::Node const& node2){
        return node1.g+node1.h < node2.g +node2.h;
    }


    bool operator>(Astar::Node const& node1, Astar::Node const& node2){
        return node2 < node1;
    }
    

    std::unordered_map<Board, char> Astar::search(Board const& board) const {
        std::unordered_map<Board, char> table;
        std::unordered_map<Board, int> best;
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
        pq.push({board, 0, 0, 0});
        best[board] = 0;
        table[board] = 0;
        
        while(!pq.empty()){
            auto node = pq.top();
            pq.pop();

            if(node.board.isSolved())
                break;
                
            if(node.g > best[node.board])
                continue;
                        
            for(char move: {'L', 'R', 'U', 'D'}){
                if(!node.board.isMoveValid(move)){
                    continue;
                }
                auto newBoard = node.board.applyMove(move);
                int g = node.g+1;
                if(best.count(newBoard) == 0 || g < best[newBoard]){
                    table[newBoard] = move;
                    best[newBoard] = g;
                    int h = getH(newBoard);
                    pq.push({newBoard, node.g+1, h, move});
                }                
            }
        }

        return table;
    }


    int Astar::getH(Board const& board) const{
        switch(heuristics){
            case Heuristics::None:
                return 0;
                break;
            default:
                std::cout<<"ERROR: unknown heuristics"<<std::endl;
                break;
        }
        return 0;
    }
}
