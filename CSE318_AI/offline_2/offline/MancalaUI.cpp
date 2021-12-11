#include "MancalaUI.hpp"

#include <cassert>
#include <iostream>
#include <iomanip>

namespace AI {
    MancalaUI::MancalaUI(MancalaAgent& _firstPlayer, MancalaAgent& _secondPlayer) 
        : firstPlayer(_firstPlayer), secondPlayer(_secondPlayer), mancala() {
    }

    void MancalaUI::run() {
        std::cout<<"Starting game"<<std::endl;
        std::cout<<"First player is "<<firstPlayer.getName()<<std::endl;
        std::cout<<"Second player is "<<secondPlayer.getName()<<std::endl;


        printBoard();

        while(!mancala.isEnded()){
            std::cout<<std::endl;
            std::cout<<"Turn of "<<(mancala.getCurrentPlayer() == 1? "first" : "second")<<" player: "<<std::endl;

            int move = (mancala.getCurrentPlayer() == 1? firstPlayer.getMove(mancala) : secondPlayer.getMove(mancala));
            auto result = mancala.applyMove(move);

            switch(std::get<0>(result)){
                case Mancala::Result::NORMAL:
                    std::cout<<"Last stone placed at row "<<std::get<1>(result)<<" column "<<std::get<2>(result)<<std::endl;
                    break;

                case Mancala::Result::BONUS:
                    std::cout<<"Last stone placed at bowl, bonus move awarded"<<std::endl;
                    break;

                case Mancala::Result::STEAL:
                    std::cout<<"Last stone placed at row "<<std::get<1>(result)<<" column "<<std::get<2>(result)<< " stealing from that column"<<std::endl;
                    break;
            }

            std::cout<<std::endl;

            printBoard();
        }

        std::cout<<std::endl;
        int winner = mancala.getWinner();
        if(winner == 0){
            std::cout<<"Game ended in draw"<<std::endl;
        }
        else if(winner == 1){
            std::cout<<"Game ended, first player won"<<std::endl;
        }
        else{
            std::cout<<"Game ended, second player won"<<std::endl;
        }
        mancala.setWinConfig();
        printBoard();
        std::cout<<std::endl;
    }

    void MancalaUI:: printBoard() {

        auto print = [&](int x){
            if(x == 0)
                std::cout<<"   ";
            else
                std::cout<<std::setw(3)<<std::setfill(' ')<<x;
        };

        /// top wall
        for(int i = 0; i<8; i++){
            std::cout<<"+---";
        }
        std::cout<<"+"<<std::endl;

        // top row
        std::cout<<"|   ";
        for(int i = 0; i<6; i++){
            std::cout<<"|";
            print(mancala.getPocketStoneCount(1, i+1));
        }
        std::cout<<"|   |"<<std::endl;

        /// middle wall
        std::cout<<"|";
        print(mancala.getBowlStoneCount(2));
        for(int i = 0; i<6; i++){
            std::cout<<"+---";
        }
        std::cout<<"+";
        print(mancala.getBowlStoneCount(1));
        std::cout<<"|"<<std::endl;

        /// bottom row
        std::cout<<"|   ";
        for(int i = 0; i<6; i++){
            std::cout<<"|";
            print(mancala.getPocketStoneCount(2, i+1));
        }
        std::cout<<"|   |"<<std::endl;

        /// bottom wall
        for(int i = 0; i<8; i++){
            std::cout<<"+---";
        }
        std::cout<<"+"<<std::endl;
    }
}