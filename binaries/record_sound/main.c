#include "api.h"
#include "sdk.c"

// this binary allows you to use the create function of your pen
// usage:
// set the 5. register to the recording_id
// set the 6. register to 0 to record or to 1 to playback
// call this game


void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;

        // todo: check create via .tiptoi.log

        int product_id;
        api->seek(*api->p_filehandle_current_gme, 0x14, 0);
        api->read(*api->p_filehandle_current_gme, &product_id, 4);
        api->create_recoring_folder(product_id);

        if (api->gme_registers[6] == 0) {
            // record
            api->record_sound(api->gme_registers[5], 10);
        } else if (api->gme_registers[6] == 1) {
            // play
            api->play_recording(api->gme_registers[5]);
        }

        api->gme_registers[0] = 1;
    }
}