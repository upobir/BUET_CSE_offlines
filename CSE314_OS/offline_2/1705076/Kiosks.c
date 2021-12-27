#include "Kiosks.h"

#include <stdlib.h>

void kiosks_construct(Kiosks* kiosks, int size, int delay) {
    kiosks->free_kiosks = (int*) malloc(sizeof(int) * size);
    for(int pos = size-1, i = 1; pos >= 0; pos--, i++){
        kiosks->free_kiosks[pos] = i;
    }
    kiosks->free_count = size;
    kiosks->delay = delay;
    sem_init(&kiosks->free_semaphore, 0, size);
    sem_init(&kiosks->kiosk_acquire_mutex, 0, 1);
}


void kiosks_destroy(Kiosks* kiosks) {
    sem_destroy(&kiosks->free_semaphore);
    sem_destroy(&kiosks->kiosk_acquire_mutex);
    free(kiosks->free_kiosks);
}


void kiosks_checkin(Kiosks* kiosks, int kiosk, Passenger* passenger) {
    sleep(kiosks->delay);
}


void kiosks_put_kiosk(Kiosks* kiosks, int kiosk) {
    sem_wait(&kiosks->kiosk_acquire_mutex);
    kiosks->free_count++;
    kiosks->free_kiosks[kiosks->free_count-1] = kiosk;
    sem_post(&kiosks->kiosk_acquire_mutex);

    sem_post(&kiosks->free_semaphore);
}


int kiosks_get_kiosk(Kiosks* kiosks, Passenger* passenger) {
    int kiosk = -1;
    sem_wait(&kiosks->free_semaphore);

    sem_wait(&kiosks->kiosk_acquire_mutex);
    kiosk =  kiosks->free_kiosks[kiosks->free_count-1];
    kiosks->free_count--;
    sem_post(&kiosks->kiosk_acquire_mutex);


    return kiosk;
}