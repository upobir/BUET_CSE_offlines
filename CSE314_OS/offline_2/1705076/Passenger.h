#ifndef PASSENGER_H
#define PASSENGER_H

#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

#define PASSENGER_PER_VIP 5
#define PASSENGER_PER_PASS_LOST 3

typedef struct{
    int id;
    pthread_t thread;
    bool vip;
}  Passenger;


void* passenger_run(void* passenger_v);

void passenger_construct_run(Passenger* passenger, int id);

void passenger_kiosk_checkin(Passenger* passenger);

void passenger_securitycheck(Passenger* passenger);

void passenger_board(Passenger* passenger);

void passenger_lose_pass(Passenger* passenger);

void passenger_specialkiosk_checkin(Passenger* passenger);

void passenger_use_vipchannel(Passenger* passenger, bool leftToRight);

void passenger_destroy(Passenger* passenger);

#endif