MCU = atmega328p
CC = avr-gcc
OBJCOPY = avr-objcopy
TARGET = main
CFLAGS = -mmcu=$(MCU) -g -o

all: tx rx

tx:
	$(CC) Master/$(TARGET).c $(CFLAGS) Master/$(TARGET).elf
	$(OBJCOPY) -O ihex Master/main.elf /media/sf_LinuxShared/SPI/tx.hex

rx:
	$(CC) Slave/$(TARGET).c $(CFLAGS) Slave/$(TARGET).elf
	$(OBJCOPY) -O ihex Slave/main.elf /media/sf_LinuxShared/SPI/rx.hex


clean:
	rm -rf Master/*.elf Master/*.exe Slave/*.elf Slave/*.o Master/main Slave/main