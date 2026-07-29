#include <avr/io.h>

volatile uint8_t *DDR_D  = (volatile uint8_t *)0x2A;
volatile uint8_t *Port_D = (volatile uint8_t *)0x2B;

int main()
{
    *DDR_D  = 0xFF;
    *Port_D = 0xFF;
    while(1);
    return 0;
}