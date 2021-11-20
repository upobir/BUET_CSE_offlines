#include <iostream>
#include <cassert>
#include<random>
#include<chrono>
#include<windows.h>
using namespace std;

mt19937 rng(time(0));

///get a windows performance time stamp
LARGE_INTEGER GetTime()
{
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t;
}

///get time difference of two stamps
long double TimeDiff(LARGE_INTEGER t1, LARGE_INTEGER t2)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return ((long double)(t2.QuadPart-t1.QuadPart)/(1e-6*(long double)frequency.QuadPart));
}

class queue{
    int cap, L, R, sz;
    int *memory;

    void increaseCap(int nCap){
        int *tmp = new int[nCap];
        for(int i = 0; i<sz; i++){
            tmp[i] = memory[(L+i)%cap];
        }
        swap(tmp, memory);
        delete[] tmp;
        cap = nCap;
        L = 0, R = sz;
    }

public:
    queue(){
        cap = 2;
        memory = new int[cap];
        L = R = 0;
        sz = 0;
    }

    bool empty(){
        return (sz == 0);
    }

    int enqueue(int item){
        if(sz == cap) increaseCap(2*sz);
        memory[R++] = item;
        R %= cap;
        sz++;
    }

    int dequeue(){
        assert(sz > 0);
        int x = memory[L++];
        L %= cap;
        sz--;
        return x;
    }

    ~queue(){
        delete [] memory;
    }
};

class ArrayList{
    int cap, sz;
    int *memory;

    void increaseCap(int nCap){
        int *tmp = new int[nCap];
        for(int i = 0; i<cap; i++) tmp[i] = memory[i];
        swap(tmp, memory);
        delete [] tmp;
        cap = nCap;
    }

public:
    ArrayList(){
        sz = 0;
        cap = 2;
        memory = new int[cap];
    }

    int searchItem(int item){
        for(int i = 0; i<sz; i++)
            if(memory[i] == item) return i;
        return -1;
    }

    void insertItem(int item){
        if(sz == cap) increaseCap(2*cap);
        memory[sz++] = item;
    }

    void removeItem(int item){
        int shift = 0;
        for(int i = 0; i<sz; i++)
            if(memory[i] == item) shift++;
            else memory[i-shift] = memory[i];
        sz -= shift;
    }

    void removeItemAt(int position){
        if(position >= sz) assert(0);
        for(int i = position+1; i<sz; i++)
            memory[i-1] = memory[i];
        sz--;
    }

    int getItem(int position){
        if(position >= sz) assert(0);
        return memory[position];
    }

    int getLength(){
        return sz;
    }

    bool empty(){
        return (sz == 0);
    }

    void printList(){
        for(int i = 0; i<sz; i++)
            cout<<memory[i]<<" ";
        cout<<endl;
    }

    ~ArrayList(){
        delete [] memory;
    }
};

enum{WHITE, GRAY, BLACK};
class Graph{
    int nV, nE;
    ArrayList * lists;
    bool dir;
    int *bfsPar, *bfsDist, *bfsColor;
    int *dfsPar, *dfsColor, *dfsStart, *dfsEnd;
    int clk;

    void clearGraph(){
        delete [] lists;
        delete [] bfsPar;
        delete [] bfsColor;
        delete [] bfsDist;
        delete [] dfsPar;
        delete [] dfsColor;
        delete [] dfsStart;
        delete [] dfsEnd;
        lists = nullptr;
        bfsColor = bfsDist = bfsPar = nullptr;
        dfsColor = dfsStart = dfsEnd = dfsPar = nullptr;
        nV = nE = 0;
    }
    bool validVertex(int u){
        return (0 <= u && u < nV);
    }
    void dfsVisit(int u){
        cout<<"DFS visiting "<<u<<endl;
        dfsColor[u] = GRAY;
        clk++;
        dfsStart[u] = clk;
        for(int i = 0; i<lists[u].getLength(); i++){
            int v = lists[u].getItem(i);
            if(dfsColor[v] == WHITE)
                dfsVisit(v);
        }
        dfsEnd[u] = clk;
        dfsColor[u] = BLACK;
        return;
    }

public:
    Graph(bool directed = false){
        dir = directed;
        nV = 0, nE = 0;

        lists = nullptr;
        bfsColor = bfsDist = bfsPar = nullptr;
        dfsColor = dfsStart = dfsEnd = dfsPar = nullptr;
    }
    void setnVertices(int n){
        if(lists) clearGraph();
        nV = n;
        lists = new ArrayList[nV];
        bfsColor = new int[nV];
        bfsPar = new int[nV];
        bfsDist = new int[nV];
        dfsStart = new int[nV];
        dfsEnd = new int[nV];
        dfsColor = new int[nV];
        dfsPar = new int[nV];
    }

