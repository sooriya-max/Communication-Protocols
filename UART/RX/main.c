#include <avr/io.h>

volatile uint8_t *PIN_D = (volatile uint8_t *)0x29; //Initializing the PIN_D for Rx

static inline void delay_208_cycles();  //This is for the first time after recieving the start bit to get a center reading on bit 1 (1.5x)
static inline void delay_139_cycles();  //THis is for the gap period of each bit

int main()
{

    while (*PIN_D & 0x01);
    delay_208_cycles();
    



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