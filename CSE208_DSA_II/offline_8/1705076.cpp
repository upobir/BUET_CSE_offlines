#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <time.h>
#include <vector>

using namespace std;
mt19937 rng(time(0));

class ChainingHashTable{
private:
    struct Data{
        Data* next;
        string key;
        int value;
    };

    int N, sz;
    int numCollision, numProbe;
    Data** arr;

    int (*Hash)(string&, int);
public:
    ChainingHashTable(int N, int(*Hash)(string&, int)){
        this->N = N;
        this->Hash = Hash;
        this->sz = this->numCollision = this->numProbe = 0;
        this->arr = new Data*[N];
        for(int i = 0; i<N; i++)
            arr[i] = nullptr;
    }

    ~ChainingHashTable(){
        for(int i = 0; i<N; i++){
            Data* cur = arr[i];
            while(cur){
                Data* temp = cur->next;
                delete cur;
                cur = temp;
            }
        }
        delete[] arr;
    }

    void insert(string str){
        int index = Hash(str, N);

        if(arr[index]){
            Data* cur = arr[index];

            while(cur->next){
                if(cur->key == str)
                    return;
                cur = cur->next;
            }

            if(cur->key == str)
                return;

            numCollision++;
            cur->next = new Data{nullptr, str, ++sz};
        }
        else{
            arr[index] = new Data{nullptr, str, ++sz};
        }
    }

    int find(string str){
        int index = Hash(str, N);

        Data* cur = arr[index];
        while(cur){
            numProbe++;
            if(cur->key == str)
                return cur->value;
            cur = cur->next;
        }
        return 0;
    }

    void remove(string str){
        int index = Hash(str, N);
        Data* temp = nullptr;
        if(arr[index]){
            if(arr[index]->key == str){
                temp = arr[index];
                arr[index] = temp->next;
            }
            else{
                Data *cur = arr[index];
                while(cur->next){
                    if(cur->next->key == str){
                        temp = cur->next;
                        cur->next = cur->next->next;
                        break;
                    }
                    cur = cur->next;
                }
            }
        }
        delete temp;
        return;
    }

    int getCollisionCount(){
        return numCollision;
    }

    int getProbeCount(){
        return numProbe;
    }
};

class DoubleHashTable{
private:
    struct Data{
        bool filled;
        string key;
        int value;
    };

    int N, sz;
    int numCollision, numProbe;
    Data* arr;

    int (*Hash)(string&, int);
    int (*auxHash)(string&);
public:
    DoubleHashTable(int N, int (*Hash)(string&, int), int (*auxHash)(string&)){
        this->N = N;
        this->sz = this->numCollision = this->numProbe = 0;
        this->Hash = Hash;
        this->auxHash = auxHash;
        this->arr = new Data[N];
    }
    ~DoubleHashTable(){
        delete[] arr;
    }

    void insert(string str){
        for(int i = 0; ; i++){
            int index = (Hash(str, N) + i * auxHash(str)) % N;
            if(!arr[index].filled){
                arr[index] = {true, str, ++sz};
                numCollision += i;
                return;
            }
            else if(arr[i].key == str){
                return;
            }
        }

    }

    int find(string str){
        for(int i = 0; i<N; i++){
            numProbe++;
            int index = (Hash(str, N) + i * auxHash(str)) % N;
            if(!arr[index].filled)
                break;
            if(arr[index].key == str)
                return arr[index].value;
        }
        return 0;
    }

    void remove(string str){
        for(int i = 0; i<N; i++){
            int index = (Hash(str, N) + i * auxHash(str)) % N;
            if(!arr[index].filled)
                break;
            if(arr[index].key == str){
                arr[index] = {true, "", 0};
                break;
            }
        }
        return;
    }

    int getCollisionCount(){
        return numCollision;
    }

    int getProbeCount(){
        return numProbe;
    }
};

class CustomHashTable{
private:
    struct Data{
        bool filled;
        string key;
        int value;
    };

