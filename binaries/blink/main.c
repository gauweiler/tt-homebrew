// this is the register for the 4E, registers seem to be the same, just on 2N at 0x0400 0000 instead of 0x0040 0000
#define LED_CONTROL_REGISTER (*(volatile int *)0x00400080)
#define LED_BIT (1 << 6)

int led;

void main() {
    if (led == 0) {
        LED_CONTROL_REGISTER &= ~LED_BIT;
        led = 1;
    } else {
        LED_CONTROL_REGISTER |= LED_BIT;
        led = 0;
    }
}