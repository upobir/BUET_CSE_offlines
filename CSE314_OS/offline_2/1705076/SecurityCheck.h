#pragma once

#include "Passenger.h"
#include "semaphore.h"

class SecurityCheck {
public:
    int beltCount;
    int beltCapacity;
    int delay;
    sem_t* capacity_semaphores;

    SecurityCheck() = default;

    void init(int beltCount, int beltCapacity, int delay);

    void accessBelt(int beltNum, Passenger& passenger);

    void releaseBelt(int beltNum);

    void checkin(int beltNum, Passenger& passenger);

    ~SecurityCheck();

};