###############################################################################
# Makefile for the project Proto2
###############################################################################

## General Flags
PROJECT = Proto2
MCU = atmega32
TARGET = Proto2.elf
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Proto2.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = Proto2.o ADCcontrol.o LedsControl.o moteur.o motorControl.o portsControl.o sonars.o timer.o uart.o os_mem.o os_cpu_c.o ucos_ii.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Proto2.hex Proto2.eep Proto2.lss size

## Compile
Proto2.o: ../Proto2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ADCcontrol.o: ../ADCcontrol.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LedsControl.o: ../LedsControl.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

moteur.o: ../moteur.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

motorControl.o: ../motorControl.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

portsControl.o: ../portsControl.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sonars.o: ../sonars.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: ../timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: ../uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

os_mem.o: ../ucos_core/os_mem.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

os_cpu_c.o: ../ucos_core/os_cpu_c.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ucos_ii.o: ../ucos_core/ucos_ii.c
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
	-rm -rf $(OBJECTS) Proto2.elf dep/* Proto2.hex Proto2.eep Proto2.lss Proto2.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

