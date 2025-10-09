#include "api.h"
#include "sdk.c"

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        initPlaySoundFifo();

        playSound(1);
        playSound(2);
        playSound(2);
        playSound(3);
    }
    // put your loop code here
    short read_oid = getOid();
    if (read_oid != 0) {
        if (read_oid == 4907) {
            playSound(4);
        } else if (read_oid == 4906) {
            playSound(5);
        } else {
            playSound(6);
        }
    }

    handlePlaySound();
}