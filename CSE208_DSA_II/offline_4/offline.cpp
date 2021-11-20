#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <cmath>
#include <algorithm>
using namespace std;

#define INF 100000000

class Edge{
    int u, v;
    int cost;
public:
    Edge(int u, int v, int cost){
        this->u = u;
        this->v = v;
        this->cost = cost;
    }
    int getCost() {return cost;}
    void setCost(int cost) {this->cost = cost;}
    int getU() {return u;}
    int getV() {return v;}
    bool operator<(const Edge other) const{
        return cost < other.cost;
    }
};

class DSU{
    int nV;
    vector<int> master, sz;
public:
    DSU(int n = 0){
        nV = n;
        master.resize(nV);
        sz.resize(nV);
        for(int i = 0; i<nV; i++)
            master[i] = i, sz[i] = 1;
    }
    int fnd(int u){
        if(master[u] == u) return u;
        return master[u] = fnd(master[u]);
    }
    bool unite(int u, int v){
        u = fnd(u), v = fnd(v);
        if(u == v) return false;
        if(sz[v] > sz[u]) swap(u, v);
        master[v] = u;
        sz[u] += v;
        return true;
    }
};

class Graph{
    int nV;
    vector<vector<Edge>> adj;
    void clearGraph(){
        adj.clear();
        nV = -1;
    }
public:
    Graph(){
        nV = -1;
    }
    void setnVertices(int n){
        assert(n > 0);
        clearGraph();
        nV = n;
        adj.resize(nV);
    }
    bool addEdge(int u, int v, int w){
        adj[u].push_back(Edge(u, v, w));
        adj[v].push_back(Edge(v, u, w));
        return true;
    }
    void printGraph(){
        for(int u = 0; u<nV; u++){
            cout<<u<<" : ";
            for(int i = 0; i<adj[u].size(); i++){
                if(i) cout<<"-->";
                cout<<adj[u][i].getV()<<"("<<adj[u][i].getCost()<<")";
            }
            cout<<endl;
        }
        return;
    }
    int Kruskal(vector<Edge> &vec){
        vec.clear();
        vector<Edge> E;
        for(int i = 0; i<nV; i++){
            for(Edge e: adj[i]){
                if(e.getV() > e.getU())
                    E.push_back(e);
            }
        }
        sort(E.begin(), E.end());
        DSU dsu(nV);
        int ans = 0;
        for(Edge e: E){
            int u = e.getU();
            int v = e.getV();
            if(dsu.unite(u, v)){
                vec.push_back(e);
                ans += e.getCost();
            }
        }
        if(vec.size() != nV-1) {
            ans = -1.0;
            vec.clear();
        }
        return ans;
    }
    int Prim(int root, vector<Edge> &vec){
        vec.clear();
        vector<bool> used(nV, false);
        int ans = 0;
        priority_queue<Edge> epq;
        epq.push(Edge(root, root, 0));
        while(!epq.empty()){
            Edge e = epq.top();
            epq.pop();
            if(used[e.getV()]) continue;
            used[e.getV()] = true;
            e.setCost(-e.getCost());
            if(e.getU() != e.getV())vec.push_back(e);
            for(Edge f: adj[e.getV()]){
                if(!used[f.getV()]){
                    f.setCost(-f.getCost());
                    epq.push(f);
                }
            }
        }
        if(vec.size() != nV-1) {
            ans = -1.0;
            vec.clear();
        }
        return ans;
    }
    ~Graph(){
        clearGraph();
    }
};

int main(){
    int V, E;
    cin>>V>>E;
    Graph G;
    G.setnVertices(V);
    for(int i = 0; i<E; i++){
        int u, v;
        int w;
        cin>>u>>v>>w;
        G.addEdge(u, v, w);
    }
    vector<Edge> v;
    int ans = G.Kruskal(v);
    cout<<ans<<endl;
    cout<<"Kruskal's Algorithm: "<<endl;
    for(Edge e: v){
        cout<<e.getU()<<" "<<e.getV()<<endl;
    }
    int root = 0;
    G.Prim(root, v);
    cout<<"Prim's Algorithm: "<<endl;
    cout<<"Root node = "<<root<<endl;
    for(Edge e: v){
        cout<<e.getU()<<" "<<e.getV()<<endl;
    }
    return 0;
}
/*
5 9
1 2 3
1 3 8
1 5 -4
2 4 1
2 5 7
3 2 4
4 1 2
4 3 -5
5 4 6
*/

