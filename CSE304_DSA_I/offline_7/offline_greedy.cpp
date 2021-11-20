#include<iostream>
#include<random>
#include<time.h>
using namespace std;
///using this instead of rand(), as rand() has range upto only 2^16
mt19937 rng(time(0));

class interval{
    int id;
    int st, en;
    bool taken;
public:
    interval(){}
    interval(int _i, int _s, int _e){
        id = _i;
        st = _s;
        en = _e;
        taken = false;
    }

    int getStart(){return st;}
    int getEnd(){return en;}
    int getId(){return id;}
    bool isTaken(){return taken;}

    void take(){taken = true;}

    bool operator<(interval other){
        if(en != other.en) return (en < other.en);
        else return (st < other.st);
    }
};

void MergeSort(interval *arr, int n);
int greedyCount(interval *arr, int n);

int main()
{
    while(1){
        int n, m;
        interval *arr;
        cout<<"Manual(1) or Random(2)?"<<endl;
        cin>>m;
        cout<<"Enter number of intervals: ";
        cin>>n;
        arr = new interval[n];
        if(m == 1){
            cout<<"Enter start points and end points: "<<endl;
            for(int i = 0; i<n; i++){
                int l, r;
                cin>>l>>r;
                arr[i] = interval(i+1, l, r);
            }
        }
        else{
            for(int i = 0; i<n; i++){
                int l = rng()%100+1;
                int r = rng()%100+1;
                if(l < r)
                    arr[i] = interval(i+1, l, r);
                else
                    arr[i] = interval(i+1, r, l);
                cout<<arr[i].getStart()<<" "<<arr[i].getEnd()<<endl;
            }

        }
        int ans = greedyCount(arr, n);
        cout<<"The maximum number of compatible intervals: "<<ans<<endl;

        cout<<"Greedy selection: ";
        for(int i = 0; i<n; i++){
            if(arr[i].isTaken()) cout<<arr[i].getId()<<" ";
        }
        cout<<endl<<endl;

        delete[] arr;
    }
    return 0;
}


///arr = begin pointer, n = size
void MergeSort(interval *arr, int n)
{
    if(n == 1) return;
    MergeSort(arr, n/2);
    MergeSort(arr+n/2, n-n/2);
    interval *tmp = new interval[n];
    for(int i = 0, j = n/2, k = 0; k<n; k++)
        if(j == n || (i < n/2 && arr[i] < arr[j])) tmp[k] = arr[i++];
        else tmp[k] = arr[j++];
    for(int i = 0; i<n; i++) arr[i] = tmp[i];

    delete[] tmp;
    return;
}

int greedyCount(interval *arr, int n)
{
    MergeSort(arr, n);
    int last = -99999999;
    int ans = 0;
    for(int i = 0; i<n; i++){
        if(arr[i].getStart() >= last){
            ans++;
            last = arr[i].getEnd();
            arr[i].take();
        }
    }
    return ans;
}
