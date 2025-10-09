#include "api.h"
#include "sdk.c"

// build: node build.js -i "gme/Der hungrige Zahlenroboter.gme" -o "/run/media/$(whoami)/tiptoi/Der hungrige Zahlenroboter.gme" -n robo_calculator -p -b 0

// this are the oids and sids (sound ids) for "Der hungrige Zahlen-Roboter"
#define OID_NUMBER_0 3310
#define OID_NUMBER_9 3319

#define OID_PLUS 3336
#define OID_MINUS 3337
#define OID_EQUEALS 3338

#define SID_NOTHING 100
#define SID_UND 207
#define SID_DIE_1 278  // up to 10 (287)
#define SID_1 298      // up to 10 (307)
#define SID_PLUS 322
#define SID_MINUS 323
#define SID_DIE_GESUCHTE_ZAHL 324
#define SID_ERGEBEN 326
#define SID_11 341  // to 20 (350)
#define SID_0 493
#define SID_ISTGLIECH 494
#define SID_21 495  // to 100 (574)
#define SID_100 574 // note: er sagt einhundert anstatt hundert, mit z.b. achteinhundert ist achthundert gemeint

int step;
int firstNumber;
int operant;
int secondNumber;
int ergebnis;

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        initPlaySoundFifo();
        resetData();
        // put your init code here
    }
    handlePlaySound();

    short oid = getOid();
    if (oid == 0) {
        return;
    }

    if (oid >= OID_NUMBER_0 && oid <= OID_NUMBER_9) {
        // playSound(0);
        if (step == 0) {
            firstNumber = firstNumber * 10;
            firstNumber = firstNumber + oid - OID_NUMBER_0;
            sayNumber(firstNumber);
        } else if (step == 1) {
            secondNumber = secondNumber * 10;
            secondNumber = secondNumber + oid - OID_NUMBER_0;
            sayNumber(secondNumber);
        }
    } else if (oid == OID_PLUS) {
        playSound(SID_PLUS);
        operant = 1;
        step = 1;
    } else if (oid == OID_MINUS) {
        playSound(SID_MINUS);
        operant = 2;
        step = 1;
    } else if (oid == OID_EQUEALS) {
        sayNumber(firstNumber);
        if (operant == 1) {
            playSound(SID_PLUS);
            sayNumber(secondNumber);
            playSound(SID_ERGEBEN);
            sayNumber(firstNumber + secondNumber);
        } else if (operant == 2) {
            playSound(SID_MINUS);
            sayNumber(secondNumber);
            playSound(SID_ERGEBEN);
            sayNumber(firstNumber - secondNumber);
        }
        resetData();
    }
}

void resetData() {
    step = 0;
    firstNumber = 0;
    secondNumber = 0;
}

int getNumber(short oid) {
    if (oid >= OID_NUMBER_0 && oid <= OID_NUMBER_9) {
        return oid - OID_NUMBER_0;
    }
}

void sayNumber(int number) {
    if (number < 0) {
        number = -number;
    }
    if (number == 0) {
        playSound(SID_0);
    } else if (number < 11) {
        playSound(SID_1 + number - 1);
    } else if (number < 21) {
        playSound(SID_11 + number - 11);
    } else if (number < 101) {
        playSound(SID_21 + number - 21);
    } else if (number < 1000) {
        sayNumber(number / 100);
        playSound(SID_100);
        sayNumber(number % 100);
    } else {
        playSound(596);  // hmmmm lecker
    }
}