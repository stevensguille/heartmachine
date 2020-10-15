###############################################################################
# Makefile for the project thm
###############################################################################

## General Flags
PROJECT = thm
MCU = atmega328p
TARGET = thm.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=thm.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = attractor_patch.o can.o can_command.o can_debug.o delay.o main.o mcp2515.o mstimer.o patches.o pulse_patch.o qtouch.o serial_command.o serial_console.o spi.o thm.o thm_node.o tlc_demo.o Tlc5940.o uart.o utility.o attractor_patch.o can.o can_command.o can_debug.o defines.o delay.o mcp2515.o mcp2515_bittime.o mcp2515_defs.o mstimer.o patches.o pulse_patch.o qtouch.o revision.o serial_command.o serial_console.o spi.o thm_node.o tlc_config.o tlc_defines.o tlc_demo.o Tlc5940.o touch_api.o touch_qt_config.o uart.o utility.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) thm.hex thm.eep thm.lss size

## Compile
attractor_patch.o: ../attractor_patch.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

can.o: ../can.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

can_command.o: ../can_command.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

can_debug.o: ../can_debug.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

delay.o: ../delay.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mcp2515.o: ../mcp2515.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mstimer.o: ../mstimer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

patches.o: ../patches.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

pulse_patch.o: ../pulse_patch.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

qtouch.o: ../qtouch.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

serial_command.o: ../serial_command.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

serial_console.o: ../serial_console.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi.o: ../spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

thm.o: ../thm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

thm_node.o: ../thm_node.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tlc_demo.o: ../tlc_demo.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Tlc5940.o: ../Tlc5940.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: ../uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

utility.o: ../utility.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

attractor_patch.o: ../attractor_patch.h
can.o: ../can.h
can_command.o: ../can_command.h
can_debug.o: ../can_debug.h
defines.o: ../defines.h
delay.o: ../delay.h
mcp2515.o: ../mcp2515.h
mcp2515_bittime.o: ../mcp2515_bittime.h
mcp2515_defs.o: ../mcp2515_defs.h
mstimer.o: ../mstimer.h
patches.o: ../patches.h
pulse_patch.o: ../pulse_patch.h
qtouch.o: ../qtouch.h
revision.o: ../revision.h
serial_command.o: ../serial_command.h
serial_console.o: ../serial_console.h
spi.o: ../spi.h
thm_node.o: ../thm_node.h
tlc_config.o: ../tlc_config.h
tlc_defines.o: ../tlc_defines.h
tlc_demo.o: ../tlc_demo.h
Tlc5940.o: ../Tlc5940.h
touch_api.o: ../touch_api.h
touch_qt_config.o: ../touch_qt_config.h
uart.o: ../uart.h
utility.o: ../utility.h
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
	-rm -rf $(OBJECTS) thm.elf dep/* thm.hex thm.eep thm.lss thm.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

