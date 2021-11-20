#include <iostream>
using namespace std;

#define BUFFER 10

class stack
{
    int * memory;
    int sz;
    int maxsize;

    void resize(int s)
    {
        int *temp = new int[s];
        for(int i = 0; i<sz; i++) temp[i] = memory[i];
        delete[] memory;
        memory = temp;
    }
public:
    stack()
    {
        cout<<"..Default construction"<<endl;
        sz = 0;
        maxsize = 0;
        memory = NULL;
    }

    stack(int s)
    {
        cout<<"..Parametrized construction"<<endl;
        sz = 0;
        maxsize = s;
        memory = new int[s];
    }

    stack(const stack &st)
    {
        cout<<"..Copy construction"<<endl;
        sz = st.sz;
        maxsize = st.maxsize;
        memory = new int[maxsize];
        for(int i = 0; i<sz; i++) memory[i] = st.memory[i];
    }

    ~stack()
    {
        cout<<"..Destruction"<<endl;
        delete[] memory;
    }

    int top() {return (sz == 0) ? -1 : memory[sz-1];}
    int size() {return sz;}

    void push(int x)
    {
        if(sz == maxsize) resize(maxsize+BUFFER);
        memory[sz++] = x;
    }

    void push(int arr[], int n)
    {
        for(int i = 0; i<n; i++) push(arr[i]);
    }

    void push(stack st)
    {
        for(int i = st.sz-1; i>= 0; i--) push(st.memory[i]);
    }

    int pop()
    {
        if(sz == 0) return -1;
        int temp = memory[--sz];
        if(sz < maxsize-10) resize(maxsize-BUFFER);
        return temp;
    }

    double similarity(stack st)
    {
        int matches = 0;
        double avgsize = (sz+st.sz)/2.0;
        for(int i = sz-1, j = st.sz-1; i >= 0 && j>= 0; i--, j--)
            if(memory[i] == st.memory[j]) matches++;
        return matches/avgsize;
    }
};

int main()
{
    stack mainStack, *tempStack;
    bool working = true;
    while(working)
    {
        cout<<"Enter command"<<endl;
        cout<<"1.Push an element"<<endl;
        cout<<"2.Push an array"<<endl;
        cout<<"3.Push an stack"<<endl;
        cout<<"4.Pop"<<endl;
        cout<<"5.Top"<<endl;
        cout<<"6.Size"<<endl;
        cout<<"7.Similarity"<<endl;
        cout<<"8.Exit"<<endl;

        int command;
        cin>>command;

        int n, x, *arr;

        switch(command)
        {
        case 1:
            cout<<"Enter element: ";
            cin>>n;
            mainStack.push(n);
            break;
        case 2:
            cout<<"Enter array size: ";
            cin>>n;
            arr = new int[n];
            cout<<"Enter array elements: ";
            for(int i = 0; i<n; i++) cin>>arr[i];
            mainStack.push(arr, n);
            delete[] arr;
            break;
        case 3:
            cout<<"Enter stack size: ";
            cin>>n;
            tempStack = new stack(n);
            cout<<"Enter elements pushed onto stack: ";
            for(int i = 0; i<n; i++)
            {
                cin>>x;
                tempStack->push(x);
            }
            mainStack.push(*tempStack);
            delete tempStack;
            break;
        case 4:
            cout<<"Popped "<<mainStack.pop()<<endl;;
            break;
        case 5:
            cout<<"Top element: "<<mainStack.top()<<endl;
            break;
        case 6:
            cout<<"Size: "<<mainStack.size()<<endl;
            break;
        case 7:
            cout<<"Enter stack size: ";
            cin>>n;
            tempStack = new stack(n);
            cout<<"Enter elements pushed onto stack: ";
            for(int i = 0; i<n; i++)
            {
                cin>>x;
                tempStack->push(x);
            }
            cout<<"Similarity: "<<mainStack.similarity(*tempStack)<<endl;
            delete tempStack;
            break;
        case 8:
            cout<<"Popping: ";
            working = false;
            while(mainStack.size()!= 0) cout<<mainStack.pop()<<" ";
            break;
        default:
            cout<<"Enter a valid command!"<<endl;
            break;
        }
        cout<<endl;
    }
    return 0;
}
