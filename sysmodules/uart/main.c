#include "uart.c"

void main() {
    while (1) {
        char value;
        if (uart_getc(&value)) {
            uart_printf("Got %c\r\n", value);
        }
    }
}
