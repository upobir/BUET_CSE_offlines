#pragma once

#include "Passenger.h"
#include "semaphore.h"

class SpecialKiosk {
public:
    sem_t special_kiosk_semaphore;
    int delay;

    SpecialKiosk() = default;

    void init(int delay);

    void access(Passenger& passenger);

    void checkin(Passenger& passenger);

    void release();

    ~SpecialKiosk();
};

