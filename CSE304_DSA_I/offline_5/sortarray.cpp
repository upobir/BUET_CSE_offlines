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
    int choice, n;
    int *arr = NULL;
    LARGE_INTEGER t1, t2;

    while(1)
    {
        printf("0. Enter Manual Array\n");
        printf("1. Generate average case\n");
        printf("2. Generate best case\n");
        printf("3. Generate worst case\n");
        printf("4. Apply Merge sort\n");
        printf("5. Apply Quicksort\n");
        printf("6. Print array\n");
        printf("> ");
        scanf("%d", &choice);

        switch(choice)
        {
        case 0:
            printf("Number of elements: ");
            scanf("%d", &n);
            if(arr) delete[] arr;
            arr = new int[n];
            for(int i = 0; i<n; i++) scanf("%d", &arr[i]);
            break;
        case 1:
            printf("Number of elements: ");
            scanf("%d", &n);
            if(arr) delete[] arr;
            arr = new int[n];
            GenerateAverageCase(arr, n);
            break;

        case 2:
            printf("Number of elements: ");
            scanf("%d", &n);
            if(arr) delete[] arr;
            arr = new int[n];
            GenerateBestCase(arr, n);
            break;

        case 3:
            printf("Number of elements: ");
            scanf("%d", &n);
            if(arr) delete[] arr;
            arr = new int[n];
            GenerateWorstCase(arr, n);
            break;

        case 4:
            printf("Applying merge sort\n");
            t1 = GetTime();
            MergeSort(arr, n);
            t2 = GetTime();
            printf("Time taken to finish: %.6f microseconds\n ", double(TimeDiff(t1, t2)));
            break;

        case 5:
            printf("Applying quick sort\n");
            t1 = GetTime();
            QuickSort(arr, n);
            t2 = GetTime();
            printf("Time taken to finish: %.6f microseconds\n", double(TimeDiff(t1, t2)));
            break;

        case 6:
            printf("Array:\n");
            for(int i = 0; i<n; i++) printf("%d ", arr[i]);
            printf("\n");
            break;
        }
    }
}

#define MAX 10000

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

///arr = begin pointer, n = size
void QuickSort(int *arr, int n){
    if(n <= 1) return;
    int mid = 0, tmp;
    for(int i = 1; i<n; i++){
        if(arr[i] <= arr[0]) {
            mid++;
            tmp = arr[i];
            arr[i] = arr[mid];
            arr[mid] = tmp;
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
