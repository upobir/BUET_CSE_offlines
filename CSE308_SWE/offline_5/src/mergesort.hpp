#pragma once

#include <vector>

template<typename T>
void mergeSortRange(std::vector<T>& input, int start, int end){
    if(end-start <= 1){
        return;
    }

    int mid = (start+end)/2;
    mergeSortRange(input, start, mid);
    mergeSortRange(input, mid, end);

    std::vector<T> tmp(end-start);
    for(int i = start, j = mid, k = 0; i<mid || j<end; ++k){
        if(j == end || (i < mid && input[j] > input[i]))
            tmp[k] = input[i++];
        else
            tmp[k] = input[j++];
    }

    for(int i = start, k = 0; i<end; ++i, ++k)
        input[i] = tmp[k];
        
    return;
}

template<typename T>
void mergeSort(std::vector<T>& input){
    mergeSortRange(input, 0, input.size());
}

