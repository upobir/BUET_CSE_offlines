#include "SpecialKiosk.h"


void SpecialKiosk::init(int _delay) {
    delay = _delay;
    sem_init(&special_kiosk_semaphore, 0, 1);
}

void SpecialKiosk::access(Passenger& passenger) {
    sem_wait(&special_kiosk_semaphore);
}

void SpecialKiosk::checkin(Passenger& passenger) {
    sleep(delay);
}

void SpecialKiosk::release() {
    sem_post(&special_kiosk_semaphore);
}

SpecialKiosk::~SpecialKiosk() {

}