    int N, sz;
    int numCollision, numProbe;
    Data* arr;
    long long int C1, C2;

    int (*Hash)(string&, int);
    int (*auxHash)(string&);
public:
    CustomHashTable(int N, int (*Hash)(string&, int), int (*auxHash)(string&), long long int C1 = 13, long long int C2 = 20){
        this->N = N;
        this->Hash = Hash;
        this->auxHash = auxHash;
        this->sz = this->numCollision = this->numProbe = 0;
        this->C1 = C1;
        this->C2 = C2;
        this->arr = new Data[N];
    }
    ~CustomHashTable(){
        delete[] arr;
    }

    void insert(string str){
        for(int i = 0; ; i++){
            int index = (Hash(str, N) + i * C1 * auxHash(str) + C2*i*i) % N;
            if(!arr[index].filled){
                arr[index] = {true, str, ++sz};
                numCollision += i;
                return;
            }
            else if(arr[i].key == str){
                return;
            }
        }
    }

    int find(string str){
        for(int i = 0; i<N; i++){
            numProbe++;
            int index = (Hash(str, N) + i * C1 * auxHash(str) + C2*i*i) % N;
            if(!arr[index].filled)
                break;
            if(arr[index].key == str)
                return arr[index].value;
        }
        return 0;
    }

    void remove(string str){
        for(int i = 0; i<N; i++){
            int index = (Hash(str, N) + i * C1 * auxHash(str) + C2*i*i) % N;
            if(!arr[index].filled)
                break;
            if(arr[index].key == str){
                arr[index] = {true, "", 0};
                break;
            }
        }
        return;
    }

    int getCollisionCount(){
        return numCollision;
    }

    int getProbeCount(){
        return numProbe;
    }
};

int hash1(string &str, int n){
    long long int h = 0;
    for(int c : str){
        h = ((h*102000890) + c)%1000000007;
    }
    double tmp;
    return int(n * modf(h*0.618033, &tmp));
}

int hash2(string &str, int n){
    long long int h = 0;
    for(int c : str){
        h = ((h^102000890)*c)%1000000007;
    }
    double tmp;
    return int(n * modf(h*0.618033, &tmp));
}

int auxHash(string &str){
    int h = 0;
    for(int c : str){
        h ^= (c << 4) ^ c;
    }
    return 10*h+1;
}

string all = "abcdefghijklmnopqrstuvwxyz";
string randomString(int n){
    string ret;
    for(int i = 0; i<n; i++){
        ret.push_back(all[rng()%26]);
    }
    return ret;
}

int main(){

    int n = 10000;
    int m = 1000;

    ChainingHashTable table1(n, &hash2);
    DoubleHashTable table2(n, &hash2, &auxHash);
    CustomHashTable table3(n, &hash2, &auxHash);

    vector<string> strings;
    for(int i = 0; i<n; i++){
        string str = randomString(7);
        strings.push_back(str);
        table1.insert(str);
        table2.insert(str);
        table3.insert(str);
    }

    for(int i = 0; i<m; i++){
        string str = strings[rng()%n];
        table1.find(str);
        table2.find(str);
        table3.find(str);
    }

    cout<<"--------Chaining---------"<<endl;
    cout<<"collisions: "<<table1.getCollisionCount()<<endl;
    cout<<"avarage probes: "<<table1.getProbeCount()/(1.0*m)<<endl;
    cout<<endl;

    cout<<"--------Double Hashing---------"<<endl;
    cout<<"collisions: "<<table2.getCollisionCount()<<endl;
    cout<<"avarage probes: "<<table2.getProbeCount()/(1.0*m)<<endl;
    cout<<endl;

    cout<<"--------Custom Probing---------"<<endl;
    cout<<"collisions: "<<table3.getCollisionCount()<<endl;
    cout<<"avarage probes: "<<table3.getProbeCount()/(1.0*m)<<endl;
    cout<<endl;

    return 0;
}
