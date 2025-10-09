#include "api.h"
#include "sdk.c"

// if you already wrote a game with gme-scripts, you can use this code to add a savegame mechanism
// set the 5. register to 0 to load the savestate or set it to 1 to save the savestate
// then call this game binary

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        // put your init code here
        int product_id;
        api->seek(*api->p_filehandle_current_gme, 0x14, 0);
        api->read(*api->p_filehandle_current_gme, &product_id, 4);

        unsigned short filename[] = L"B:/tt-homebrew0000.save";

        if (product_id > 9999) {
            product_id = 9999;
        }

        filename[14] = L'0' + ((product_id / 1000) % 10);
        filename[15] = L'0' + ((product_id / 100) % 10);
        filename[16] = L'0' + ((product_id / 10) % 10);
        filename[17] = L'0' + ((product_id) % 10);

        if (api->gme_registers[5] == 0) {
            // load game
            int file = api->open(filename, 0, 0);
            api->read(file, api->gme_registers, *api->gme_register_count * 2);  // oh man, dass muss definitiv getestet werden
            api->close(file);
            api->play_chomp_voice(3);  // play a sound to know that loading worked
        } else if (api->gme_registers[5] == 1) {
            // save game
            int file = api->open(filename, 1, 1);
            api->write(file, api->gme_registers, *api->gme_register_count * 2);
            api->close(file);
            api->play_chomp_voice(4);  // play a sound to know that saving worked
        }
        api->gme_registers[0] = 1;  // exit the game | this sets $0 to 1 which is the red eye, 2 is the blue speech bubble
    }
    // put your loop code here
}