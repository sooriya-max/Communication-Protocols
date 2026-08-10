#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t recieved_data = 0;
volatile int8_t bit_index = 7;

volatile uint8_t *PIN_D = 0x29;
volatile uint8_t *PORT_B = 0x25;
volatile uint8_t *DDR_B = 0x24;

ISR(INT0_vect)
{
    if(*PIND & 0x08)            //CHecks if MOSI is high
    {
        recieved_data |= (1 << bit_index);
    }
    bit_index--;

    if(bit_index == -1)
    {
        bit_index = 7;
        *PORT_B = recieved_data;        //Sending the Recieved data to portB for LEDs
    }
}

int main()
{
    *DDR_B = 0xFF;              //Setting the pins as Output
    return 0;
}