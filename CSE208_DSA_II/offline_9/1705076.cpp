#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <chrono>
#include <numeric>
#include <algorithm>
using namespace std;

class TSP_brute{
public:
    void setVertices(int _V){
        V = _V;
        coords.resize(V+1);
        dist.resize(V+1, vector<double>(V+1));
    }

    void setDistance(int u, int v, double d){
        dist[u][v] = (u == v)? 0 : d;
    }

    void setCoordinate(int u, double x, double y){
        coords[u] = make_pair(x, y);
    }

    void computeDistances(){
        for(int u = 1; u<=V; u++){
            for(int v = 1; v <= u; v++){
                double d = hypot(coords[u].first - coords[v].first, coords[u].second - coords[v].second);
                dist[u][v] = d;
                dist[v][u] = d;
            }
        }
    }

    double solve(){
        used.resize(V+1, false);
        return recurse(1, 1, 0);
    }

    double recurse(int u, int cnt, double path){
        if(cnt == V){
            return path + dist[u][1];
        }

        used[u] = true;

        double ret = numeric_limits<double>::infinity();
        for(int v = 2; v<=V; v++){
            if(used[v]) continue;
            double tmp = recurse(v, cnt+1, path + dist[u][v]);
            ret = min(ret, tmp);
        }

        used[u] = false;
        return ret;
    }
private:
    int V;
    vector<pair<double, double>> coords;
    vector<vector<double>> dist;
    vector<bool> used;
};

class TSP_bnb{
public:
    void setVertices(int _V){
        V = _V;
        coords.resize(V+1);
        dist.resize(V+1, vector<double>(V+1));
    }

    void setDistance(int u, int v, double d){
        dist[u][v] = (u == v)? 0 : d;
    }

    void setCoordinate(int u, double x, double y){
        coords[u] = make_pair(x, y);
    }

    void computeDistances(){
        for(int u = 1; u<=V; u++){
            for(int v = 1; v <= u; v++){
                double d = hypot(coords[u].first - coords[v].first, coords[u].second - coords[v].second);
                dist[u][v] = d;
                dist[v][u] = d;
            }
        }
    }

    double solve(){
        ans = 0;
        int cur = 1;
        used.resize(V+1, false);
        used[cur] = true;

        for(int i = 1; i<V; i++){
            int x = -1;
            for(int u = 1; u<=V; u++){
                if(used[u]) continue;
                if(x == -1 || dist[cur][u] < dist[cur][x])
                    x = u;
            }
            ans += dist[cur][x];
            cur = x;
            used[cur] = true;
        }
        ans += dist[cur][1];


        used.clear();
        used.resize(V+1, false);
        heauristic.resize(V+1);

        double lb = 0;
        for(int u = 1; u<=V; u++){
            double mn1 = numeric_limits<double>::infinity();
            double mn2 = numeric_limits<double>::infinity();
            for(int v = 1; v<=V; v++){
                if(u == v) continue;
                mn1 = min(mn1, dist[u][v]);
                mn2 = min(mn2, dist[v][u]);
            }
            heauristic[u] = (V == 1)? 0 : (mn1+mn2)/2;
            lb += heauristic[u];
        }
        recurse(1, 1, 0, lb-heauristic[1]);
        return ans;
    }

    void recurse(int u, int cnt, double path, double lb){
        if(cnt == V){
            ans = min(path + dist[u][1], ans);
            return;
        }

        used[u] = true;
        for(int v = 2; v<=V; v++){
            if(used[v]) continue;
            if(path+dist[u][v] + lb - heauristic[v] >= ans)
                continue;
            recurse(v, cnt+1, path + dist[u][v], lb - heauristic[v]);
        }

        used[u] = false;
        return;
    }
private:
    int V;
    double ans;
    vector<pair<double, double>> coords;
    vector<vector<double>> dist;
    vector<bool> used;
    vector<double> heauristic;
};

int main(){
    TSP_bnb T;

    string type;
    cin>>type;
    int n;
    if(type.substr(0, 4) == "data"){
        cin>>n;
        T.setVertices(n);
        for(int i = 1; i<=n; i++){
            for(int j = 1; j<=n; j++){
                double d;
                cin>>d;
                T.setDistance(i, j, d);
            }
        }
    }
    else{
        cin>>n;
        T.setVertices(n);
        for(int i = 1; i<=n; i++){
            int id;
            double x, y;
            cin>>id>>x>>y;
            T.setCoordinate(id, x, y);
        }
        T.computeDistances();
    }

    cout<<"Number of vertices "<<n<<endl;


    auto start = chrono::high_resolution_clock::now();

    cout<<"distance: "<<T.solve()<<endl;

    auto finish = chrono::high_resolution_clock::now();
    auto seconds = chrono::duration_cast<chrono::milliseconds>(finish - start);

    cout << "Elapsed Time: " << seconds.count() << " milliseconds" << endl;
    cout<<endl;

}
