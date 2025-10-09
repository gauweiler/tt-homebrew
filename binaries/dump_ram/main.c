#include "api.h"
#include "sdk.c"

int i;
int remaining;
int file;
int registerFile;
int ledState;
int last_button_state;

#define SPEAKER_ENABLE_CONTROL_REGISTER (*(volatile int*)0x00400080)
#define LED_CONTROL_REGISTER (*(volatile int*)0x00400080) // crashes
#define BUTTONS (*(volatile int*)0x004000bc)
#define NEW_BUTTON (*(volatile int*)0x004000f0) // any button
#define INT_BUTTON (*(volatile int*)0x004000e0)

#define LED_BIT (1 << 6)
#define SPEAKER_BIT (1 << 11)

#define VOL_UP_BIT (1 << 7)
#define VOL_DOWN_BIT (1 << 24)
#define MIC_BIT (1 << 4)
#define BLOCKSIZE 0xffff

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
        i = 0x00829000;
        // remaining = 0x1D6FFF; // until end of stack, but there is unmaped memory in between
        remaining = 0x180000;

        int sysapifile = api->open(L"B:/dump/sysapi3L", 1, 1);
        api->write(sysapifile, apiOrgi, 0x2ff);
        api->close(sysapifile);

        int bootromFile = api->open(L"B:/dump/bootrom", 1, 1);
        // reading from 0 doesnt work
        api->write(bootromFile, (void *)4, 0x2fff);
        api->close(bootromFile);


        // we are already supervisor
        // unsigned int cpsr;
        // __asm__ __volatile__("mrs %0, cpsr" : "=r"(cpsr));

        // __asm__ __volatile__("svc #0");

        // unsigned int cpsr2;
        // __asm__ __volatile__("mrs %0, cpsr" : "=r"(cpsr2));

        // int superFile = api->open(L"B:/dump/super", 1, 1);
        // api->write(superFile, &cpsr, 4);
        // api->write(superFile, &cpsr2, 4);
        // api->close(superFile);
        

        int biosFile = api->open(L"B:/dump/bios", 1, 1);
        api->write(biosFile, (void *)0x00800000, 0x1000); // should be 0x8620
        api->close(biosFile);

        registerFile = api->open(L"B:/dump/registers", 1, 1);
        api->write(registerFile, (void *)0x00400000, 0x200); 

        int pSysapiFile = api->open(L"B:/dump/pSysapi", 1, 1);
        api->write(pSysapiFile, (void *)&apiOrgi, 4);
        api->close(pSysapiFile);

        int fdFile = api->open(L"B:/dump/fd", 1, 1);
        api->write(fdFile, &file, 4);
        api->close(fdFile);

        int stackFile = api->open(L"B:/dump/stackpointer", 1, 1);
        volatile int stack_marker = 0;
        unsigned int addr = (unsigned int)&stack_marker;
        api->write(stackFile, &addr, 4);
        api->close(stackFile);

        ledState = 0;
        last_button_state = BUTTONS;
#endif
    }

    // INT_BUTTON = 0x4000; // this disables all buttons

    // if ()

    // todo: 2N, 3L bios, 3l bootrom
    // todo: test dumpRam func

    // if (!(BUTTONS & VOL_DOWN_BIT)) {
    //     ledState = 1;
    // }

    // if (!(BUTTONS & VOL_UP_BIT)) {
    //     ledState = 0;
    // }
    // if (!(BUTTONS & MIC_BIT) && (last_button_state & MIC_BIT)) {
    //     api->play_chomp_voice(1);
    // }

    if (ledState == 0) {
        LED_CONTROL_REGISTER &= ~LED_BIT;
        // ledState = 1;
    } else {
        LED_CONTROL_REGISTER |= LED_BIT;
        // ledState = 0;
    }

    
    last_button_state = BUTTONS;
    // this works
    if (remaining > BLOCKSIZE) {
        api->write(registerFile, (void *)0x00400000, 0x200); 
        // api->write(file, (void *)i, BLOCKSIZE);
        i += BLOCKSIZE;
        remaining -= BLOCKSIZE;
    } else if (remaining != 0) {
        api->write(file, (void *)i, remaining);
        remaining -= remaining;
        api->close(registerFile);
        api->close(file);
    } else {
        if (api->is_audio_playing() == 0) {
            api->play_chomp_voice(6);  // When the dump is over, you hear the same sound over and over again, you can safely turn off the pen
        }
    }
}
