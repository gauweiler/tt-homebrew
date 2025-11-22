#include "api.h"
#include "sdk.c"
#include "uart.c"

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        initPlaySoundFifo();
        // put your init code here
        init_uart();
        uart_printf("api: %x\r\n", (int)apiPara);
        uart_printf("malloc: %x\r\n", api->malloc(0x10000));
    }
    // put your loop code here

    handlePlaySound();
}