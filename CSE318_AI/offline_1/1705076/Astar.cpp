#include <algorithm>
#include <queue>
#include <utility>
#include <vector>
#include <unordered_set>

#include "Astar.hpp"

namespace AI {
    Astar::Astar(Heuristics _heuristics)
        : heuristics(_heuristics) {
    } 

      
    std::tuple<std::string, int, int> Astar::solve(Board const& board) const {
        auto temp = search(board);
        auto explored = std::get<0>(temp);
        auto expandedCount = std::get<1>(temp);

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

        return make_tuple(moves, explored.size(), expandedCount);
    }


    bool operator<(Astar::Node const& node1, Astar::Node const& node2){
        return node1.g+node1.h < node2.g +node2.h;
    }


    bool operator>(Astar::Node const& node1, Astar::Node const& node2){
        return node2 < node1;
    }
    

    std::tuple<std::unordered_map<Board, char>, int> Astar::search(Board const& board) const{
        std::unordered_map<Board, char> exploredTable;
        std::unordered_map<Board, int> best;
        std::unordered_set<Board> expanded;

        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
        pq.push({board, 0, 0, 0});
        best[board] = 0;
        exploredTable[board] = 0;
        
        while(!pq.empty()){
            auto node = pq.top();
            pq.pop();
            expanded.insert(node.board);

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
                    exploredTable[newBoard] = move;
                    best[newBoard] = g;
                    int h = getH(newBoard);
                    pq.push({newBoard, node.g+1, h, move});
                }                
            }
        }

        return make_tuple(exploredTable, (int) expanded.size());
    }


    int Astar::getH(Board const& board) const{
        switch(heuristics){
            case Heuristics::None:
                return 0;
                break;
            case Heuristics::HammingDistance:
                return getHammingDistance(board);
                break;
            case Heuristics::ManhattanDistance:
                return getManhattanDistance(board);
                break;
            case Heuristics::LinearConflict:
                return getManhattanDistance(board) + 2 * getLinearConflict(board);
                break;
            default:
                std::cout<<"ERROR: unknown heuristics"<<std::endl;
                break;
        }
        return 0;
    }


    int Astar::getHammingDistance(Board const& board) const {
        int result = 0;
        for(int i = 0; i<board.grid.size(); i++){
            if(board.grid[i] == 0)
                continue;
            result += (board.grid[i] != i+1);
        }
        return result;
    }
        

    int Astar::getManhattanDistance(Board const& board) const {
        int result = 0;
        int k = 0;
        for(int i = 0; i<board.size; i++){
            for(int j = 0; j<board.size; j++){
                int x = board.grid[k];

                if(x != 0){
                    int ii = (x-1) / board.size;
                    int jj = (x-1) % board.size;
                    result += std::abs(i-ii) + std::abs(j-jj);
                }
                
                k++;
            }
        }
        return result;
    }


    int Astar::getLinearConflict(Board const& board) const {
        int result = 0;
        int l = 0;
        for(int i = 0; i<board.size; i++){
            int r = l+board.size;
            for(int k = l; k<r; k++){
                if(board.grid[k] < l+1 || r+1 <= board.grid[k])
                    continue;
                    
                for(int kk = l; kk<k; kk++){
                    if(board.grid[kk] > board.grid[k] && board.grid[kk] < r+1)
                        result += 1;
                }
            }
            l += board.size;
        }
        return result;
    }
}
