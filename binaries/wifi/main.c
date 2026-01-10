#include "api.h"
#include "sdk.c"

int (*memset)()      = (void*)0x0082a398;
int (*to_unicode)()      = (void*)0x00802038;
int (*fetch)()       = (void*)0x008af937;
int (*csv)()       = (void*)0x008b260d;

int buff = 0x008066a0;

// WARNING:
// This currently does NOT work

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        initPlaySoundFifo();
        memset(buff, 0x206);
        to_unicode(buff + 6, "http://ifconfig.me/all.json", 29);
        int ret = fetch(buff, 9, 0, 0);
        if (ret != 0) {
            api->play_chomp_voice(69); //zero
        } else {
            api->play_chomp_voice(70);
        }
    }

    short oid = getOid();
    if (oid == 4906) {
        playSound(22);
        
    }

    handlePlaySound();
}