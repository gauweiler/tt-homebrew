#include "api.h"
#include "sdk.c"

int i;
int file;
int fileOpen;

// note: if you try to read from unmaped memory, a interupt will turn off the pen
// also you have to create the dump foler first or change the filepath

void main(system_api *apiOrgi) {
    initTT(apiOrgi);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        i = 0;
#ifdef build_for_2N
        file = api->open(L"B:/dump/sysapi2N", 1, 1);
        api->write(file, apiOrgi, 0x1ff);
        api->close(file);
        // todo: add more offsets for diffrent pens
#else
        file = api->open(L"B:/dump/0x82_9000_3L", 1, 1);
        api->play_chomp_voice(4);
        fileOpen = 1;
        int sysapifile = api->open(L"B:/dump/sysapi3L", 1, 1);
        api->write(sysapifile, apiOrgi, 0x2ff);
        api->close(sysapifile);
        int oidfile = api->open(L"B:/dump/oid3L", 1, 1);
        api->write(oidfile, apiOrgi, 0x2ff);
        api->close(oidfile);
#endif
    }

    // todo: 2N, 3L bios, 3l bootrom
    // todo: test dumpRam func

    // this works
    if (i < 0x15ffff) {
        api->write(file, START_PROG_BIN + i, 0xffff);
        i += 0xffff;
    } else {
        if (fileOpen == 1) {
            api->close(file);
            fileOpen = 0;
        }
        if (api->is_audio_playing() == 0) {
            api->play_chomp_voice(6);  // When the dump is over, you hear the same sound over and over again, you can safely turn off the pen
        }
    }
}
