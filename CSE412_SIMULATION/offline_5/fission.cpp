#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "progress_bar.hpp"

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

const int iterations = 10000;

int simulate(int initialNeutrons, int generations, std::vector<double>& fissionProbs);
int randomPick(std::vector<double>& probs);

int main(int argc, char** argv){

    if(argc > 1){
        rng = std::mt19937(std::stoi(argv[1]));
    }
    
    int fissionMaxCnt = 3;
    // std::cout<<"Enter the maximum number of neutrons generated during fission: ";
    // std::cin>>fissionMaxCnt;

    std::vector<double> fissionProbs(fissionMaxCnt+1);
    // std::cout<<"Enter the probabilities of fission for each number of neutrons (will be normalized in program): " << std::endl;
    // for(int i=0; i<fissionMaxCnt+1; i++){
    //     std::cin>>fissionProbs[i];
    // }

    for (int i = 1; i < fissionMaxCnt+1; i++) {
        fissionProbs[i] = (0.2126) * pow(0.5893, i-1);
    }
    fissionProbs[0] = 1.0 - accumulate(fissionProbs.begin(), fissionProbs.end(), 0.0);
    if(fissionProbs[0] < 0.0){
        std::cout<<"Error: fission probabilities sum to greater than 1."<<std::endl;
        return 1;
    }

    double fissionProbSum = accumulate(fissionProbs.begin(), fissionProbs.end(), 0.0);
    if(fissionProbSum == 0.0){
        std::cout<<"Error: fission probabilities sum to zero."<<std::endl;
        return 1;
    }

    for(int i=0; i<fissionMaxCnt+1; i++){
        fissionProbs[i] /= fissionProbSum;
    }

    std::partial_sum(fissionProbs.begin(), fissionProbs.end(), fissionProbs.begin());

    int generations;
    std::cout<<"Enter the number of generations to simulate: ";
    std::cin>>generations;

    int maxNeutronCalc;
    std::cout<<"Enter the maximum number of neutrons to calculate probabilities: ";
    std::cin>>maxNeutronCalc;

    std::vector<double> neutronProbs(maxNeutronCalc+2);

    int initialNeutrons = 1;
    // ProgressBar progressBar(50);
    for(int i = 0; i<iterations; i++){
        int neutronCnt = simulate(initialNeutrons, generations, fissionProbs);
        if(neutronCnt <= maxNeutronCalc){
            neutronProbs[neutronCnt] += 1.0;
        }
        else {
            neutronProbs[maxNeutronCalc+1] += 1.0;
        }
        // progressBar.update((double)(i+1)/(double)iterations);
    }

    for(int i=0; i<maxNeutronCalc+2; i++){
        neutronProbs[i] /= iterations;
        neutronProbs[i] *= 100.0;
    }

    std::cout<<"Neutron probabilities: " << std::endl;
    for(int i=0; i<=maxNeutronCalc; i++){
        std::cout<<i<<"\t: "<<neutronProbs[i]<<" %"<<std::endl;
    }
    std::cout<<">"<<maxNeutronCalc<<"\t: "<<neutronProbs[maxNeutronCalc+1]<<" %"<<std::endl;

    return 0;
}

int randomPick(std::vector<double>& probs) {
    double u = std::uniform_real_distribution<double>(0.0, 1.0)(rng);
    if(probs.size() > 10) {
        return (int)(std::lower_bound(probs.begin(), probs.end(), u) - probs.begin());
    }
    else{
        return (int)(std::find_if(probs.begin(), probs.end(), [u](double p){return p >= u;}) - probs.begin());
    }
}

int simulate(int initialNeutrons, int generations, std::vector<double>& fissionProbs) {
    int currentNeutrons = initialNeutrons;
    for(int g = 1; g <= generations; g++){
        int newNeutrons = 0;
        for(int n = 0; n < currentNeutrons; n++){
            int fissionCnt = randomPick(fissionProbs);
            newNeutrons += fissionCnt;
        }
        currentNeutrons = newNeutrons;
    }
    return currentNeutrons;
}