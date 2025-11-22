#include "api.h"
#include "sdk.c"

volatile int* const pREG_SHARE_PIN_CTRL = (int*)0x04000074;
volatile int* const pREG_UART_CFG1 = (int*)0x04036000;
int (*bios_uart_putc)(char) = (void*)0x08003d18;
int (*bios_uart_puts)(char*) = (void*)0x08003d8c;

void main(system_api* apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        int pinctl = *pREG_SHARE_PIN_CTRL;
        *pREG_SHARE_PIN_CTRL = pinctl | 1;

        int uartctl = *pREG_UART_CFG1;
        *pREG_UART_CFG1 = uartctl | 0x200000;
    }

    bios_uart_puts("hallo welt\n");
}