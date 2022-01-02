#pragma once

#include <semaphore.h>
#include "Passenger.h"

class Kiosks {
public:
    int* free_kiosks;
    int free_count;
    int delay;
    sem_t free_semaphore;
    sem_t kiosk_acquire_mutex;

    Kiosks() = default;

    void init(int size, int delay);

    int getKiosk(Passenger& passenger);

    void putKiosk(int kiosk);

    void checkin(int kiosk, Passenger& passenger);

    ~Kiosks();
};

