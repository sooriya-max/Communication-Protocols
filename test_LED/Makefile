MCU = atmega328p
CC = avr-gcc
FREQUENCY = 16000000UL
OBJCOPY = avr-objcopy
CFLAGS = -g -mmcu=$(MCU) -DF_CPU=$(FREQUENCY) -Os

all:
	$(CC) $(CFLAGS) main.c -o main.elf
	$(OBJCOPY) -O ihex main.elf main.hex

clean:
	rm -f *.elf *.hex