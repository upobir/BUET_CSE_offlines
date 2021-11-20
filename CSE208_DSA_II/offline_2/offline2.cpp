#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

/*
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
*/

#define INF 1000000000
class Graph{
    int nV;
    vector<vector<pair<int, int>>> lists;
    vector<int> dist, par;
    bool dir;

    void clearGraph(){
        lists.clear();
        dist.clear();
        par.clear();
        nV = 0;
    }
    bool validVertex(int u){
        return (0 <= u && u < nV);
    }

public:
    Graph(bool directed = false){
        dir = directed;
        nV = 0;
    }
    void setnVertices(int n){
        if(lists.size()) clearGraph();
        nV = n;
        lists.resize(nV);
        dist.resize(nV);
        par.resize(nV);
    }

    bool isEdge(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return false;
        for(auto p: lists[u]){
            if(p.first == v) return true;
        }
        return false;
    }

    bool addEdge(int u, int v, int w){
        if(!validVertex(u) || !validVertex(v)) return false;
        if(isEdge(u, v)) return false;
        lists[u].emplace_back(v, w);
        if(!dir) lists[v].emplace_back(u, w);
        return true;
    }

    void printGraph(){
        cout<<endl;
        for(int i = 0; i<nV; i++){
            cout<<i<<" : ";
            for(auto p: lists[i]){
                cout<<p.first<<"("<<p.second<<")"<<" ";
            }
            cout<<endl;
        }
    }

    void removeEdge(int u, int v){
        if(!validVertex(u) || !validVertex(v)) return;
        for(int i = 0; i<lists[u].size(); i++){
            if(lists[u][i].first == v){
                lists[u].erase(lists[u].begin()+i);
                break;
            }
        }
        if(!dir){
            for(int i = 0; i<lists[v].size(); i++){
                if(lists[v][i].first == u){
                    lists[v].erase(lists[v].begin()+i);
                    break;
                }
            }
        }
    }

    int getOutDegree(int u){
        if(!validVertex(u)) return -1;
        return lists[u].size();
    }

    int getInDegree(int u){
        if(!validVertex(u)) return -1;
        int degree = 0;
        for(int i = 0; i<nV; i++)
            if(isEdge(i, u)) degree++;
        return degree;
    }

    void printpath(int t){
        vector<int> path;
        path.push_back(t);
        while(par[t] != -1){
            t = par[t];
            path.push_back(t);
        }

        reverse(path.begin(), path.end());
        for(int i = 0; i<path.size(); i++){
            cout<<path[i];
            if(i + 1 != path.size())
                cout<<" -> ";
        }
        cout<<endl;
    }

    void dijkstra(int s, int t){
        if(!validVertex(s) || !validVertex(t)) return;
        for(int i = 0; i<nV; i++) dist[i] = INF, par[i] = -1;
        dist[s] = 0, par[s] = -1;
        priority_queue<pair<int, int>> pq;
        pq.emplace(0, s);
        while(!pq.empty()){
            auto p = pq.top();
            pq.pop();
            int d = -p.first, u = p.second;
            if(dist[u] != d) continue;
            if(u == t) break;
            for(auto q: lists[u]){
                if(dist[u] + abs(q.second) < dist[q.first]){
                    dist[q.first] = dist[u] + abs(q.second);
                    par[q.first] = u;
                    pq.emplace(-dist[q.first], q.first);
                }
            }
        }

        if(dist[t] == INF){
            cout<<"NOT REACHABLE"<<endl;
            return;
        }

        cout<<dist[t]<<endl;
        printpath(t);
    }

    void bellmanford(int s, int t){
        if(!validVertex(s) || !validVertex(t)) return;
        for(int i = 0; i<nV; i++) dist[i] = INF, par[i] = -1;
        dist[s] = 0;
        int x;
        for(int i = 0; i<nV; i++){
            x = -1;
            for(int u = 0; u < nV; u++){
                if(dist[u] == INF) continue;
                for(auto p: lists[u]){
                    if(dist[u] + p.second <  dist[p.first]){
                        dist[p.first] = max(-INF, dist[u] + p.second);
                        par[p.first] = u;
                        x = p.first;
                    }
                }
            }


        }
        if(x != -1){
            cerr<<x<<endl;
            cout<<"Negative Cycle"<<endl;
            return;
        }
        if(dist[t] == INF){
            cout<<"NOT REACHABLE"<<endl;
            return;
        }

        cout<<dist[t]<<endl;
        printpath(t);
    }

};



int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int V, E;
    cin>>V>>E;
    Graph G(true);
    G.setnVertices(V);

    for(int i = 0; i<E; i++){
        int u, v, w;
        cin>>u>>v>>w;
        G.addEdge(u, v, w);
    }

    int s, t;
    cin>>s>>t;

//    G.printGraph();

    cout<<"Bellman For Algorithm: "<<endl;
    G.bellmanford(s, t);

    cout<<"Dijkstra Algorithm: "<<endl;
    G.dijkstra(s, t);




    return 0;
}

