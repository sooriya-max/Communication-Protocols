#include <avr/interrupt.h>
#include <stdint.h>

volatile uint8_t recieved_data = 0;
volatile int8_t bit_index = 7;


//FOr the Actual SPI slave bit bang
volatile uint8_t *PIN_D = (volatile uint8_t *)0x29;
volatile uint8_t *DDR_D = (volatile uint8_t *)0x27;

//For the LEDS
volatile uint8_t *PORT_B = (volatile uint8_t *)0x25;
volatile uint8_t *DDR_B = (volatile uint8_t *)0x24;


//Interrupt Specific
volatile uint8_t *EICRA_INT = (volatile uint8_t *)0x69;         //FOr configuring the External Interrupts in AVR
volatile uint8_t *EIMSK_INT = (volatile uint8_t *)0x1D;         //For Enabling the INT0

ISR(INT0_vect)
{
    if(*PIN_D & 0x08)            //CHecks if MOSI is high
    {
        recieved_data |= (1 << bit_index);
    }
    bit_index--;

    if(bit_index == -1)
    {
        bit_index = 7;
        *PORT_B = recieved_data;        //Sending the Recieved data to portB for LEDs
        recieved_data = 0x00;           //Clears for the next frame
    }
}

int main()
{
    *DDR_B = 0xFF;              //Setting the pins as Output for LED
    *DDR_D = 0x10;              //Setting MISO as Output
    *EICRA_INT = 0x03;          //Setting the last two bits to 1, as it makes the INT0 trigger on a rishing edge
    *EIMSK_INT = 0x01;          //This Enables the INT0
    sei();       //Global interrupts enable

    while(1);

    return 0;
}