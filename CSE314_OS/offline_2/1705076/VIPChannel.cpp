#include "VIPChannel.h"

void VIPChannel::init(int _delay) {
    delay = _delay;
    onL2R = 0;
    onR2L = 0;
    sem_init(&R2Lblock, 0, 1);
    sem_init(&L2Rblock, 0, 1);
    sem_init(&L2Rmutex, 0, 1);
    sem_init(&R2Lmutex, 0, 1);
}

void VIPChannel::access(bool leftToRight, Passenger& passenger) {
    if(leftToRight){
        sem_wait(&L2Rmutex);
        if(onL2R == 0)
            sem_wait(&R2Lblock);
        onL2R++;
        sem_post(&L2Rmutex);

        sem_wait(&L2Rblock);
        sem_post(&L2Rblock);
    }
    else{
        sem_wait(&R2Lblock);
        sem_post(&R2Lblock);

        sem_wait(&R2Lmutex);
        if(onR2L == 0)
            sem_wait(&L2Rblock);
        onR2L++;
        sem_post(&R2Lmutex);
    }
}

void VIPChannel::use(bool leftToRight, Passenger& passenger) {
    sleep(delay);
}

void VIPChannel::release(bool leftToRight) {
    if(leftToRight){
        sem_wait(&L2Rmutex);
        onL2R--;
        if(onL2R == 0)
            sem_post(&R2Lblock);
        sem_post(&L2Rmutex);
    }
    else{
        sem_wait(&R2Lmutex);
        onR2L--;
        if(onR2L == 0)
            sem_post(&L2Rblock);
        sem_post(&R2Lmutex);
    }
}

VIPChannel::~VIPChannel() {

}