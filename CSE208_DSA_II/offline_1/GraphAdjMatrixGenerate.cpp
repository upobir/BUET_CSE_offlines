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

    void enqueue(int item){
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

enum{WHITE, GRAY, BLACK};
class Graph{
    int nV, nE;
    int **matrix;
    bool dir;
    int clk;
    int *bfsPar, *bfsDist, *bfsColor;
    int *dfsPar, *dfsColor, *dfsStart, *dfsEnd;

    void clearGraph(){
        for(int i = 0; i<nV; i++)
            delete [] matrix[i];
        delete [] matrix;
        delete [] bfsPar;
        delete [] bfsColor;
        delete [] bfsDist;
        delete [] dfsPar;
        delete [] dfsColor;
        delete [] dfsStart;
        delete [] dfsEnd;
        matrix = nullptr;
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
        for(int v = 0; v < nV; v++){
            if(matrix[u][v] != 1) continue;
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
        matrix = nullptr;
        bfsColor = bfsDist = bfsPar = nullptr;
        dfsColor = dfsStart = dfsEnd = dfsPar = nullptr;
    }
    void setnVertices(int n){
        if(matrix) clearGraph();
        nV = n;
        matrix = new int*[nV];
        for(int i = 0; i<n; i++){
            matrix[i] = new int[nV];
            for(int j = 0; j<n; j++)
                matrix[i][j] = 0;
        }
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
        return (matrix[u][v] == 1);
    }

    bool addEdge(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return false;
        if(isEdge(u, v)) return false;
        matrix[u][v] = 1;
        if(!dir) matrix[v][u] = 1;
        return true;
    }

    void printGraph(){
        cout<<endl;
        for(int i = 0; i<nV; i++){
            cout<<i<<" : ";
            for(int j = 0; j<nV; j++)
                if(matrix[i][j] == 1) cout<<j<<" ";
            cout<<endl;
        }
    }

    void removeEdge(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return;
        matrix[u][v] = 0;
        if(!dir) matrix[v][u] = 0;
    }

    int getOutDegree(int u){
        if(!validVertex(u)) return -1;
        int degree = 0;
        for(int i = 0; i<nV; i++)
            if(matrix[u][i] == 1) degree++;
        return degree;
    }

    int getInDegree(int u){
        if(!validVertex(u)) return -1;
        int degree = 0;
        for(int i = 0; i<nV; i++)
            if(matrix[i][u] == 1) degree++;
        return degree;
    }

    bool hasCommonAdjacent(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return false;
        for(int i = 0; i<nV; i++){
            if(matrix[u][i] == matrix[v][i]) return true;
            if(matrix[i][u] == matrix[v][i]) return true;
        }
        return false;
    }

    void bfs(int source){
        for(int i = 0; i<nV; i++){
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
            for(int v = 0; v<nV; v++){
                if(matrix[u][v] != 1) continue;
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
        if(matrix) clearGraph();
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
        }while(!G.addEdge(u, v)) ;
    }
    cout<<"generated"<<endl;

    long double tottime = 0;
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
