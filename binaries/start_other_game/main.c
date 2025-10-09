#include "api.h"
#include "sdk.c"


void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        initPlaySoundFifo();
        // put your init code here
        api->play_chomp_voice(3);
        api->fpAkOidPara[next_game] = 1; // game id 
        api->fpAkOidPara[start_next_game] = 1; 
        api->fpAkOidPara[First_time_exec] = 3;
    }
    // put your loop code here

    handlePlaySound();
}