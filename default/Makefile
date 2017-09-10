###############################################################################
# Makefile for the project linesimulater
###############################################################################

## General Flags
PROJECT = linesimulater
MCU = atmega128
TARGET = linesimulater.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2   -std=gnu99    -finput-charset=UTF-8  -DF_CPU=7372800UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=linesimulater.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = linesimulater.o W5100.o SPI.o main.o http.o xmlparser.o logger.o Globals.o timers.o NTPclient.o ports.o controller.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) linesimulater.hex linesimulater.eep linesimulater.lss size

## Compile
linesimulater.o: ../linesimulater.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

W5100.o: ../W5100.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SPI.o: ../SPI.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

http.o: ../http.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

xmlparser.o: ../xmlparser.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

logger.o: ../logger.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Globals.o: ../Globals.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timers.o: ../timers.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

NTPclient.o: ../NTPclient.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ports.o: ../ports.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

controller.o: ../controller.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) linesimulater.elf dep/* linesimulater.hex linesimulater.eep linesimulater.lss linesimulater.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

