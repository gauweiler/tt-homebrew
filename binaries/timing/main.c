#include "api.h"
#include "sdk.c"
int i;
void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        // put your init code here
        i = 0;
        api->play_chomp_voice(3);
    }
    // put your loop code here
    i++;
    // this gets executet after 30 seconds
    if (i > 300) {
        if (api->is_audio_playing() == 0) {
            api->play_chomp_voice(4);
        }
    }
}