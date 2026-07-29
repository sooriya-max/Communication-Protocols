#include <avr/io.h>

volatile uint8_t *PIN_B = (volatile uint8_t *)0x23; //Initializing the PIN_B for Rx

volatile uint8_t *DDR_B = (volatile uint8_t *)0x24;     //DDR address of POrt B ; Set Direction
volatile uint8_t *Port_B = (volatile uint8_t *)0x25;    //Port B Address        ; Set Values

/*For Sending data to the LEDs using port D*/
volatile uint8_t *DDR_D = (volatile uint8_t *)0x2A;     //DDR address of POrt D ; Set Direction
volatile uint8_t *Port_D = (volatile uint8_t *)0x2B;    //Port D Address        ; Set Values

static inline void delay_208_cycles();  //This is for the first time after recieving the start bit to get a center reading on bit 1 (1.5x)
static inline void delay_139_cycles();  //THis is for the gap period of each bit


uint8_t uart_rx(void)
{
    uint8_t data = 0;
    while (*PIN_B & 0x01);
    delay_208_cycles();
    for(uint8_t i = 0; i < 8; i++)
    {
        if(*PIN_B & 0x01)
        {
            data |= (1 << i);
        }
        else
        {
            data &= ~(1 << i);
        }
        delay_139_cycles();
    }
    if(*PIN_B & 0x01)
        return data;
    else
        data = 0;

    return data;   

}


int main()
{
    *DDR_B |= (1 << 5);   // PB5 as output
    *Port_B |= (1 << 5);  // PB5 HIGH — onboard LED on uno1
    while(1);
    *DDR_D = 0xFF; //Setting the DDR of PORT D to Output for LEDs
    *Port_D = 0xFF;
    while(1);
    // *DDR_B &= ~0x01;  //Setting the LSB of DDR B to Input
    
    // *Port_B |= 0x01; //Setting the Pin High for Internal PullUp
    
    // /*DEBUG SCENARIO*/
    // *Port_D = 0xFF;
    // for(volatile long i = 0; i < 1000000; i++);  // visible delay


    // while(1)
    // {
    //     uint8_t data = uart_rx();
    //     *Port_D = data;
    
    // }
    



    return 0;
}


static inline void delay_208_cycles()
{
    for(uint8_t i = 0; i < 51; i++)
    {
        __asm__ __volatile__("NOP");
    }
}

static inline void delay_139_cycles()
{
    __asm__ __volatile__(
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 10
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 20
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 30
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 40
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 50
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 60
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 70
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 80
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 90
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 100
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 110
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 120
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"  // 130
        "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"        // 139
    );
}