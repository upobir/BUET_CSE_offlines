#include "Passenger.h"
#include "main.h"


void* passenger_run(void* passenger_v){
    Passenger* passenger = (Passenger*) passenger_v;

    passenger_kiosk_checkin(passenger);

    if(passenger->vip)
        passenger_use_vipchannel(passenger, true);
    else
        passenger_securitycheck(passenger);

    if(rand() % PASSENGER_PER_PASS_LOST == 0){
        passenger_lose_pass(passenger);
    }

    passenger_board(passenger);
}


void passenger_kiosk_checkin(Passenger* passenger) {
    int kiosk = kiosks_get_kiosk(&kiosks, passenger);

    fprintf(out_file, "Passenger %d%s waiting at kiosk %d at time %d\n", passenger->id, passenger->vip? " (VIP)" : "", kiosk, getTime());

    kiosks_checkin(&kiosks, kiosk, passenger);

    fprintf(out_file, "Passenger %d%s has got his boarding pass at time %d\n", passenger->id, passenger->vip? " (VIP)" : "", getTime());

    kiosks_put_kiosk(&kiosks, kiosk);
}


void passenger_securitycheck(Passenger* passenger) {
    int beltNum = rand() % securtiyCheck.beltCount + 1;

    secritycheck_access_belt(&securtiyCheck, beltNum, passenger);

    fprintf(out_file, "Passenger %d%s has started waiting for security check in belt %d at %d\n", passenger->id, passenger->vip? " (VIP)" : "", beltNum, getTime());

    secritycheck_checkin(&securtiyCheck, beltNum, passenger);

    fprintf(out_file, "Passenger %d%s has crossed the security check at %d\n", passenger->id, passenger->vip? " (VIP)" : "", getTime());

    securtiycheck_release_belt(&securtiyCheck, beltNum);
}


void passenger_board(Passenger* passenger) {

    boarding_access(&boardingGate, passenger);

    fprintf(out_file, "Passenger %d%s waiting at boarding gate at time %d\n", passenger->id, passenger->vip? " (VIP)" : "", getTime());

    boarding_board(&boardingGate, passenger);

    fprintf(out_file, "Passenger %d%s has boarded the plane at time %d\n", passenger->id, passenger->vip? " (VIP)" : "", getTime());

    boarding_release(&boardingGate);
}


void passenger_lose_pass(Passenger* passenger) {
    fprintf(out_file, "Passenger %d%s has lost their pass\n", passenger->id, passenger->vip? " (VIP)" : "");

    passenger_use_vipchannel(passenger, false);

    passenger_specialkiosk_checkin(passenger);

    passenger_use_vipchannel(passenger, true);
}


void passenger_specialkiosk_checkin(Passenger* passenger) {
    specialkiosk_access(&specialKiosk, passenger);

    fprintf(out_file, "Passenger %d%s waiting at special kiosk at time %d\n", passenger->id, passenger->vip? " (VIP)" : "", getTime());

    specialkiosk_checkin(&specialKiosk, passenger);

    fprintf(out_file, "Passenger %d%s has got his new boarding pass at time %d\n", passenger->id, passenger->vip? " (VIP)" : "", getTime());

    specialkiosk_release(&specialKiosk);
}


void passenger_use_vipchannel(Passenger* passenger, bool leftToRight) {
    vipchannel_access(&vipChannel, leftToRight, passenger);

    fprintf(out_file, "Passenger %d%s got on vip channel at %s at time %d\n", passenger->id, passenger->vip? " (VIP)" : "", leftToRight? "left" : "right", getTime());

    vipchannel_use(&vipChannel, leftToRight, passenger);

    fprintf(out_file, "Passenger %d%s got off vip channel at %s at time %d\n", passenger->id, passenger->vip? " (VIP)" : "", leftToRight? "right" : "left", getTime());

    vipchannel_release(&vipChannel, leftToRight);
}


void passenger_construct_run(Passenger* passenger, int id){
    passenger->id = id;
    passenger->vip = (rand() % PASSENGER_PER_VIP == 0);
    pthread_create(&passenger->thread, NULL, passenger_run, passenger);
}


void passenger_destroy(Passenger* passenger){
    pthread_join(passenger->thread, NULL);
}