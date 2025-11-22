#include "api.h"

system_api *api;

#define SOUND_BUFF_LENGTH 10
typedef struct {
    short buff[SOUND_BUFF_LENGTH];
    int head;
    int tail;
    int count;
} playSoundFifo;

playSoundFifo psFifo;

void initTT(system_api *apiPara) {
    api = apiPara;
}

void playSoundNow(unsigned int sound_id) {
    int filehandle_current_gme = *api->p_filehandle_current_gme;
    unsigned int mediaFileTableOffset;
    unsigned int soundOffset;
    unsigned int soundLength;

    api->seek(filehandle_current_gme, 4, 0);  // set reading point to adrr 4 in gme file because there is offset for media file table

    api->read(filehandle_current_gme, &mediaFileTableOffset, 4);  // reads int from filehandle

    api->seek(filehandle_current_gme, mediaFileTableOffset + sound_id * 8, 0);  // times 8 because every entry is 4 bytes offset and 4 bytes length

    api->read(filehandle_current_gme, &soundOffset, 4);
    api->read(filehandle_current_gme, &soundLength, 4);

    api->play_sound(filehandle_current_gme, soundOffset, soundLength);
}

unsigned short getOid() {
    if (api->fpAkOidPara[new_oid] == 1) {
        api->fpAkOidPara[new_oid] = 0;
        short read_oid = *(short *)&api->fpAkOidPara[oid_offset];
        return read_oid;
    }
    return 0;
}

void initPlaySoundFifo() {
    psFifo.head = 0;
    psFifo.tail = 0;
    psFifo.count = 0;
}


void handlePlaySound() {
    if (psFifo.count != 0 && api->is_audio_playing() == 0) {
        playSoundNow(psFifo.buff[psFifo.head]);
        psFifo.head = (psFifo.head + 1) % SOUND_BUFF_LENGTH;
        psFifo.count--;
    }
}

/**
 * @brief Queue a sound for playback
 * @note Make sure to call initPlaySound() at setup and handlePlaySound() in a loop
 */
int playSound(unsigned int sound_id) {
    if (psFifo.count == SOUND_BUFF_LENGTH) {
        return 1;
    }

    psFifo.buff[psFifo.tail] = sound_id;
    psFifo.tail = (psFifo.tail + 1) % SOUND_BUFF_LENGTH;
    psFifo.count++;
    return 0;
}