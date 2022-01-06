#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <random>

#include "Passenger.h"
#include "main.h"


const int PASSENGER_COUNT = 15;
const int INTERVAL_SECS = 20;

FILE* out_file;
Kiosks kiosks;
SecurityCheck securtiyCheck;
BoardingGate boardingGate;
SpecialKiosk specialKiosk;
VIPChannel vipChannel;

time_t start;
int getTime() {
    time_t now = time(NULL);
    return (now - start);
}


int main(){

    int M;  // number of kiosk
    int N;  // number of security belt
    int P;  // capacity of each belt
    int w;  // kiosk delay
    int x;  // security belt delay
    int y;  // boarding delay
    int z;  // vip channel delay

    scanf("%d %d %d", &M, &N, &P);
    scanf("%d %d %d %d", &w, &x, &y, &z);
    out_file = fopen("out.txt", "w");
    // out_file = stdout;

    srand(time(NULL));
    std::mt19937 rng(time(NULL));
    std::exponential_distribution<double> dist(PASSENGER_COUNT);

    kiosks.init(M, w);
    securtiyCheck.init(N, P, x);
    boardingGate.init(y);
    specialKiosk.init(w);
    vipChannel.init(z);

    Passenger passengers[PASSENGER_COUNT];

    start = time(NULL);
    for(int i = 0; i<PASSENGER_COUNT; i++){
        auto gap = dist(rng) * INTERVAL_SECS;
        timespec req = {.tv_sec = (int) gap, .tv_nsec = (int)(1e9 * (gap - (int) gap))}, rem;
        nanosleep(&req, &rem);
        passengers[i].init(i+1);
    }

    for(int i = 0; i<PASSENGER_COUNT; i++){
        pthread_join(passengers[i].thread, NULL);
    }

    fclose(out_file);

    return 0;
}