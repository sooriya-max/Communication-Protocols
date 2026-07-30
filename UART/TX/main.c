#include <avr/io.h>

#define BIT_CYCLES 139  //THis tells us the number of cycles we need per bit.
#define NOP __asm__ __volatile__("nop") //This Operation is for a single Cycle of NOP


volatile uint8_t *DDR_C = (volatile uint8_t *)0x27;     //DDR address of POrt C ; Set Direction
volatile uint8_t *Port_C = (volatile uint8_t *)0x28;    //Port C Address        ; Set Values

static inline void delay_n_cycles();

void uart_tx(uint8_t data)
{
    /* START BIT */
    *Port_C &= ~0x01;
    delay_n_cycles();
    
    /* ACTUAL DATA */
    for(uint8_t i = 0; i < 8; i++)
    {
        if(data & 0x01)
        {
            *Port_C |= (0x01);
        }
        else
        {
            *Port_C &= ~(0x01);    
        }
        delay_n_cycles();
        data = data >> 1;
    }
    
    /* STOP BIT */
    *Port_C |= 0x01;
    delay_n_cycles();
}

int main()
{
    /*INITIALIZATION of PORT C For Transmission*/
    
    *DDR_C |= 0x1;  //Setting the LSB of DDR C to output
    
    *Port_C |= 0x1; //Setting the Pin High Before Transmission

    uint8_t data = 0xA5; //Temp Data;  1111 1110
    while(1)
    {
        uart_tx(data);
        for(volatile long int i = 0; i < 50000; i++);
    }
    

    return 0;
}

/*  DELAY BLOCK FOR EXACTLY ONE CYCLE */
static inline void delay_n_cycles(void)
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

