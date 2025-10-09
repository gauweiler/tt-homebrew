#include "api.h"

#define OID_TO_SAVE 4906
#define OID_TO_LOAD 4907

// only 3L
// game 9 in den bergen | red 4906 | green 4907
// node build.js -i Wimmelbuch.gme  -o Wimmelbuchwraped.gme -n inject -b 9
// note: can only read oids from current book
// todo: multiple saves, check ghidra that there are only local vars

void (*orginal_binary)(system_api *) = (void *)(BINARY_OFFSET + 4);

void main(system_api *api) {
    if (api->fpAkOidPara[new_oid] == 1) {
        short read_oid = *(short *)&api->fpAkOidPara[oid_offset];
        if (read_oid == OID_TO_SAVE) {
            api->fpAkOidPara[new_oid] = 0;
            api->play_chomp_voice(4);
            // create a savefile
            int file = api->open(L"B:/dump/wimmelgame3.save", 1, 1);
            api->write(file, (void *)SAVEDATA, 0xf000);
            api->close(file);
        }
        if (read_oid == OID_TO_LOAD) {
            api->fpAkOidPara[new_oid] = 0;
            api->play_chomp_voice(3);
            // load savefile
            int file = api->open(L"B:/dump/wimmelgame3.save", 0, 0);
            api->read(file, (void *)SAVEDATA, 0x300);
            api->close(file);
        }
    }
    orginal_binary(api);
}