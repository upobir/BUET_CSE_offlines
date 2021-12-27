#ifndef SECURITY_CHECKH_H
#define SECURITY_CHECKH_H

#include "Passenger.h"
#include "semaphore.h"

typedef struct {
    int beltCount;
    int beltCapacity;
    int delay;
    sem_t* capacity_semaphores;
} SecurityCheck;

void securitycheck_construct(SecurityCheck* securtiyCheck, int beltCount, int beltCapacity, int delay);

void secritycheck_access_belt(SecurityCheck* securtiyCheck, int beltNum, Passenger* passenger);

void securtiycheck_release_belt(SecurityCheck* securtiyCheck, int beltNum);

void secritycheck_checkin(SecurityCheck* securtiyCheck, int beltNum, Passenger* passenger);

void securitycheck_destroy(SecurityCheck* securityCheck);

#endif