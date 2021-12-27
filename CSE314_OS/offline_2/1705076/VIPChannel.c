#include "VIPChannel.h"

void vipchannel_construct(VIPChannel* channel, int delay) {
    channel->delay = delay;
    channel->onL2R = 0;
    channel->onR2L = 0;
    sem_init(&channel->R2Lblock, 0, 1);
    sem_init(&channel->L2Rblock, 0, 1);
    sem_init(&channel->L2Rmutex, 0, 1);
    sem_init(&channel->R2Lmutex, 0, 1);
}

void vipchannel_access(VIPChannel* channel, bool leftToRight, Passenger* passenger) {
    if(leftToRight){
        sem_wait(&channel->L2Rmutex);
        if(channel->onL2R == 0)
            sem_wait(&channel->R2Lblock);
        channel->onL2R++;
        sem_post(&channel->L2Rmutex);

        sem_wait(&channel->L2Rblock);
        sem_post(&channel->L2Rblock);
    }
    else{
        sem_wait(&channel->R2Lblock);
        sem_post(&channel->R2Lblock);

        sem_wait(&channel->R2Lmutex);
        if(channel->onR2L == 0)
            sem_wait(&channel->L2Rblock);
        channel->onR2L++;
        sem_post(&channel->R2Lmutex);
    }
}

void vipchannel_use(VIPChannel* channel, bool leftToRight, Passenger* passenger) {
    sleep(channel->delay);
}

void vipchannel_release(VIPChannel* channel, bool leftToRight) {
    if(leftToRight){
        sem_wait(&channel->L2Rmutex);
        channel->onL2R--;
        if(channel->onL2R == 0)
            sem_post(&channel->R2Lblock);
        sem_post(&channel->L2Rmutex);
    }
    else{
        sem_wait(&channel->R2Lmutex);
        channel->onR2L--;
        if(channel->onR2L == 0)
            sem_post(&channel->L2Rblock);
        sem_post(&channel->R2Lmutex);
    }
}

void vipchannel_destroy(VIPChannel* channel) {

}