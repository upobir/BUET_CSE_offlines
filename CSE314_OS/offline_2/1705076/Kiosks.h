#ifndef KIOSKS_H
#define KIOSKS_H

#include <semaphore.h>
#include "Passenger.h"

typedef struct {
    int* free_kiosks;
    int free_count;
    int delay;
    sem_t free_semaphore;
    sem_t kiosk_acquire_mutex;
} Kiosks;

void kiosks_construct(Kiosks* kiosks, int size, int delay);

int kiosks_get_kiosk(Kiosks* kiosks, Passenger* passenger);

void kiosks_put_kiosk(Kiosks* kiosks, int kiosk);

void kiosks_checkin(Kiosks* kiosks, int kiosk, Passenger* passenger);

void kiosks_destroy(Kiosks* kiosks);

#endif