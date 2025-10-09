#include "api.h"
#include "sdk.c"

// TODO NOTE: THIS CURRENTLY DOESNT WORK

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        // put your init code here
        
    }
    // put your loop code here
    unsigned short oid = getOid();

    if (oid != 0) {
        api->play_chomp_voice(3);
    }
    // todo: check if sounds in gme file are encrypted
}