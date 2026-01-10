#include "uart.c"

// int (*test) = (int *)0x0809000;

void main(char* eee) {
    uart_printf(eee);
    // uart_puts(eee);
    // uart_printf("hello from sysmodule\r\n");
    // uart_printf("start points to %x\r\n", *task);
    // uart_printf("start adrress located at %x\r\n", task);
    // if (*task != sysmodule_addr) {
    //     *task = sysmodule_addr;
    //     uart_printf("set to %x\r\n", *task);
    //     // sysmodule("e");
    // }
    // uart_printf("end sysmodule\r\n");
}

// todo: auto calc jumps and hook func
// ziel überlegen
// herausfinden wie sysmodule entfernt wurde
// in loop gecallt werden