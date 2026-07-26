MCU = atmega328p
CC = avr-gcc
FREQUENCY = 16000000UL
OBJCOPY = avr-objcopy

CFLAGS = -g -mmcu=$(MCU) -DF_CPU=$(FREQUENCY) -Os


tx:
	$(CC) $(CFLAGS) TX/main.c -o TX/main.elf
	$(OBJCOPY) -O ihex TX/main.elf TX/tx.hex

rx:
	$(CC) $(CFLAGS) RX/main.c -o RX/main.elf
	$(OBJCOPY) -O ihex RX/main.elf RX/rx.hex




clean:
	rm -f TX/*.elf TX/*.hex RX/*.elf RX/*.hex