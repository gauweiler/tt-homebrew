
// TODO WARNING: THIS FILE HAS FIRMWARE VERSION SPECIFIC OFFSETS
// AND WONT WORK ON ANY OTHER FIRMWARE VERSION (2N-MT, )

// NOTE: the speaker is lower volume if uart is enabled

#include <stdarg.h>

#define UART_INTERFACE_EN (1 << 21)

volatile int* const pREG_SHARE_PIN_CTRL = (int*)0x04000074;
volatile int* const pREG_UART_CFG1 = (int*)0x04036000;

int (*bios_uart_putc)(char) = (void*)0x08003d18;
int (*bios_uart_puts)(char*) = (void*)0x08003d8c;

void init_uart() {
    int pinctl = *pREG_SHARE_PIN_CTRL;
    *pREG_SHARE_PIN_CTRL = pinctl | 1;

    int uartctl = *pREG_UART_CFG1;
    *pREG_UART_CFG1 = uartctl | UART_INTERFACE_EN;
}

void uart_putc(char c) { bios_uart_putc(c); }

void uart_puts(char* str) { bios_uart_puts(str); }

void print_integer(int value, int base, int is_signed) {
    char buffer[33];
    int i = 0;
    unsigned int u_val;

    if (value == 0) {
        uart_putc('0');
        return;
    }

    if (is_signed && value < 0) {
        uart_putc('-');
        u_val = (unsigned int)(-(value + 1)) + 1;
    } else {
        u_val = (unsigned int)value;
    }

    // because we cant devide
    if (base == 10) {
        while (u_val > 0) {
            int digit = u_val % 10;

            if (digit < 10) {
                buffer[i] = digit + '0';
            } else {
                buffer[i] = digit - 10 + 'a';
            }

            u_val /= 10;
            i++;
        }
    } else {
        while (u_val > 0) {
            int digit = u_val % 16;

            if (digit < 10) {
                buffer[i] = digit + '0';
            } else {
                buffer[i] = digit - 10 + 'a';
            }

            u_val /= 16;
            i++;
        }
    }

    while (i > 0) {
        i--;
        uart_putc(buffer[i]);
    }
}


void uart_printf(char* str, ...) {
    va_list args;
    va_start(args, str);

    while (*str != '\0') {
        if (*str != '%') {
            uart_putc(*str++);
            continue;
        }

        str++;

        if (*str == '\0') {
            break;
        }

        switch (*str) {
            case 'd': {
                int c = va_arg(args, int);
                print_integer(c, 10, 0);
                break;
            }
            case 'x': {
                int c = va_arg(args, int);
                print_integer(c, 16, 0);
                break;
            }
            case 'c': {
                int c = va_arg(args, int);
                uart_putc(c);
                break;
            }

            default: {
                uart_putc('%');
                uart_putc(*str);
                break;
            }
        }
        str++;
    }
}
