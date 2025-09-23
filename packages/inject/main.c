#include "api.h"

// only 3L
// game 9 in den bergen | red 4906 | green 4907
// node build.js -i Wimmelbuch.gme  -o Wimmelbuchwraped.gme -n inject -b 9
// note: can only read oids from current book
// todo: multiple saves
// todo: overwrite api for print, etc

void (*orginal_binary)(system_api *) = (void *)0x00930004;
void (*play_sound)(unsigned int) = (void *)0x00930794;

void main(system_api *api) {
    // if (api->fpAkOidPara[First_time_exec] == 0) {
    // }

    if (api->fpAkOidPara[new_oid] == 1) {
        short read_oid = *(short *)&api->fpAkOidPara[oid_offset];
        if (read_oid == 4906) {
            api->fpAkOidPara[new_oid] = 0;
            api->play_chomp_voice(4);

            // create a savefile
            int file = api->open(L"B:/dump/wimmelgame3.save", 1, 1);
            api->write(file, (void *)SAVEDATA, 0xf000);
            api->close(file);
        }
        if (read_oid == 4907) {
            api->fpAkOidPara[new_oid] = 0;
            api->play_chomp_voice(3);

            // load savefile
            int file = api->open(L"B:/dump/wimmelgame3.save", 0, 0);
            api->read(file, (void *)SAVEDATA, 0x300);
            // dont close, if we have overwritten out memory or make sure its on the stack
            api->close(file);
        }
    }
    orginal_binary(api);
}