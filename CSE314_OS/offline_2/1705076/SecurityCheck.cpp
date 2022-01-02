#include "SecurityCheck.h"
#include <stdlib.h>


void SecurityCheck::init(int _beltCount, int _beltCapacity, int _delay) {
    beltCount = _beltCount;
    beltCapacity = _beltCapacity;
    delay = _delay;
    capacity_semaphores = (sem_t*) malloc(sizeof(sem_t) * beltCount);

    for(int i = 0; i<beltCount; i++)
        sem_init(&capacity_semaphores[i], 0, beltCapacity);
}

void SecurityCheck::accessBelt(int beltNum, Passenger& passenger) {
    sem_wait(&capacity_semaphores[beltNum-1]);
}

void SecurityCheck::releaseBelt(int beltNum) {
    sem_post(&capacity_semaphores[beltNum-1]);
}

void SecurityCheck::checkin(int beltNum, Passenger& passenger) {
    sleep(delay);
}

SecurityCheck::~SecurityCheck() {
    free(capacity_semaphores);
}