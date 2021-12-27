#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include "Kiosks.h"
#include "SecurityCheck.h"
#include "BoardingGate.h"
#include "SpecialKiosk.h"
#include "VIPChannel.h"

extern FILE* out_file;
extern Kiosks kiosks;
extern SecurityCheck securtiyCheck;
extern BoardingGate boardingGate;
extern SpecialKiosk specialKiosk;
extern VIPChannel vipChannel;

int getTime();

#endif