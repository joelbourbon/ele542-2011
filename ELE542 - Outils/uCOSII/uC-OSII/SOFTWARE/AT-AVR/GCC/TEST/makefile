#
#
# simple make file for ucos-ii avr-gcc port
#
# AVR-GCC port version : 1.0 	2001-04-02  Jesper Hansen (jesperh@telia.com)
#
#
#

CC		= avr-gcc
AS		= avr-gcc -x assembler-with-cpp	
RM		= rm -f
RN		= mv
BIN		= avr-objcopy



TRG	= test
TRGDIR	= ../OBJ
LSTDIR  = ../LST
INCDIR	= .
LIBDIR	= $(AVR)/avr/lib
SHELL   = $(AVR)/bin/sh.exe
FORMAT 	= srec



#
# Select the processor and appropriate linker script below
#

#
# At90S8515
#

MCU 	= at90s8515
LDFLAGS = -T../source/avr85xx.x 


#############

#
# Atmega103
#

#MCU 	= atmega103
#LDFLAGS = -T../source/avrmega103.x 



######################################################################################
######################################################################################

CPFLAGS = -g -O3 -Wall -Wstrict-prototypes -Wa,-ahlms=$(LSTDIR)/$(*F).lst -mmcu=$(MCU)
ASFLAGS = -Wa,-gstabs -mmcu=$(MCU)
LDFLAGS += -Wl,-Map=test.map,--cref -mmcu=$(MCU)

######################################################################################

CSRC	= test.c os_cpu_c.c ucos_ii.c
ASRC	= os_cpu_a.asm
OBJ	= $(CSRC:%.c=$(TRGDIR)/%.o) $(ASRC:%.asm=$(TRGDIR)/%.o)

######################################################################################

all:	$(TRG).obj $(TRG).elf $(TRG).rom $(TRG).eep

$(TRGDIR)/$(TRG).o: $(TRG).c
	$(CC) -c $(CPFLAGS) -I$(INCDIR) -o $@ $<

$(TRGDIR)/os_cpu_c.o: ../source/os_cpu_c.c
	$(CC) -c $(CPFLAGS) -I$(INCDIR) -o $@ $<

$(TRGDIR)/os_cpu_a.o: ../source/os_cpu_a.asm
	$(AS) -c $(ASFLAGS) -I$(INCDIR) -o $@ $<

$(TRGDIR)/ucos_ii.o: ../../../source/ucos_ii.c
	$(CC) -c $(CPFLAGS) -I$(INCDIR) -o $@ $<

######################################################################################		  


%elf: $(OBJ)
	$(CC) $(OBJ) $(LIB) $(LDFLAGS) -o $@

%obj: %elf
	$(BIN) -O avrobj $< $@

%rom: %elf
	$(BIN) -O $(FORMAT) $< $@

%eep: %elf
	$(BIN) -j .eeprom --set-section-flags=.eeprom="alloc,load" -O $(FORMAT) $< $@

######################################################################################

clean:
		rm -f $(TRGDIR)/*.o
		rm -f $(LSTDIR)/*.lst

cleanall:
		rm -f $(TRGDIR)/*.o
		rm -f $(LSTDIR)/*.lst
		rm -f *.map
		rm -f *.obj
		rm -f *.elf
		rm -f *.rom
		rm -f *.eep

