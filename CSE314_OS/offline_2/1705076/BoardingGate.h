#pragma once

#include "Passenger.h"
#include "semaphore.h"

class BoardingGate {
public:
    int delay;
    sem_t boarding_semaphore;

    BoardingGate() = default;

    void init(int delay);

    void access(Passenger& passenger);

    void board(Passenger& passenger);

    void release();

    ~BoardingGate();
};