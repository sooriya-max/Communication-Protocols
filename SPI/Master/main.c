#include <stdint.h>

volatile uint8_t* PINC = (volatile uint8_t *)0x26;
volatile uint8_t* DDRC = (volatile uint8_t *)0x27;
volatile uint8_t* PORTC = (volatile uint8_t *)0x28;


volatile uint8_t* PIN_D = (volatile uint8_t *)0x29;
volatile uint8_t* DDR_D = (volatile uint8_t *)0x2A;

//For the LEDS
volatile uint8_t *PORT_B = (volatile uint8_t *)0x25;
volatile uint8_t *DDR_B = (volatile uint8_t *)0x24;

static inline void delay_160_cycles();      //This is for Delaying the Master by 160 cycles for INterupt to service it cleanly in slave for the next bit


uint8_t spi_tx(uint8_t data, uint8_t slave_selection)
{
    uint8_t recieved_data = 0x00;   //setting the Recieved data to be 0

    /*SLAVE Stablizing and Selcting Lines - START*/
    *PORTC |= (0b111 << 3);  //Setting the Chip Select to one after the operations are done to deselect it
    *PORTC &= ~(1 << (slave_selection + 2));        //This will select only the required slave meanwhile preserving the remaining ones
    /*SLAVE Stablizing and Selcting Lines - END*/


    for(int8_t i = 7; i >= 0; i--)
    {
        if(data & 0x80)
        {
            *PORTC |= 0x02;   //Setting MOSI to 1
            //delay_160_cycles();

        }
        else
        {
            *PORTC &= ~0x02;  //Setting MOSI to 0
            //delay_160_cycles();
            
        }
        *PORTC |= 0x01;                     //Pulling SCK high
        delay_160_cycles();
        *PORTC &= ~0x01;                    //Pulling SCK Low
        __asm__ __volatile__("nop");        //To Let the MISO Settle after the falling SCK Edge
        if(*PINC & 0x04)                    //Checking MISO bit 2 for Input
        {
            recieved_data |= (1 << i);
        }
        data <<= 1;
    }
    *PORTC |= (0b111 << 3);  //Setting the Chip Select to one after the operations are done to deselect it
    return recieved_data;
}

int main()
{
    *DDRC = 0x3B;       //SCK and MOSI, SS are 1, MISO is 0, so 0x0B ; So setting the Direction //Adding two SS lines on PC4 and PC5
    *PORTC = 0x38;      //Setting CS as High for active low scenario                            //The Added lines are set high
    *DDR_B = 0xFF;              //Setting the pins as Output for LED

    /*DEBUG*/
    *PORT_B = 0xAA;

    *DDR_D = 0x00;      //All three lines are input switches

    uint8_t slave_selection = 0;
    uint8_t prev_slave = 0;
    uint8_t data = 0xF7;
    uint8_t recieved_data = 0x00;

    while(1)
    {
        /* SWITCH SELECTION LOGIC - START*/
    
        if(*PIN_D & 0x01)
        {
            slave_selection = 1;
        }
        else if (*PIN_D & 0x02)
        {
            slave_selection = 2;
        }
        else if (*PIN_D & 0x04)
        {
            slave_selection = 3;
        }
        else
        {
            slave_selection = 0;
        }

        /* SWITCH SELECTION LOGIC - END*/

        if(slave_selection != 0)
        {
            if(prev_slave != 0)
                spi_tx(0x00, prev_slave);
            recieved_data =  spi_tx(data, slave_selection);
            *PORT_B = recieved_data;
            prev_slave = slave_selection;
        }

        //while((*PIN_D & 0x07));          //Wait until all SWITCHES are released
        /*DEBUG*/
        //for(volatile long int i = 0; i < 500000; i++);
        //while(!(*PIN_D & 0x07));   // wait for press
        while(*PIN_D & 0x07);      // wait for release
    }
    return 0;
}

static inline void delay_160_cycles()
{
    for(uint8_t i = 0; i < 40; i++)
    {
        __asm__ __volatile__("NOP");
    }
}