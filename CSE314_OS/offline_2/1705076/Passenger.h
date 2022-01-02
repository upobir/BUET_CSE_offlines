#pragma once

#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

#define PASSENGER_PER_VIP 5
#define PASSENGER_PER_PASS_LOST 3

class Passenger {
public:
    int id;
    pthread_t thread;
    bool vip;

    Passenger() = default;

    void init(int id);

    void kioskCheckin();

    void securityCheck();

    void board();

    void losePass();

    void specialKioskCheckin();

    void useVipChannel(bool leftToRight);

    ~Passenger();

    static void* run(void* passenger_v);
};