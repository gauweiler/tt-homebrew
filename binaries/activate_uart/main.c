#include "api.h"
#include "sdk.c"

// THIS CODE CURRENTLY DOESNT WORK

int (*bios_uart_putc)(char)       = (void*)0x08003d18;

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        // bootrom_uart_init();
        
    }
    volatile int *isEnabled = (volatile int *)0x08008d1c;
    *isEnabled = 0;
    api->printf("ee");
    bios_uart_putc('e');
    // bootrom_uart_putc('e');

}