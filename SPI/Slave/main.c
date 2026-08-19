#include <avr/interrupt.h>
#include <stdint.h>

volatile uint8_t recieved_data = 0;
volatile int8_t bit_index = 7;
volatile int8_t tx_bit_index = 7;
volatile uint8_t tx_data = 0x3D;


//FOr the Actual SPI slave bit bang
volatile uint8_t *PIN_D = (volatile uint8_t *)0x29;
volatile uint8_t *DDR_D = (volatile uint8_t *)0x2A;

//FOr MISO Bit Transfer
volatile uint8_t *PORT_D = (volatile uint8_t *)0x2B;

//For the LEDS
volatile uint8_t *PORT_B = (volatile uint8_t *)0x25;
volatile uint8_t *DDR_B = (volatile uint8_t *)0x24;


//Interrupt Specific
volatile uint8_t *EICRA_INT = (volatile uint8_t *)0x69;         //FOr configuring the External Interrupts in AVR
volatile uint8_t *EIMSK_INT = (volatile uint8_t *)0x3D;         //For Enabling the INT0


/*INTERRUPT SERVICE ROUTINE For the Incoming Bit*/
ISR(INT0_vect)
{
    /*DEBUG SESSION*/
    //if(*PIN_D & 0x20) return;   // CS is PD5 — if HIGH, slave not selected, bail
    // if(!(*PIN_D & 0x20))
    // {
    //     *DDR_D |= (1 << 4);
    // }
    // else
    // {
    //     *DDR_D &= ~(1 << 4);
    //     *PORT_D &= ~(1 << 4);
    // }


    /*CHECK if the Edge is Rising to Sample MOSI*/
    if(*PIN_D & 0x04)  // PD2 is HIGH — rising edge just fired
    {
        if(tx_data & (1 << tx_bit_index))
        {
            *PORT_D |= (1 << 4);    //Setting PD4 as HIGH in MISO
        }
        else
        {
            *PORT_D &= ~(1 << 4);   //Setting PD4 as LOW in MISO
        }
        tx_bit_index--;

        if(tx_bit_index == -1)
        {
            tx_bit_index = 7;
        }
    }
    /*CHECK if the Edge is Falling to send MISO*/
    else
    {
        
        if(*PIN_D & 0x08)            //CHecks if MOSI is high
        {
            recieved_data |= (1 << bit_index);
        }
        else
        {
            recieved_data &= ~(1 << bit_index);  // explicitly clear the bit
        }
        
        bit_index--;

        if(bit_index == -1)
        {
            bit_index = 7;
            *PORT_B = recieved_data;        //Sending the Recieved data to portB for LEDs
            recieved_data = 0x00;           //Clears for the next frame
        }
    }

    
}

int main()
{
    *DDR_B = 0xFF;              //Setting the pins as Output for LED
    *DDR_D = 0x10;              //Setting MISO as Output
    *EICRA_INT = 0x01;          //Setting the last two bits to 1, as it makes the INT0 trigger on both edges 
    *EIMSK_INT = 0x01;          //This Enables the INT0
    sei();       //Global interrupts enable

    while(1);

    return 0;
}