#include<iostream>
using namespace std;

class DSU{
    class element{
    public:
        int parent;
        int rank;
        element(){
            parent = -1;
            rank = 0;
        }

    } *arr;
    int sz;

    void resize(int n){
        element *tmp = new element[n];
        for(int i = 0; i<sz; i++) tmp[i] = arr[i];
        delete[] arr;
        arr = tmp;
        sz = n;
    }

public:
    DSU(){
        arr = new element[1];
        sz = 1;
    }

    ~DSU(){
        delete[] arr;
    }

    void make_set(int id){
        if(id >= 2*sz)
            resize(id+1);
        else if(id >= sz)
            resize(2*sz);

        arr[id].parent = id;
    }

    int find_set(int x){
        if(arr[x].parent == x) return x;
        return find_set(arr[x].parent);
    }

    void union_set(int u, int v){
        u = find_set(u);
        v = find_set(v);
        if(u == v) return;
        if(arr[u].rank < arr[v].rank){
            arr[u].parent = v;
        }
        else{
            arr[v].parent = u;
            if(arr[u].rank == arr[v].rank) arr[u].rank++;
        }
    }

    void print(int x){
        x = find_set(x);
        for(int i = 0; i<sz; i++){
            if(arr[i].parent != -1 && find_set(i) == x)
                cout<<i<<" ";
        }
        cout<<endl;
    }
};

int main()
{
    DSU dsu;
    do{
        cout<<"Enter choice:"<<endl;
        cout<<"1. Make Set\t 2. Find Set"<<endl;
        cout<<"3. Unite   \t 4. Print"<<endl;
        cout<<"5. Quit"<<endl;

        int c;
        cin>>c;
        if(c == 1){
            cout<<"Enter new id: ";
            int x;
            cin>>x;
            dsu.make_set(x);
        }
        else if(c == 2){
            cout<<"Enter set id: ";
            int x;
            cin>>x;
            cout<<"Representative of set of "<<x<<" is : "<<dsu.find_set(x)<<endl;
        }
        else if(c == 3){
            cout<<"Enter sets to unite: ";
            int u, v;
            cin>>u>>v;
            dsu.union_set(u, v);
        }
        else if(c == 4){
            cout<<"Enter set to print elements of: ";
            int x;
            cin>>x;
            cout<<"Elements of set of "<<x<<" : ";
            dsu.print(x);
        }
        else if(c == 5){
            break;
        }
        cout<<endl;

    }while(true);
}
