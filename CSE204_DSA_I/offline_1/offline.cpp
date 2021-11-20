#include<iostream>
#include<random>
#include<chrono>
#include<windows.h>

using namespace std;
///using this instead of rand(), as rand() has range upto only 2^16
mt19937 rng(time(0));

void RandomGenerate(int *arr, int n);
void GetArrayInput(int *arr, int n);
void MergeSort(int *arr, int n);
int LinearSearch(int *arr, int n, int val);
int BinarySearch(int *arr, int n, int val);

#define Time(t) QueryPerformanceCounter(&t)
#define TimeDiff(t1, t2) ((long double)(t2.QuadPart-t1.QuadPart)/(1e-6*(long double)frequency.QuadPart))

int main()
{
    LARGE_INTEGER frequency, t1, t2;
    QueryPerformanceFrequency(&frequency);

    while(1)
    {
        cout<<"Enter R/M for random or manual (X to quit): ";
        char c;
        cin>>c;
        if(c == 'X') break;

        cout<<"Enter size: ";
        int n;
        cin>>n;
        int *arr = new int[n];

        if(c == 'R') RandomGenerate(arr, n);
        else GetArrayInput(arr, n);
        MergeSort(arr, n);

        cout<<"Enter how many queries: ";
        int q;
        cin>>q;

        for(int i = 0; i<q; i++)
        {
            int val;
            if(c == 'R') val = rng();
            else cin>>val;

            int ret;

            cout<<"== Linear Search =="<<endl;
            Time(t1);
            ret = LinearSearch(arr, n, val);
            Time(t2);
            //cout<<"index in sorted array: "<<ret<<endl;
            cout<<"time: "<<TimeDiff(t1, t2)<<" microseconds"<<endl;

            cout<<"== Binary Search =="<<endl;
            Time(t1);
            ret = BinarySearch(arr, n, val);
            Time(t2);
            //cout<<"index in sorted array: "<<ret<<endl;
            cout<<"time: "<<TimeDiff(t1, t2)<<" microseconds"<<endl;

            cout<<endl;
        }

        delete[] arr;
    }
    return 0;
}



///generate random array and sort
void RandomGenerate(int *arr, int n)
{
    for(int i = 0; i<n; i++)
        arr[i] = rng();
}

///get array input from user
void GetArrayInput(int *arr, int n)
{
    for(int i = 0; i<n; i++)
        cin>>arr[i];
}

///arr = begin pointer, n = size
void MergeSort(int *arr, int n)
{
    if(n == 1) return;

    MergeSort(arr, n/2);
    MergeSort(arr+n/2, n-n/2);
    int *tmp = new int[n];
    for(int i = 0, j = n/2, k = 0; k<n; k++)
        if(j == n || (i < n/2 && arr[i] < arr[j])) tmp[k] = arr[i++];
        else tmp[k] = arr[j++];
    for(int i = 0; i<n; i++) arr[i] = tmp[i];
    delete[] tmp;
    return;
}

///seach via loop through
int LinearSearch(int *arr, int n, int val)
{
    for(int i = 0; i<n; i++)
        if(arr[i] == val) return i;
    return -1;
}

///binary search
int BinarySearch(int *arr, int n, int val)
{
    int lo = 0, hi = n-1;
    while(lo <= hi)
    {
        int mid = (lo + hi)/2;
        if(arr[mid] == val) return mid;
        else if(arr[mid] < val) lo = mid+1;
        else hi = mid-1;
    }
    return -1;
}
