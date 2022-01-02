#pragma once

#include "Passenger.h"
#include <stdbool.h>

class VIPChannel {
public:
    int delay;
    int onL2R;
    int onR2L;
    sem_t L2Rblock;
    sem_t R2Lblock;
    sem_t L2Rmutex;
    sem_t R2Lmutex;

    VIPChannel() = default;

    void init(int delay);

    void access(bool leftToRight, Passenger& passenger);

    void use(bool leftToRight, Passenger& passenger);

    void release(bool leftToRight);

    ~VIPChannel();

};