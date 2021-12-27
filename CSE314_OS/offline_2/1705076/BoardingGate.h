#ifndef BOARDING_GATE_H
#define BOARDING_GATE_H

#include "Passenger.h"
#include "semaphore.h"

typedef struct {
    int delay;
    sem_t boarding_semaphore;
} BoardingGate;

void boarding_constuct(BoardingGate* boardingGate, int delay);

void boarding_access(BoardingGate* boardingGate, Passenger* passenger);

void boarding_board(BoardingGate* boardingGate, Passenger* passenger);

void boarding_release(BoardingGate* boardingGate);

void boarding_destroy(BoardingGate* boardingGate);

#endif