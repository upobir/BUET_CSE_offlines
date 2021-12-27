#include "SecurityCheck.h"
#include <stdlib.h>

void securitycheck_construct(SecurityCheck* securtiyCheck, int beltCount, int beltCapacity, int delay) {
    securtiyCheck->beltCount = beltCount;
    securtiyCheck->beltCapacity = beltCapacity;
    securtiyCheck->delay = delay;
    securtiyCheck->capacity_semaphores = malloc(sizeof(sem_t) * beltCount);

    for(int i = 0; i<beltCount; i++)
        sem_init(&securtiyCheck->capacity_semaphores[i], 0, beltCapacity);
}


void secritycheck_access_belt(SecurityCheck* securtiyCheck, int beltNum, Passenger* passenger) {
    sem_wait(&securtiyCheck->capacity_semaphores[beltNum-1]);
}


void securtiycheck_release_belt(SecurityCheck* securityCheck, int beltNum) {
    sem_post(&securityCheck->capacity_semaphores[beltNum-1]);
}


void secritycheck_checkin(SecurityCheck* securtiyCheck, int beltNum, Passenger* passenger) {
    sleep(securtiyCheck->delay);
}


void securitycheck_destroy(SecurityCheck* securityCheck) {
    free(securityCheck->capacity_semaphores);
}