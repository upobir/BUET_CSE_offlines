#include "BoardingGate.h"

void boarding_constuct(BoardingGate* boardingGate, int delay) {
    boardingGate->delay = delay;
    sem_init(&boardingGate->boarding_semaphore, 0, 1);
}

void boarding_access(BoardingGate* boardingGate, Passenger* passenger) {
    sem_wait(&boardingGate->boarding_semaphore);
}

void boarding_board(BoardingGate* boardingGate, Passenger* passenger) {
    sleep(boardingGate->delay);
}

void boarding_release(BoardingGate* boardingGate) {
    sem_post(&boardingGate->boarding_semaphore);
}

void boarding_destroy(BoardingGate* boardingGate) {

}