#include "api.h"

// only 3L
// game 9 in den bergen | red 4906 | green 4907
// node build.js -i Wimmelbuch.gme  -o Wimmelbuchwraped.gme -n inject -b 9
// note: can only read oids from current book
// todo: multiple saves
// todo: overwrite api for print, etc
// todo: replay inputs (tas)

int currTime;
int isRecording;
int recordingFile;

int isReplaying;
int replayFile;
int targetTime;

void (*orginal_binary)(system_api *) = (void *)0x00930004;
void (*play_sound)(unsigned int) = (void *)0x00930794;

void main(system_api *api) {
    currTime++;
    if (api->fpAkOidPara[First_time_exec] == 0) {
        currTime = 0;
        isRecording = 0;
        isReplaying = 0;
    }

    if (api->fpAkOidPara[new_oid] == 1) {
        short read_oid = *(short *)&api->fpAkOidPara[oid_offset];

        if (isRecording == 1) {
            api->write(recordingFile, &currTime, 4);
            api->write(recordingFile, &read_oid, 2);
        }

        if (read_oid == 4906) {
            api->fpAkOidPara[new_oid] = 0;
            if (!api->is_audio_playing()) {
                if (isRecording == 1) {
                    api->play_chomp_voice(5);
                    api->close(recordingFile);
                    isRecording = 0;
                } else {
                    api->play_chomp_voice(4);
                    recordingFile = api->open(L"B:/dump/wimmelsuch.tas", 1, 1);
                    isRecording = 1;
                }
            }
        }

        // replaying hat ein bug
        if (read_oid == 4907) {
            *(short *)&api->fpAkOidPara[oid_offset] = 5265;
            int oidparafile = api->open(L"B:/dump/OidPara", 1, 1);
            api->write(oidparafile, &api, 4);
            api->write(oidparafile, &api->fpAkOidPara, 4);
            api->write(oidparafile, &oidparafile, 4);
            api->write(oidparafile, api->fpAkOidPara, 0x2fff);
            api->close(oidparafile);
            // api->fpAkOidPara[new_oid] = 0;
            // api->play_chomp_voice(3);

            // replayFile = api->open(L"B:/dump/wimmelsuch.tas", 0, 0);
            // api->read(replayFile, &targetTime, 4);
            // isReplaying = 1;
        }
    }

    if (isReplaying == 1) {
        if (currTime == targetTime) {
            api->fpAkOidPara[new_oid] = 1;
            api->read(replayFile, (void *)api->fpAkOidPara[oid_offset], 2);
            api->read(replayFile, &targetTime, 4);
        }
    }

    if (currTime == 300) {
        api->play_chomp_voice(8);
        api->fpAkOidPara[new_oid] = 1;
        *(short *)&api->fpAkOidPara[oid_offset] = 5265;
    }

    orginal_binary(api);
}