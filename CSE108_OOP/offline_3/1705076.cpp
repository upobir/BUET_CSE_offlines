#include <iostream>
using namespace std;

class Unit
{
protected:
    string name, weapon, movement;
    int range, health, damage, cost, position;

    virtual void show_status()
    {
        cout<<name<<". "<<"Health: "<<health<<". ";
    }

    virtual void move(int p)
    {
        cout<<movement<<". Positioned at "<<position<<". ";
    }

    void attack(Unit *o)
    {
        if(abs(position - o->position) <= range)
        {
            cout<<name<<" is shooting "<<weapon<<". ";
            o->health -= damage;
        }
    }

    virtual void kill()
    {
        cout<<"Unit Died!! "<<endl;
    }

    bool playround(Unit *o)     ///returns whether this unit could finish the round
    {
        show_status();
        if(isDead()) return false;
        move(o->position);
        attack(o);
        cout<<endl;
        return true;
    }

    virtual int retrieve(int &coins)
    {}

public:
    Unit(int r = 0, int h = 0, int d = 0, int c = 0, int p = 0)
    {
        range = r;
        health = h;
        damage = d;
        cost = c;
        position = p;
    }

    bool isDead()
    {
        return (health <= 0);
    }

    int buy(int &coins)
    {
        coins -= cost;
    }

    void fight(Unit *o, int &r, int &c)
    {
        while(1)
        {
            cout<<"Round : "<<r<<endl;
            if(!playround(o))
            {
                kill();
                break;
            }
            if(!o->playround(this))
            {
                o->kill();
                break;
            }
            r++;
        }
        cout<<endl;
        if(isDead()) retrieve(c);
    }

};

class Stationary: virtual public Unit
{
protected:
    int return_coin;

    virtual int retrieve(int &coins)
    {
        coins += return_coin;
    }

    virtual void kill()
    {
        cout<<"Unit Died!! "<<endl;
        cout<<name<<" gave "<<return_coin<<" coins while dying. "<<endl;
    }

public:
    Stationary(int r = 0)
    {
        return_coin = r;
        movement = "Stationary";
    }
};

class Moving: virtual public Unit
{
protected:
    int step;

    virtual void move(int p)
    {
        if(position + step <= p) position += step;
        cout<<movement<<". Positioned at "<<position<<". ";
    }
public:
    Moving(int s = 0)
    {
        step = s;
    }

};

class Enemy_tower: virtual public Unit
{
protected:
    virtual void move(int p)
    {}

    virtual void show_status()
    {
        cout<<name<<"'s "<<"Health: "<<health<<". ";
    }

    virtual void kill()
    {
        cout<<"You Destroyed the Enemy Tower!! "<<endl;
    }

public:
    Enemy_tower(int p = 100):Unit(200, 300, 40, 1000, p)
    {
        name = "Enemy Tower";
        weapon = "Fire Arrow";
    }
};

class Bowman: virtual public Unit, public Stationary
{
protected:
public:
    Bowman(int p = 0):Unit(110, 60, 10, 100, p), Stationary(40)
    {
        name = "Bowman";
        weapon = "Basic Arrow";
    }
};

class AdvancedBowman: virtual public Unit, public Stationary
{
protected:
public:
    AdvancedBowman(int p = 0):Unit(130, 85, 15, 200, p), Stationary(60)
    {
        name = "Advanced Bowman";
        weapon = "Canon";
    }
};


class Archer: virtual public Unit, virtual public Moving
{
protected:
public:
    Archer(int p = 0):Unit(50, 100, 25, 150, p), Moving(25)
    {
        name = "Archer";
        movement = "Running";
        weapon = "Advanced Arrow";
    }
};

class AdvancedArcher: virtual public Unit, public Moving
{
protected:
public:
    AdvancedArcher(int p = 0):Unit(50, 120, 50, 600, p), Moving(30)
    {
        name = "Advanced Archer";
        movement = "Riding Horse";
        weapon = "Improved Arrow";
    }
};


int main()
{
    int coins = 1600, round = 1;
    Unit *w = new Unit();
    Unit *e = new Enemy_tower(100);
    bool win;
    while(1)
    {
        cout<<"Coin Remaining: "<<coins<<endl;
        cout<<"Choose your option:"<<endl;
        cout<<"1. Archer (Cost 150)"<<endl;
        cout<<"2. Advanced Archer (Cost 600)"<<endl;
        cout<<"3. Bowman (Cost 100)"<<endl;
        cout<<"4. Advanced Bowman (Cost 200)"<<endl;
        int choice;
        cin>>choice;

        switch(choice)
        {
        case 1:w =  new Archer(0);
                break;
        case 2:w = new AdvancedArcher(0);
                break;
        case 3: w =  new Bowman(0);
                break;
        default : w = new AdvancedBowman(0);
                break;
        }
        w->buy(coins);
        if(coins <= 0)
        {
            cout<<"you have used up your coins!"<<endl;
            cout<<"YOU LOSE :("<<endl;
            break;
        }
        w->fight(e, round, coins);
        if(e->isDead())
        {
            cout<<"The enemy has lost!"<<endl;
            cout<<"YOU WIN  :)"<<endl;
            break;
        }

        ///You have to complete all the functionalities of the user using the base class pointer w.
        ///That means you have to call the derived class functions using only this.
    }
}
