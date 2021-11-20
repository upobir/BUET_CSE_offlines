#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <cmath>
using namespace std;

#define debug(x) cerr<<#x<<" = "<<x<<endl;

class Edge{
    int u, v;
    double cost;
public:
    Edge(int u, int v, double cost){
        this->u = u;
        this->v = v;
        this->cost = cost;
    }
    double getCost() {return cost;}
    void setCost(double cost) {this->cost = cost;}
    int getU() {return u;}
    int getV() {return v;}
};

class Graph{
    int nV;
    vector<vector<Edge*>> adj;
    double **distMat;
    int **parMat;
    void clearGraph(){
        for(int u = 0; u<=nV; u++){
            for(int i = 0; i<adj[u].size(); i++)
                delete adj[u][i];
        }
        for(int u = 0; u<=nV; u++){
            delete[] distMat[u];
            delete[] parMat[u];
        }
        if(distMat) delete[] distMat;
        if(parMat) delete[] parMat;
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
        adj.resize(nV+1);
        parMat = new int*[nV+1];
        distMat = new double*[nV+1];
        for(int i = 0; i<=nV; i++) {
            parMat[i] = new int[nV+1];
            distMat[i] = new double[nV+1];
        }
        cleanSPInfo();
    }
    Edge* searchEdge(int u, int v){
        for(int i = 0; i<adj[u].size(); i++){
            if(adj[u][i]->getV() == v) return adj[u][i];
        }
        return nullptr;
    }
    bool isEdge(int u, int v){
        return searchEdge(u, v) != nullptr;
    }
    bool addEdge(int u, int v, double w){
        if(isEdge(u, v)) return false;
        adj[u].push_back(new Edge(u, v, w));
        return true;
    }
    void printGraph(){
        for(int u = 1; u<=nV; u++){
            cout<<u<<" : ";
            for(int i = 0; i<adj[u].size(); i++){
                if(i) cout<<"-->";
                cout<<adj[u][i]->getV()<<"("<<adj[u][i]->getCost()<<")";
            }
            cout<<endl;
        }
        return;
    }
    void removeEdge(int u, int v){
        for(int i = 0; i<adj[u].size(); i--){
            if(adj[u][i]->getV() != v) continue;
            delete adj[u][i];
            adj[u].erase(adj[u].begin()+i);
            break;
        }
        return;
    }
    void reweightEdge(int u, int v, double w){
        Edge * ep = searchEdge(u, v);
        if(ep == nullptr) addEdge(u, v, w);
        else ep->setCost(w);
        return;
    }
    double getWeight(int u, int v){
        Edge * ep = searchEdge(u, v);
        if(ep == nullptr) return INFINITY;
        else return ep->getCost();
    }
    void cleanSPInfo(){
        for(int u = 0; u<=nV; u++)
            for(int v = 0; v <=nV; v++){
                distMat[u][v] = INFINITY;
                parMat[u][v] = -1;
            }
        return;
    }
    void Dijkstra(int u){
        distMat[u][u] = 0;
        priority_queue<pair<double, int>> pq;
        pq.emplace(0, u);
        while(!pq.empty()){
            double c = - pq.top().first;
            int v = pq.top().second;
            pq.pop();
            if(c > distMat[u][v]) continue;
            for(int i = 0; i<adj[v].size(); i++){
                int w = adj[v][i]->getV();
                if(distMat[u][w] > distMat[u][v] + adj[v][i]->getCost()){
                    distMat[u][w] = distMat[u][v] + adj[v][i]->getCost();
                    parMat[u][w] = v;
                    pq.emplace(-distMat[u][w], w);
                }
            }
        }
        return;
    }
    bool BellmanFord(int u){
        distMat[u][u] = 0;
        int x;
        for(int i = 0; i<nV; i++){
            x = -1;
            for(int v = 1; v <= nV; v++){
                for(int j = 0; j < adj[v].size(); j++){
                    int w = adj[v][j]->getV();
                    if(distMat[u][w] > distMat[u][v] + adj[v][j]->getCost()){
                        distMat[u][w] = distMat[u][v] + adj[v][j]->getCost();
                        parMat[u][w] = v;
                        x = w;
                    }
                }
            }
        }
        return (x != -1);
    }
    void FloydWarshall(){
        for(int u = 1; u<=nV; u++) {
            distMat[u][u] = 0, parMat[u][u] = -1;
            for(int i = 0; i<adj[u].size(); i++){
                int v = adj[u][i]->getV();
                distMat[u][v] = adj[u][i]->getCost();
                parMat[u][v] = u;
            }
        }
        for(int w = 1; w<=nV; w++)
            for(int u = 1; u<=nV; u++)
                for(int v = 1; v<=nV; v++)
                    if(distMat[u][v] > distMat[u][w] + distMat[w][v]){
                        distMat[u][v] = distMat[u][w] + distMat[w][v];
                        parMat[u][v] = (w == v)? parMat[u][v] : parMat[w][v];
                    }
        return;
    }
    void printDistMat(){
        for(int u = 1; u<=nV; u++){
            for(int v = 1; v<=nV; v++){
                if(distMat[u][v] == INFINITY) cout<<"INF ";
                else cout<<distMat[u][v]<<" ";
            }
            cout<<endl;
        }
    }
    void printPredMat(){
        for(int u = 1; u<=nV; u++){
            for(int v = 1; v<=nV; v++){
                if(parMat[u][v] == -1) cout<<"NIL ";
                else cout<<parMat[u][v]<<" ";
            }
            cout<<endl;
        }
    }
    double getShortestPathWeight(int u, int v){
        return distMat[u][v];
    }
    void printShortestPath(int u, int v){
        vector<int> path;
        path.push_back(v);
        while(parMat[u][v] != -1){
            v = parMat[u][v];
            path.push_back(v);
        }
        for(int i = path.size()-1; i>=0; i--){
            if(i+1 != path.size()) cout<<"-->";
            cout<<path[i];
            if(i+1 != path.size()) cout<<"("<<searchEdge(path[i+1], path[i])->getCost()<<")";
        }
        cout<<endl;
        return;
    }
    bool JohnsonAlgo(){
        Graph G1;
        G1.setnVertices(nV+1);
        for(int u = 1; u <= nV; u++){
            G1.addEdge(nV+1, u, 0);
            for(int i = 0; i<adj[u].size(); i++)
                G1.addEdge(u, adj[u][i]->getV(), adj[u][i]->getCost());
        }
        if(G1.BellmanFord(nV+1)) return true;
        for(int u = 1; u<=nV; u++){
            double sw = G1.getShortestPathWeight(nV+1, u);
            for(int i = 0; i<adj[u].size(); i++){
                int v = adj[u][i]->getV();
                double tw = G1.getShortestPathWeight(nV+1, v);
                adj[u][i]->setCost(adj[u][i]->getCost()+sw-tw);
            }
        }
        for(int u = 1; u<=nV; u++){
            Dijkstra(u);
            double sw = G1.getShortestPathWeight(nV+1, u);
            for(int v = 1; v<=nV; v++){
                double tw = G1.getShortestPathWeight(nV+1, v);
                distMat[u][v] -= sw-tw;
            }
        }
        for(int u = 1; u<=nV; u++){
            double sw = G1.getShortestPathWeight(nV+1, u);
            for(int i = 0; i<adj[u].size(); i++){
                int v = adj[u][i]->getV();
                double tw = G1.getShortestPathWeight(nV+1, v);
                adj[u][i]->setCost(adj[u][i]->getCost()+tw-sw);
            }
        }
        return false;
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
        double w;
        cin>>u>>v>>w;
        G.addEdge(u, v, w);
    }
    cout<<"Graph Created."<<endl;
    while(1){
        int c;
        cin>>c;
        if(c < 1 || c > 7) break;
        switch(c){
        case 1:
            G.cleanSPInfo();
            cout<<"APSP matrices cleared"<<endl;
            break;
        case 2:
            G.FloydWarshall();
            cout<<"Floyd-Warshall algorithm run"<<endl;
            break;
        case 3:
            cout<<"Johnson's algorithm run"<<endl;
            if(G.JohnsonAlgo())
                cout<<"Negative Cycle in Graph"<<endl;
            break;
        case 4:
            int u, v;
            cin>>u>>v;
            cout<<"Shortest Path Weight = "<<G.getShortestPathWeight(u, v)<<endl;
            cout<<"Path: ";
            G.printShortestPath(u, v);
            break;
        case 5:
            cout<<"Graph:"<<endl;
            G.printGraph();
            break;
        case 6:
            cout<<"Distance Matrix"<<endl;
            G.printDistMat();
            break;
        case 7:
            cout<<"Predecessor Matrix"<<endl;
            G.printPredMat();
            break;
        }
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
