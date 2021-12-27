#ifndef VIP_CHANNEL_H
#define VIP_CHANNEL_H

#include "Passenger.h"
#include <stdbool.h>

typedef struct {
    int delay;
    int onL2R;
    int onR2L;
    sem_t L2Rblock;
    sem_t R2Lblock;
    sem_t L2Rmutex;
    sem_t R2Lmutex;
} VIPChannel;

void vipchannel_construct(VIPChannel* channel, int delay);

void vipchannel_access(VIPChannel* channel, bool leftToRight, Passenger* passenger);

void vipchannel_use(VIPChannel* channel, bool leftToRight, Passenger* passenger);

void vipchannel_release(VIPChannel* channel, bool leftToRight);

void vipchannel_destroy(VIPChannel* channel);

#endif