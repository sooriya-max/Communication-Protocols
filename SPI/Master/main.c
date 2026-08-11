#include <stdint.h>

volatile uint8_t* PINC = (volatile uint8_t *)0x26;
volatile uint8_t* DDRC = (volatile uint8_t *)0x27;
volatile uint8_t* PORTC = (volatile uint8_t *)0x28;

static inline void delay_160_cycles();      //This is for Delaying the Master by 160 cycles for INterupt to service it cleanly in slave for the next bit


uint8_t spi_tx(uint8_t data)
{
    uint8_t recieved_data = 0x00;   //setting the Recieved data to be 0
    *PORTC &= ~0x08; //Setting the Chip Select to zero to select it
    for(int8_t i = 7; i >= 0; i--)
    {
        if(data & 0x80)
        {
            *PORTC |= 0x02;   //Setting MOSI to 1
            delay_160_cycles();

        }
        else
        {
            *PORTC &= ~0x02;  //Setting MOSI to 0
            delay_160_cycles();
            
        }
        *PORTC |= 0x01;                     //Pulling SCK high
        *PORTC &= ~0x01;                    //Pulling SCK Low
        __asm__ __volatile__("nop");        //To Let the MISO Settle after the falling SCK Edge
        if(*PINC & 0x04)                    //Checking MISO bit 2 for Input
        {
            recieved_data |= (1 << i);
        }
        data <<= 1;
    }
    *PORTC |= 0x08;  //Setting the Chip Select to one after the operations are done to deselect it
    return recieved_data;
}

int main()
{
    *DDRC = 0x0B;       //SCK and MOSI, SS are 1, MISO is 0, so 0x0B ; So setting the Direction
    *PORTC = 0x08;      //Setting CS as High for active low scenario
    uint8_t data = 0xA6;
    uint8_t recieved_data = 0x00;
    recieved_data = spi_tx(data);
    return 0;
}

static inline void delay_160_cycles()
{
    for(uint8_t i = 0; i < 40; i++)
    {
        __asm__ __volatile__("NOP");
    }
}