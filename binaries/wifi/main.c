#include "api.h"
#include "sdk.c"

int (*fetch)()       = (void*)0x008af937;
int (*csv)()       = (void*)0x008b260d;

// WARNING:
// This currently does NOT work

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        initPlaySoundFifo();
        // put your init code here
        // fetch(L"https://example.com", 9, 0, 0, 0);
        // seek(f, 0, 2); // go to end of file
        csv();
    }
    // put your loop code here
    short oid = getOid();
    if (oid == 4906) {
        playSound(22);
        csv();
    }

    handlePlaySound();
}