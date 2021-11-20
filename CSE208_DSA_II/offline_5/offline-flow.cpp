#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Edge {
private:
    int u, v;
    int cap, flow;
    Edge *rev;
public:
    Edge(int u, int v, int cap, int flow){
        this->u = u;
        this->v = v;
        this->cap = cap;
        this->flow = flow;
    }
    void assignRevEdge(Edge * rev){
        this->rev = rev;
    }
    int getU(){return u;}
    int getV(){return v;}
    int getFlow(){return flow;}
    int getCap(){return cap;}
    int getResCap(){return cap-flow;}

    void sendFlow(int f){
        flow += f;
        rev->flow -= f;
    }
};

class FlowGraph {
private:
    int V;

    vector<int> dist;
    vector<Edge*> edges, prev;
    bool bfs(int s, int t){
        fill(dist.begin(), dist.end(), V+1);
        queue<int> q;
        q.push(s);
        dist[s] = 0;

        while(!q.empty()){
            int u = q.front();
            q.pop();

            for(Edge * e : adjList[u]){
                int v = e->getV();
                if(e->getResCap() > 0 && dist[v] > dist[u]+1){
                    dist[v] = dist[u]+1;
                    prev[v] = e;
                    q.push(v);
                }
            }
        }
        return dist[t] <= V;
    }

    int sendFlow(int s, int t){
        int cur = t;
        int flow = 1'000'000'000;
        while(cur != s){
            flow = min(flow, prev[cur]->getResCap());
            cur = prev[cur]->getU();
        }

        cur = t;
        while(cur != s){
            prev[cur]->sendFlow(flow);
            cur = prev[cur]->getU();
        }

        return flow;
    }
protected:
    vector<vector<Edge*>> adjList;
public:
    FlowGraph(int v){
        V = v;
        adjList.resize(V);
        dist.resize(V);
        prev.resize(V);
    }

    void addEdge(int u, int v, int c){
        Edge* eF = new Edge(u, v, c, 0);
        Edge* eR = new Edge(v, u, c, c);

        eF->assignRevEdge(eR);
        eR->assignRevEdge(eF);

        edges.push_back(eF);
        adjList[u].push_back(eF);
        adjList[v].push_back(eR);
    }



    int maximumFlow(int s, int t){
        int ans = 0;
        while(bfs(s, t)){
            ans += sendFlow(s, t);
        }
        return ans;
    }

    vector<Edge> getEdges(){
        vector<Edge> ret;
        for(Edge * e : edges)
            ret.push_back(*e);
        return ret;
    }

    ~FlowGraph(){
        for(int u = 0; u<V; u++){
            for(Edge* e : adjList[u]){
                delete e;
            }
        }
    }
};

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int V, E;
    cin>>V>>E;
    FlowGraph G(V);
    for(int i = 0; i<E; i++){
        int u, v, c;
        cin>>u>>v>>c;
        G.addEdge(u, v, c);
    }

    int s, t;
    cin>>s>>t;

    cout<<G.maximumFlow(s, t)<<endl;

    for(Edge e : G.getEdges()){
        cout<<e.getU()<<" "<<e.getV()<<" "<<e.getFlow()<<"/"<<e.getCap()<<endl;
    }


    return 0;
}
