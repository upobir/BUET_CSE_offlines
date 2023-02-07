#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>
#include "progress_bar.hpp"

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

const int iterations = 10000;

bool simulate(int candidateCnt, int sampleCnt, int standard);

int main(int argc, char** argv){

    if(argc > 1){
        rng = std::mt19937(std::stoi(argv[1]));
    }

    /// READ INPUTS

    int candidateCnt;
    int standard;
    std::string filename;

    std::cout << "Enter the number of candidates (n): ";
    std::cin >> candidateCnt;

    std::cout << "Enter the standard (s): ";
    std::cin >> standard;

    std::cout << "Enter the filename to save the results to: ";
    std::cin >> filename;

    std::cout << "Simulating " << iterations << " iterations for each sample size (m)..." << std::endl;

    /// SIMULATE

    std::vector<double> probabilities(candidateCnt);
    ProgressBar progressBar(50);
    int calls = 0;
    for(int sampleCnt = 0; sampleCnt < candidateCnt; sampleCnt++){
        int successCnt = 0;

        for(int i = 0; i < iterations; i++){
            if(simulate(candidateCnt, sampleCnt, standard)){
                successCnt++;
            }
            calls++;
            
        }
        progressBar.update((double) calls / (double) (candidateCnt * iterations));

        double probability = (double) successCnt / (double) iterations;
        probabilities[sampleCnt] = probability * 100.0;
    }

    int bestSampleCnt = (int) (max_element(probabilities.begin(), probabilities.end()) - probabilities.begin());

    /// REPORT

    std::cout << "The best sample size (m) is " << bestSampleCnt << " ensuring a probability of " << probabilities[bestSampleCnt] << "%" << std::endl;

    std::ofstream file(filename);
    for(int i = 0; i < candidateCnt; i++) {
        file << i << "," << probabilities[i] << std::endl;
    }
    file.close();

    return 0;
}

bool simulate(int candidateCnt, int sampleCnt, int standard){
    std::vector<int> candidates(candidateCnt);
    std::iota(candidates.begin(), candidates.end(), 1);
    std::shuffle(candidates.begin(), candidates.end(), rng);

    /// assuming lower value is better candidate

    int bestSample = candidateCnt + 1;
    if(sampleCnt > 0)
        bestSample = *std::min_element(candidates.begin(), candidates.begin() + sampleCnt);

    auto chosenIt = std::find_if(candidates.begin()+sampleCnt, candidates.end(), [&](int candidate){
        return candidate < bestSample;
    });

    if(chosenIt == candidates.end()){
        chosenIt--;
    }
    int chosen = *chosenIt;

    return chosen <= standard;
}