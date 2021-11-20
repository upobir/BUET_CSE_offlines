#include<iostream>
#include<random>
#include<chrono>
#include<windows.h>

using namespace std;
///using this instead of rand(), as rand() has range upto only 2^16
mt19937 rng(time(0));

void RandomAvarageGenerate(int *arr, int n);
void RandomBestGenerate(int *arr, int n);
void RandomWorstGenerate(int *arr, int n);
void GetArrayInput(int *arr, int n);
void SelectionSort(int *arr, int n);
void InsertionSort(int *arr, int n);
LARGE_INTEGER GetTime();
long double TimeDiff(LARGE_INTEGER t1, LARGE_INTEGER t2);

int main()
{
    LARGE_INTEGER t1, t2;
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
        int *arr2 = new int[n];

        if(c == 'R')
        {
            RandomAvarageGenerate(arr, n);
            memcpy(arr2, arr, sizeof(int)*n);

            cout<<"==Selection Sort (Avarage Case)=="<<endl;
            t1 = GetTime();
            SelectionSort(arr, n);
            t2 = GetTime();
            cout<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
            cout<<endl;
            cout<<"==Insertion Sort (Avarage Case)=="<<endl;
            t1 = GetTime();
            InsertionSort(arr2, n);
            t2 = GetTime();
            cout<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
            cout<<endl;

            RandomBestGenerate(arr, n);
            memcpy(arr2, arr, sizeof(int)*n);

            cout<<"==Selection Sort (Best Case)=="<<endl;
            t1 = GetTime();
            SelectionSort(arr, n);
            t2 = GetTime();
            cout<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
            cout<<endl;
            cout<<"==Insertion Sort (Best Case)=="<<endl;
            t1 = GetTime();
            InsertionSort(arr2, n);
            t2 = GetTime();
            cout<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
            cout<<endl;

            RandomWorstGenerate(arr, n);
            memcpy(arr2, arr, sizeof(int)*n);

            cout<<"==Selection Sort (Worst Case)=="<<endl;
            t1 = GetTime();
            SelectionSort(arr, n);
            t2 = GetTime();
            cout<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
            cout<<endl;
            cout<<"==Insertion Sort (Worst Case)=="<<endl;
            t1 = GetTime();
            InsertionSort(arr2, n);
            t2 = GetTime();
            cout<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
            cout<<endl;
        }
        else
        {
            cout<<"Enter Elements: ";
            GetArrayInput(arr, n);
            memcpy(arr2, arr, sizeof(int)*n);

            cout<<"==Selection Sort=="<<endl;
            t1 = GetTime();
            SelectionSort(arr, n);
            t2 = GetTime();
            cout<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
            cout<<endl;
            cout<<"==Insertion Sort=="<<endl;
            t1 = GetTime();
            InsertionSort(arr2, n);
            t2 = GetTime();
            cout<<"Time taken: "<<TimeDiff(t1, t2)<<" microsecends"<<endl;
            cout<<endl;

            cout<<"Sorted Elements: ";
            for(int i = 0; i<n; i++)
                cout<<arr[i]<<" ";
            cout<<endl;
        }

        delete[] arr;
        delete[] arr2;
    }
    return 0;
}



///generate random array and sort
void RandomAvarageGenerate(int *arr, int n)
{
    for(int i = 0; i<n; i++)
        arr[i] = rng();
}

///generate an array where elements are increasing
void RandomBestGenerate(int *arr, int n)
{
    arr[0] = rng();
    for(int i = 1; i<n; i++)
        arr[i] = arr[i-1] + rng()%5;
}

///generate an array where elements are decreasing
void RandomWorstGenerate(int *arr, int n)
{
    arr[0] = rng();
    for(int i = 1; i<n; i++)
        arr[i] = arr[i-1] - rng()%5;
}

///get array input from user
void GetArrayInput(int *arr, int n)
{
    for(int i = 0; i<n; i++)
        cin>>arr[i];
}

///sorts using selection sort
void SelectionSort(int *arr, int n)
{
    for(int i = 0; i<n; i++)
    {
        int minid = i;
        for(int j = i+1; j<n; j++)
            if(arr[minid] > arr[j]) minid = j;
        int tmp = arr[i];
        arr[i] = arr[minid];
        arr[minid] = tmp;
    }
    return;
}

///sorts using insertion sort
void InsertionSort(int *arr, int n)
{
    for(int i = 0; i<n; i++)
    {
        int x = arr[i];
        int j;
        for(j = i-1; j>=0; j--)
        {
            if(x >= arr[j])
                break;
            else
                arr[j+1] = arr[j];
        }
        arr[j+1] = x;
    }
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
