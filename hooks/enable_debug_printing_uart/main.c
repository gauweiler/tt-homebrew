#include "api.h"

// WARNING: ONLY FOR 2N-MT FIRMWARE

int int_to_str(char* dst, unsigned int value) {
    if (value == 0) {
        dst[0] = '0';
        dst[1] = 0;
        return 1;
    }
    char tmp[11];
    int ti = 0;
    while (value) {
        tmp[ti++] = '0' + (value % 10);
        value /= 10;
    }
    int pos = 0;
    for (int j = ti - 1; j >= 0; --j) {
        dst[pos++] = tmp[j];
    }
    dst[pos] = '\0';
    return pos;
}

void hook(unsigned char* string) {
    unsigned int a, b, c;
    __asm__ volatile(
        "mov %0, r1\n"
        "mov %1, r2\n"
        "mov %2, r3\n"
        : "=r"(a), "=r"(b), "=r"(c));
    int test;
    unsigned int args[3];
    args[0] = a;
    args[1] = b;
    args[2] = c;
    int currentArg = 0;
    unsigned char buff[12];
    int i = 0;

    int (*bios_uart_puts)(char*) = (void*)0x08003d8c;
    int (*bios_uart_putc)(char) = (void*)0x08003d18;

    while (string[i] != 0) {
        if (string[i] == '%') {
            i++;
            if (string[i] == 'd' || string[i] == 'x') {
                int_to_str(buff, args[currentArg]);
                bios_uart_puts(buff);
            } else if (string[i] == 'c') {
                bios_uart_putc(args[currentArg]);
            } else {
                bios_uart_putc(string[i - 1]);
                bios_uart_putc(string[i]);
            }
            currentArg++;
        } else {
            bios_uart_putc(string[i]);
        }
        i++;
    }

    bios_uart_puts(string);
}

void main(system_api* api) {
    void (*orginal_binary)(system_api*) = (void*)(BINARY_OFFSET + 4);
    volatile int* const pREG_SHARE_PIN_CTRL = (int*)0x04000074;
    volatile int* const pREG_UART_CFG1 = (int*)0x04036000;

    int pinctl = *pREG_SHARE_PIN_CTRL;
    *pREG_SHARE_PIN_CTRL = pinctl | 1;

    int uartctl = *pREG_UART_CFG1;
    *pREG_UART_CFG1 = uartctl | 0x200000;

    api->printf = hook;

    int (*bios_uart_puts)(char*) = (void*)0x08003d8c;
    bios_uart_puts("o");
    orginal_binary(api);
}