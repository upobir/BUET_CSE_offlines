#include "Kiosks.h"

#include <stdlib.h>


void Kiosks::init(int size, int _delay) {
    free_kiosks = (int*) malloc(sizeof(int) * size);
    for(int pos = size-1, i = 1; pos >= 0; pos--, i++){
        free_kiosks[pos] = i;
    }
    free_count = size;
    delay = _delay;
    sem_init(&free_semaphore, 0, size);
    sem_init(&kiosk_acquire_mutex, 0, 1);
}

int Kiosks::getKiosk(Passenger& passenger) {
    int kiosk = -1;
    sem_wait(&free_semaphore);

    sem_wait(&kiosk_acquire_mutex);
    kiosk =  free_kiosks[free_count-1];
    free_count--;
    sem_post(&kiosk_acquire_mutex);

    return kiosk;
}

void Kiosks::putKiosk(int kiosk){
    sem_wait(&kiosk_acquire_mutex);
    free_count++;
    free_kiosks[free_count-1] = kiosk;
    sem_post(&kiosk_acquire_mutex);

    sem_post(&free_semaphore);
}

void Kiosks::checkin(int kiosk, Passenger& passenger) {
    sleep(delay);
}

Kiosks::~Kiosks() {
    sem_destroy(&free_semaphore);
    sem_destroy(&kiosk_acquire_mutex);
    free(free_kiosks);
}