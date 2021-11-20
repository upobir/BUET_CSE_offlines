#include <iostream>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <fstream>
using namespace std;

#define GRIDSIZE 4
#define UP      0
#define DOWN    2
#define LEFT    3
#define RIGHT   1
#define MAX_SHOTS 3


string to_String(int x) {           ///changing name as collides with builtin to_string()
    std::string out_string;
    std::stringstream ss;
    ss << x;
    return ss.str();
}


class Position {

    int x, y;

public:

    Position (int x, int y) {
        this->x = x;
        this->y = y;
    }

    Position() {}

    // Modify the following four so that the resulting position does not leave the grid
    void moveRight() {
        if(x < GRIDSIZE-1) x++;
    }

    void moveLeft() {
        if(x > 0) x--;
    }

    void moveUp() {
        if (y < GRIDSIZE-1) y++;
    }

    void moveDown() {
        if(y > 0) y--;
    }

    bool isAdjacent(Position p) {
        return (abs(x - p.getX()) + abs(y-p.getY()) == 1);
    }

    bool isSamePoint(Position p) {
        return (x == p.getX() && y == p.getY());
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }
};


class Wumpus {

    bool killed;
    Position p;

public:

    Wumpus(int x, int y) {
        p = Position(x, y);
        killed = false;
    }

    Wumpus() {
        p = Position(rand()%4, rand()%4);
        killed = false;
    }

    void kill() {
        killed = true;
    }

    Position getPosition() {
        return p;
    }

    bool isDead(){              ///returns state of wumpus
        return killed;
    }
};


class Player {

    int direction;
    int total_shots;
    bool killed;
    Position p;

public:

    Player() {
        direction = UP;
        total_shots = MAX_SHOTS;
        killed = false;
        p = Position(0, 0);
    }

    void turnLeft() {
        direction = (direction+3) % 4;      ///the macros are such that turning left is just subtracting 1, equivalent to adding 3
    }

    void turnRight() {
        direction = (direction+1) % 4;      ///same as above, just adding 1
    }

    void moveForward() {
        switch(direction)
        {
        case UP:
            p.moveUp();
            break;
        case DOWN:
            p.moveDown();
            break;
        case LEFT:
            p.moveLeft();
            break;
        case RIGHT:
            p.moveRight();
            break;
        }
    }

    bool isAdjacent(Position pos) {
        return p.isAdjacent(pos);
    }

    bool isSamePoint(Position pos) {
        return p.isSamePoint(pos);
    }

    void kill() {
        killed = true;
    }

    string getPositionInfo() {
        return "Player is now at " + to_String(p.getX()) + ", " + to_String(p.getY());
    }

    string getDirectionInfo() {
        string s;
        if (direction == UP) s = "up";
        if (direction == DOWN) s = "down";
        if (direction == LEFT) s = "left";
        if (direction == RIGHT) s = "right";
        return "Player is moving at direction: " + s;
    }

    Position getPosition(){
        return p;
    }

    int getShots(){                     ///get arrow left
        return total_shots;
    }

    int useArrow(){
        if(total_shots > 0) total_shots--;
    }

    int getDirection(){                  ///get direction
        return direction;
    }
};



class WumpusWorld {

private:

    Player player;
    Wumpus wumpus;
    Position gold_position;
    Position pit_position;
    bool ended;

public:

    WumpusWorld() {
        wumpus = Wumpus();
        gold_position = Position(rand()%4, rand()%4);
        pit_position = Position(rand()%4, rand()%4);
    }

    WumpusWorld(int wumpus_x, int wumpus_y) {
        wumpus = Wumpus(wumpus_x, wumpus_y);
        gold_position = Position(rand()%4, rand()%4);
        pit_position = Position(rand()%4, rand()%4);
    }

    WumpusWorld(int wumpus_x, int wumpus_y, int gold_x, int gold_y) {
        wumpus = Wumpus(wumpus_x, wumpus_y);
        gold_position = Position(gold_x, gold_y);
        pit_position = Position(rand()%4, rand()%4);
    }

    WumpusWorld(int wumpus_x, int wumpus_y, int gold_x, int gold_y, int pit_x, int pit_y) {
        wumpus = Wumpus(wumpus_x, wumpus_y);
        gold_position = Position(gold_x, gold_y);
        pit_position = Position(pit_x, pit_y);
    }

    void moveForward() {
        player.moveForward();
        return showGameState();
    }

    void turnLeft() {
        player.turnLeft();
        return showGameState();
    }

    void turnRight() {
        player.turnRight();
        return showGameState();
    }

    void shoot() {
        if(player.getShots() == 0 || wumpus.isDead())
        {
            cout<<"Nothing happened"<<endl;
            showGameState();
            return;
        }
        else player.useArrow();

        Position ppos = player.getPosition();
        Position wpos = wumpus.getPosition();

        switch(player.getDirection())
        {
        case UP:
            if(ppos.getX() == wpos.getX() && ppos.getY() < wpos.getY()) wumpus.kill();
            break;
        case DOWN:
            if(ppos.getX() == wpos.getX() && ppos.getY() > wpos.getY()) wumpus.kill();
            break;
        case RIGHT:
            if(ppos.getY() == wpos.getY() && ppos.getX() < wpos.getX()) wumpus.kill();
            break;
        case LEFT:
            if(ppos.getY() == wpos.getY() && ppos.getX() > wpos.getX()) wumpus.kill();
            break;
        }

        if(wumpus.isDead()) cout<<"The wumpus screamed in pain and died"<<endl;
        else cout<<"you missed, arrows remaining: "<<player.getShots()<<endl;
        showGameState();
    }

    void showGameState() {
        cout << player.getPositionInfo() << endl;
        cout << player.getDirectionInfo() << endl;

        if (!wumpus.isDead() && player.isAdjacent(wumpus.getPosition())) {
            cout << "stench!" << endl;
        }

        if (player.isAdjacent(pit_position)) {
            cout << "breeze!" << endl;
        }

        if (!wumpus.isDead() && player.isSamePoint(wumpus.getPosition())) {
            cout << "Wumpus killed you!" << endl;
            player.kill();
            cout << "Game over!" << endl;
            ended = true;
        }

        if (!ended && player.isSamePoint(pit_position)) {
            cout << "You fell in the pit!" << endl;
            player.kill();
            cout << "Game over!" << endl;
            ended = true;
        }

        if (!ended && player.isSamePoint(gold_position)) {
            cout << "Got the gold!" << endl;
            cout << "Game ended, you won!" << endl;
            ended = true;
        }
    }

    bool isOver() {
        return ended;
    }

};


int main()
{
    int c, wumpus_x, wumpus_y, gold_x, gold_y, pit_x, pit_y;

    ifstream file;
    file.open("wumpus.txt");
    file >> wumpus_x >> wumpus_y;
    file >> gold_x >> gold_y;
    file >> pit_x >> pit_y;
    file.close();

    WumpusWorld w(wumpus_x, wumpus_y, gold_x, gold_y, pit_x, pit_y);

    w.showGameState();
    while (!w.isOver()) {
        cout << "1: move forward" << endl;
        cout << "2: Turn left" << endl;
        cout << "3: Turn right" << endl;
        cout << "4: Shoot" << endl;
        cin >> c;
        if (c == 1) {
            w.moveForward();
        } else if (c == 2) {
            w.turnLeft();
        } else if (c == 3) {
            w.turnRight();
        } else if (c == 4) {
            w.shoot();
        }
    }
    return 0;
}
