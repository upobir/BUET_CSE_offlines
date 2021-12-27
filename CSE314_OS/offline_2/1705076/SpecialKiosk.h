#ifndef SPECIAL_KIOSK_H
#define SPECIAL_KIOSK_H

#include "Passenger.h"
#include "semaphore.h"

typedef struct {
    sem_t special_kiosk_semaphore;
    int delay;
} SpecialKiosk;

void specialkiosk_construct(SpecialKiosk* kiosk, int delay);

void specialkiosk_access(SpecialKiosk* kiosk, Passenger* passenger);

void specialkiosk_checkin(SpecialKiosk* kiosk, Passenger* passenger);

void specialkiosk_release(SpecialKiosk* kiosk);

void specialkiosk_destroy(SpecialKiosk* kiosk);

#endif