#include "api.h"
#include "sdk.c"

// this only works on one firmware (2N-MT), todo: read firmware version and add other functions
void (*internal_play_sound)(int filehandle, int offset, int size, int filetype) = (void *)0x080ab6ac;
int (*internal_get_filesize)(int filehandle) = (void *)0x080ad530;

int file;
int filesize;

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        // put your init code here
        file = api->open(L"B:/TestTone/Test1KHz.wav", 0, 0);
        filesize = internal_get_filesize(file);

        // this does work, but only if there is no audio playing
        internal_play_sound(file, 0, filesize, 3);

        // this doesnt work
        // api->last_thing_that_gets_called_to_play_sound(file, 0, filesize);
    }
    // put your loop code here

}