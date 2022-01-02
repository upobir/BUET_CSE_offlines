#include "BoardingGate.h"

void BoardingGate::init(int _delay) {
    delay = _delay;
    sem_init(&boarding_semaphore, 0, 1);
}

void BoardingGate::access(Passenger& passenger) {
    sem_wait(&boarding_semaphore);
}

void BoardingGate::board(Passenger& passenger) {
    sleep(delay);
}

void BoardingGate::release() {
    sem_post(&boarding_semaphore);
}

BoardingGate::~BoardingGate() {

}