    bool isEdge(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return false;
        return (lists[u].searchItem(v) != -1);
    }

    bool addEdge(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return false;
        if(isEdge(u, v)) return false;
        lists[u].insertItem(v);
        if(!dir) lists[v].insertItem(u);
        return true;
    }

    void printGraph(){
        cout<<endl;
        for(int i = 0; i<nV; i++){
            cout<<i<<" : ";
            lists[i].printList();
        }
    }

    void removeEdge(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return;
        lists[u].removeItem(v);
        if(!dir) lists[v].removeItem(u);
    }

    int getOutDegree(int u){
        if(!validVertex(u)) return -1;
        return lists[u].getLength();
    }

    int getInDegree(int u){
        if(!validVertex(u)) return -1;
        int degree = 0;
        for(int i = 0; i<nV; i++)
            if(isEdge(i, u)) degree++;
        return degree;
    }

    bool hasCommonAdjacent(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return false;
        for(int i = 0; i<nV; i++){
            if(isEdge(i, u) && isEdge(i, v)) return true;
            if(isEdge(u, i) && isEdge(v, i)) return true;
        }
        return false;
    }

    void bfs(int source){
        for(int i = 0; i <nV; i++){
            bfsColor[i] = WHITE;
            bfsDist[i] = 1000000000;
            bfsPar[i] = -1;
        }
        bfsColor[source] = GRAY;
        bfsDist[source] = 0;
        queue Q;
        Q.enqueue(source);
        while(!Q.empty()){
            int u = Q.dequeue();
//            cout<<"BFS visiting "<<u<<endl;
            for(int i = 0; i<lists[u].getLength(); i++){
                int v = lists[u].getItem(i);
                if(bfsColor[v] == WHITE){
                    bfsColor[v] = GRAY;
                    bfsDist[v] = bfsDist[u]+1;
                    bfsPar[v] = u;
                    Q.enqueue(v);
                }
            }
            bfsColor[u] = BLACK;
        }
    }

    void dfs(int source){
        for(int i = 0; i<nV; i++){
            dfsColor[i] = WHITE;
            dfsPar[i] = -1;
        }
        clk = 0;
        dfsVisit(source);
    }



    int getDist(int u, int v){
        bfs(u);
        if(bfsDist[v] == 1000000000) return -1;
        else return bfsDist[v];
    }

    ~Graph(){
        if(lists) clearGraph();
    }

};

int main(){
    int V, E;
    V = 16000;
//    E = 1000*V;
    E = (V*V-V)/2;
    assert(V*V-V >= 2*E);
    Graph G;
    G.setnVertices(V);
    for(int i = 0; i<E; i++){
        int u, v;
        do{
            u = rng() % V;
            v = rng() % V;
        }while(!G.addEdge(u, v));
    }
//    for(int i = 0; i<V; i++){
//        for(int j = 0; j<i; j++){
//            G.addEdge(i, j);
//        }
//    }
    cout<<"Generated"<<endl;

    double tottime = 0;
    for(int i = 0; i<10; i++){
        int u = rng() % V;
        LARGE_INTEGER t1 = GetTime();
        G.bfs(u);
        LARGE_INTEGER t2 = GetTime();
        tottime += TimeDiff(t1, t2);
    }
    tottime /= 10.0;
    cout<<"Avarage time: "<<tottime;
    return 0;
}
