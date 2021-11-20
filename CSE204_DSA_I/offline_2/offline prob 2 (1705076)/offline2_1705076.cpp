#include<iostream>
#include<random>
#include<windows.h>
#include<iomanip>

using namespace std;
///using this instead of rand(), as rand() has range upto only 2^16
mt19937 rng(time(0));

void RandomGenerate(char *arr, int n);
char RandomChar();
void MergeSort(int *arr, int n);
void PowerSet(char *arr, boolean *taken, int st, int en, boolean print);
LARGE_INTEGER GetTime();
long double TimeDiff(LARGE_INTEGER t1, LARGE_INTEGER t2);

int main()
{
    LARGE_INTEGER t1, t2;
    int n = 5;
    while(1)
    {
        /*cout<<"Enter R/M for random or manual (X to quit): ";
        char c;
        cin>>c;
        if(c == 'X') break;

        cout<<"Enter size: ";
        int n;
        cin>>n;*/
        char c = 'R';
        cout<<"n = "<<n<<endl;

        char *arr = new char[n+1];
        boolean *taken = new boolean[n];

        if(c == 'R')
        {
            RandomGenerate(arr, n);
            arr[n] = 0;
        }
        else
        {
            cout<<"Enter Elements (as a string): ";
            cin>>arr;
        }
        for(int i = 0; i<n; i++) taken[i] = false;

        char p = 'N';
        /*cout<<"Should subsets be printed? (Y/N): ";
        cin>>p;*/

        cout<<"==Power Set Generation=="<<endl;
        t1 = GetTime();
        PowerSet(arr, taken, 0, n, (p == 'Y'));
        t2 = GetTime();
        cout<<setprecision(10)<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
        cout<<endl;

        delete[] arr;
        delete[] taken;

        n++;
        if(n == 31) break;
    }
    return 0;
}


///generate random multiset
void RandomGenerate(char *arr, int n)
{
    for(int i = 0; i<n; i++)
        arr[i] = RandomChar();
}

///generate a random char between 0-9 a-z A-Z
char RandomChar()
{
    int r = rng()%62;
    if(r < 10) return '0' + r;
    else if(r < 36) return 'a' + r-10;
    else return 'A' + r-36;
}

///sorting array
void MergeSort(char *arr, int n)
{
    if(n == 1) return;

    MergeSort(arr, n/2);
    MergeSort(arr+n/2, n-n/2);
    char *tmp = new char[n];
    for(int i = 0, j = n/2, k = 0; k<n; k++)
        if(j == n || (i < n/2 && arr[i] < arr[j])) tmp[k] = arr[i++];
        else tmp[k] = arr[j++];
    for(int i = 0; i<n; i++) arr[i] = tmp[i];
    delete[] tmp;
    return;
}

void PowerSet(char *arr, boolean *taken, int st, int en, boolean print)
{
    if(st == en)
    {
        if(print)
        {
            for(int i = 0; i<en; i++)
                if(taken[i]) cout<<arr[i];
            cout<<"\n";
        }
        return;
    }
    int nxt = st;
    while(nxt < en && arr[nxt] == arr[st]) nxt++;

    PowerSet(arr, taken, nxt, en, print);
    for(int i = st; i<nxt; i++)
    {
        taken[i] = true;
        PowerSet(arr, taken, nxt, en, print);
    }
    for(int i = st; i<nxt; i++) taken[i] = false;
    return;
}

///get a windows performance time stamp
LARGE_INTEGER GetTime()
{
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t;
}

///get time difference of two stamps
long double TimeDiff(LARGE_INTEGER t1, LARGE_INTEGER t2)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return ((long double)(t2.QuadPart-t1.QuadPart)/(1e-6*(long double)frequency.QuadPart));
}
