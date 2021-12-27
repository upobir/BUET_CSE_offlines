#include "SpecialKiosk.h"

void specialkiosk_construct(SpecialKiosk* kiosk, int delay) {
    kiosk->delay = delay;
    sem_init(&kiosk->special_kiosk_semaphore, 0, 1);
}

void specialkiosk_access(SpecialKiosk* kiosk, Passenger* passenger) {
    sem_wait(&kiosk->special_kiosk_semaphore);
}

void specialkiosk_checkin(SpecialKiosk* kiosk, Passenger* passenger) {
    sleep(kiosk->delay);
}

void specialkiosk_release(SpecialKiosk* kiosk) {
    sem_post(&kiosk->special_kiosk_semaphore);
}

void specialkiosk_destroy(SpecialKiosk* kiosk) {

}