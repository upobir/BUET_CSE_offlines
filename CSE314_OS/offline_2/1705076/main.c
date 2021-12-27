#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "Passenger.h"
#include "main.h"


const int PASSENGER_COUNT = 10;

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

    kiosks_construct(&kiosks, M, w);
    securitycheck_construct(&securtiyCheck, N, P, x);
    boarding_constuct(&boardingGate, y);
    specialkiosk_construct(&specialKiosk, w);
    vipchannel_construct(&vipChannel, z);

    Passenger passengers[PASSENGER_COUNT];

    start = time(NULL);
    for(int i = 0; i<PASSENGER_COUNT; i++){
        passenger_construct_run(&passengers[i], i+1);
    }


    for(int i = 0; i<PASSENGER_COUNT; i++){
        passenger_destroy(&passengers[i]);
    }

    kiosks_destroy(&kiosks);
    securitycheck_destroy(&securtiyCheck);
    boarding_destroy(&boardingGate);
    specialkiosk_destroy(&specialKiosk);
    vipchannel_destroy(&vipChannel);

    fclose(out_file);

    return 0;
}