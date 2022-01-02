#include "Passenger.h"
#include "main.h"


void Passenger::init(int _id) {
    id = _id;
    vip = (rand() % PASSENGER_PER_VIP == 0);
    pthread_create(&thread, nullptr, Passenger::run, this);
}

void Passenger::kioskCheckin() {
    int kiosk = kiosks.getKiosk(*this);

    fprintf(out_file, "Passenger %d%s waiting at kiosk %d at time %d\n", id, vip? " (VIP)" : "", kiosk, getTime());

    kiosks.checkin(kiosk, *this);

    fprintf(out_file, "Passenger %d%s has got his boarding pass at time %d\n", id, vip? " (VIP)" : "", getTime());

    kiosks.putKiosk(kiosk);
}

void Passenger::securityCheck() {
    int beltNum = rand() % securtiyCheck.beltCount + 1;

    securtiyCheck.accessBelt(beltNum, *this);

    fprintf(out_file, "Passenger %d%s has started waiting for security check in belt %d at %d\n", id, vip? " (VIP)" : "", beltNum, getTime());

    securtiyCheck.checkin(beltNum, *this);

    fprintf(out_file, "Passenger %d%s has crossed the security check at %d\n", id, vip? " (VIP)" : "", getTime());

    securtiyCheck.releaseBelt(beltNum);
}

void Passenger::board() {
    boardingGate.access(*this);

    fprintf(out_file, "Passenger %d%s waiting at boarding gate at time %d\n", id, vip? " (VIP)" : "", getTime());

    boardingGate.board(*this);

    fprintf(out_file, "Passenger %d%s has boarded the plane at time %d\n", id, vip? " (VIP)" : "", getTime());

    boardingGate.release();
}

void Passenger::losePass() {
    fprintf(out_file, "Passenger %d%s has lost their pass\n", id, vip? " (VIP)" : "");

    useVipChannel(false);

    specialKiosk.checkin(*this);

    useVipChannel(true);
}

void Passenger::specialKioskCheckin() {
    specialKiosk.access(*this);

    fprintf(out_file, "Passenger %d%s waiting at special kiosk at time %d\n", id, vip? " (VIP)" : "", getTime());

    specialKiosk.checkin(*this);

    fprintf(out_file, "Passenger %d%s has got his new boarding pass at time %d\n", id, vip? " (VIP)" : "", getTime());

    specialKiosk.release();
}

void Passenger::useVipChannel(bool leftToRight) {
    vipChannel.access(leftToRight, *this);

    fprintf(out_file, "Passenger %d%s got on vip channel at %s at time %d\n", id, vip? " (VIP)" : "", leftToRight? "left" : "right", getTime());

    vipChannel.use(leftToRight, *this);

    fprintf(out_file, "Passenger %d%s got off vip channel at %s at time %d\n", id, vip? " (VIP)" : "", leftToRight? "right" : "left", getTime());

    vipChannel.release(leftToRight);
}

Passenger::~Passenger() {
    pthread_join(thread, NULL);
}

void* Passenger::run(void* passenger_v) {
    Passenger* passenger = (Passenger*) passenger_v;

    passenger->kioskCheckin();

    if(passenger->vip)
        passenger->useVipChannel(true);
    else
        passenger->securityCheck();

    if(rand() % PASSENGER_PER_PASS_LOST == 0){
        passenger->losePass();
    }

    passenger->board();

    return nullptr;
}


