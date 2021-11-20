#include <iostream>
#include <cassert>
using namespace std;

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
        if(!validVertex(source)) return;
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
            cout<<"BFS visiting "<<u<<endl;
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
        if(!validVertex(source)) return;
        for(int i = 0; i<nV; i++){
            dfsColor[i] = WHITE;
            dfsPar[i] = -1;
        }
        clk = 0;
        dfsVisit(source);
    }



    int getDist(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return -1;
        bfs(u);
        if(bfsDist[v] == 1000000000) return -1;
        else return bfsDist[v];
    }

    ~Graph(){
        if(lists) clearGraph();
    }

};


int main(){
    Graph G;
    cout<<"1. Undirected Graph \t 2. Directed graph"<<endl;
    int com;
    cin>>com;
    if(com == 1) G = Graph(false);
    else if(com == 2) G = Graph(true);
    while(1){
        cout<<"Enter Command:"<<endl;
        cout<<"1. Set node count \t 2. Add edge"<<endl;
        cout<<"3. Print Graph \t\t 4. RemoveEdge"<<endl;
        cout<<"5. Is edge? \t\t 6. Indegree"<<endl;
        cout<<"7. Outdegree \t\t 8. Has common adjacent"<<endl;
        cout<<"9. BFS \t\t\t 10. DFS"<<endl;
        cout<<"11. Distance "<<endl;

        cin>>com;
        if(com == 1){
            cout<<"Enter count: ";
            int n;
            cin>>n;
            G.setnVertices(n);
        }
        else if(com == 2){
            cout<<"Enter u v: ";
            int u, v;
            cin>>u>>v;
            if(G.addEdge(u, v)) cout<<"Edge added"<<endl;
            else cout<<"Edge not added"<<endl;
        }
        else if(com == 3){
            G.printGraph();
        }
        else if(com == 4){
            cout<<"Enter u v: ";
            int u, v;
            cin>>u>>v;
            G.removeEdge(u, v);
        }
        else if(com == 5){
            cout<<"Enter u v: ";
            int u, v;
            cin>>u>>v;
            if(G.isEdge(u, v)) cout<<"Yes, such an edge exists"<<endl;
            else cout<<"No, such an edge doesn't exist"<<endl;
        }
        else if(com == 6){
            cout<<"Enter vertex: ";
            int u;
            cin>>u;
            cout<<"indegree: "<<G.getInDegree(u)<<endl;
        }
        else if(com == 7){
            cout<<"Enter vertex: ";
            int u;
            cin>>u;
            cout<<"outdegree: "<<G.getOutDegree(u)<<endl;
        }
        else if(com == 8){
            cout<<"Enter u v: ";
            int u, v;
            cin>>u>>v;
            if(G.hasCommonAdjacent(u, v)) cout<<"Yes\n";
            else cout<<"No\n";
        }
        else if(com == 9){
            cout<<"Enter source: ";
            int u;
            cin>>u;
            G.bfs(u);
        }
        else if(com == 10){
            cout<<"Enter source: ";
            int u;
            cin>>u;
            G.dfs(u);
        }
        else if(com == 11){
            cout<<"Enter u v: ";
            int u, v;
            cin>>u>>v;
            int tmp = G.getDist(u, v);
            if(tmp == -1) cout<<"Not connected"<<endl;
            else cout<<"Distance : "<<tmp<<endl;
        }
        else{
            cout<<"invalid command\n";
        }
        cout<<endl;
    }

    return 0;
}
