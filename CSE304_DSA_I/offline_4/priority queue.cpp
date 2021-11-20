#include<iostream>
#include<cassert>
using namespace std;

class PriorityQueue{
    int maxcap;
    int *arr;

    void resize(int s){
        if(s < 1) s = 1;
        int *tmp = new int[s];
        for(int i = 0; i<s; i++) tmp[i] = arr[i];
        delete[] arr;
        arr = tmp;
        maxcap = s;
    }
public:
    PriorityQueue(){
        maxcap = 1;
        arr = new int[1];
        arr[0] = 0;
    }

    void Insert(int x){
        if(maxcap == arr[0] + 1)
            resize(2*maxcap);
        ++arr[0];
        arr[arr[0]] = x;
        IncreaseKey(arr[0], x);
    }

    int FindMax(){
        if(arr[0] == 0){
            cout<<"Heap Empty\n";
            return -9999999;
        };
        return arr[1];
    }

    void ExtractMax(){
        if(arr[0] == 0){
            cout<<"Heap Empty\n";
            return;
        }
        arr[0]--;
        DecreaseKey(1, arr[arr[0]+1]);
        if(arr[0] < maxcap/4) resize(maxcap/4);
    }

    void IncreaseKey(int i, int newkey){
        if(i > arr[0] || arr[i] > newkey){
            cout<<"Invalid Increase Command\n";
            return;
        }
        arr[i] = newkey;
        while(i > 1 && arr[i/2] < arr[i]){
            int tmp = arr[i];
            arr[i] = arr[i/2];
            arr[i/2] = tmp;
            i = i/2;
        }
    }

    void DecreaseKey(int i, int newkey){
        if(i > arr[0] || newkey > arr[i]){
            cout<<"Invalid Decrease Command\n";
            return;
        }
        arr[i] = newkey;
        while(true){
            int largest = i;
            if(2*i <= arr[0] && arr[2*i] > arr[largest]) largest = 2*i;
            if(2*i+1 <= arr[0] && arr[2*i+1] > arr[largest]) largest = 2*i+1;
            if(largest == i) break;

            int tmp = arr[largest];
            arr[largest] = arr[i];
            arr[i] = tmp;
            i = largest;
        }
    }

    void Print(){
        for(int i = 1; i<=arr[0]; i++)
            cout<<arr[i]<<" ";
        cout<<"\n";
    }

    ~PriorityQueue(){
        delete[] arr;
    }

};

int main()
{
    PriorityQueue pq;
    while(1){
        cout<<"Enter Command:\n";
        cout<<"1. Insert\t 2. Find Max\n";
        cout<<"3. Extract Max\n";
        cout<<"4. Increase Key\t 5. Decrease Key\n";
        cout<<"6. Print\t 7. Quit\n";
        int com;
        cin>>com;
        if(com == 1){
            cout<<"Enter element: ";
            int x;
            cin>>x;
            pq.Insert(x);
        }
        else if(com == 2){
            cout<<"Max Element: ";
            cout<<pq.FindMax()<<"\n";
        }
        else if(com == 3){
            pq.ExtractMax();
        }
        else if(com == 4){
            cout<<"Enter position (1-based): ";
            int i, x;
            cin>>i;
            cout<<"Enter new key: ";
            cin>>x;
            pq.IncreaseKey(i, x);
        }
        else if(com == 5){
            cout<<"Enter position (1-based): ";
            int i, x;
            cin>>i;
            cout<<"Enter new key: ";
            cin>>x;
            pq.DecreaseKey(i, x);
        }
        else if(com == 6){
            cout<<"Priority Queue: ";
            pq.Print();
        }
        else if(com == 7){
            break;
        }
        else{
            cout<<"invalid command\n";
        }
        cout<<"\n";
    }
}
