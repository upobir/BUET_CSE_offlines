#include <cstdio>
#include <iostream>
#include<random>
#include<windows.h>
#include<iomanip>

using namespace std;
///using this instead of rand(), as rand() has range upto only 2^16
mt19937 rng(time(0));


void QuickSort(int *arr, int n);
void MergeSort(int *arr, int n);
void GenerateAverageCase(int *arr, int n);
void GenerateBestCase(int *arr, int n);
void GenerateWorstCase(int *arr, int n);
LARGE_INTEGER GetTime();
long double TimeDiff(LARGE_INTEGER t1, LARGE_INTEGER t2);

int main()
{
    int *arr = NULL;
    int *arr2 = NULL;

    LARGE_INTEGER t1, t2;
    for(int n = 10, j = 1; n<50000; n *= (j&1)? 2 : 5){
        j++;
        arr = new int[n];
        arr2 = new int[n];
        cout<<"n = "<<n<<"\n";

        long double mergetime = 0;
        long double quicktime = 0;
        for(int k = 0; k<5; k++){
            GenerateBestCase(arr, n);
            memcpy(arr2, arr, n*sizeof(int));

            t1 = GetTime();
            MergeSort(arr, n);
            t2 = GetTime();
            mergetime += TimeDiff(t1, t2);

            t1 = GetTime();
            QuickSort(arr2, n);
            t2 = GetTime();
            quicktime += TimeDiff(t1, t2);
        }

        cout<<"\nBest Case:\n";
        cout<<"MergeSort: "<<mergetime/5.0<<" microseconds\n";
        cout<<"QuickSort: "<<quicktime/5.0<<" microseconds\n";

        mergetime = 0;
        quicktime = 0;

        for(int k = 0; k<5; k++){
            GenerateWorstCase(arr, n);
            memcpy(arr2, arr, n*sizeof(int));

            t1 = GetTime();
            MergeSort(arr, n);
            t2 = GetTime();
            mergetime += TimeDiff(t1, t2);

            t1 = GetTime();
            QuickSort(arr2, n);
            t2 = GetTime();
            quicktime += TimeDiff(t1, t2);
        }

        cout<<"\nWorst Case:\n";
        cout<<"MergeSort: "<<mergetime/5.0<<" microseconds\n";
        cout<<"QuickSort: "<<quicktime/5.0<<" microseconds\n";
//
        mergetime = 0;
        quicktime = 0;

        for(int k = 0; k<5; k++){
            GenerateAverageCase(arr, n);
            memcpy(arr2, arr, n*sizeof(int));

            t1 = GetTime();
            MergeSort(arr, n);
            t2 = GetTime();
            mergetime += TimeDiff(t1, t2);

            t1 = GetTime();
            QuickSort(arr2, n);
            t2 = GetTime();
            quicktime += TimeDiff(t1, t2);
        }

        cout<<"\nAverage Case:\n";
        cout<<"MergeSort: "<<mergetime/5.0<<" microseconds\n";
        cout<<"QuickSort: "<<quicktime/5.0<<" microseconds\n";
        cout<<"\n\n\n";
        delete[] arr;
        delete[] arr2;
    }
}

#define MAX 10000000

void GenerateAverageCase(int *arr, int n){
    for(int i = 0; i<n; i++) arr[i] = rng()%MAX;
}

void GenerateWorstCase(int *arr, int n){
    arr[0] = (rng()%MAX);
    for(int i = 1; i<n; i++) arr[i] = arr[i-1] - rng()%200+1;
}

void GenerateBestCase(int *arr, int n){
    arr[0] = (rng()%MAX);
    for(int i = 1; i<n; i++) arr[i] = arr[i-1] + rng()%200+1;
}

///arr = begin pointer, n = size
void MergeSort(int *arr, int n)
{
    if(n <= 1) return;

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

///arr = begin pointer, n = size
void QuickSort(int *arr, int n){
    if(n <= 1) return;
    int mid = 0, tmp;
    for(int i = 1; i<n; i++){
        if(arr[i] <= arr[0]) {
            mid++;
            if(i != mid){
                tmp = arr[i];
                arr[i] = arr[mid];
                arr[mid] = tmp;
            }
        }
    }

    tmp = arr[0];
    arr[0] = arr[mid];
    arr[mid] = tmp;

//    for(int i = 0; i<n; i++) cout<<arr[i]<<" ";
//    cout<<"\n";

    QuickSort(arr, mid);
    QuickSort(arr+mid+1, n - mid - 1);
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
