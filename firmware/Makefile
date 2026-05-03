# ==========================================================================
# Makefile — ATtiny85 USB LED Controller
#
# Targets:
#   make          — compile firmware
#   make flash    — upload via USBasp programmer
#   make fuse     — set fuses for 16.5 MHz PLL clock
#   make clean    — remove build artefacts
#
# Prerequisites: avr-gcc, avr-libc, avrdude
# ==========================================================================

# ---- MCU & Clock --------------------------------------------------------
MCU          = attiny85
F_CPU        = 16500000
PROGRAMMER   = usbasp

# ---- Toolchain ----------------------------------------------------------
CC           = avr-gcc
OBJCOPY      = avr-objcopy
OBJDUMP      = avr-objdump
SIZE         = avr-size
AVRDUDE      = avrdude

# ---- Fuse settings (ATtiny85 @ 16.5 MHz PLL, no CKDIV8) ----------------
#   Low:  0xE1  — PLL clock, fast start-up, no CKDIV8
#   High: 0xDD  — SPI enabled, BOD @ 2.7 V
#   Ext:  0xFE  — self-programming enabled
FUSE_LOW     = 0xE1
FUSE_HIGH    = 0xDD
FUSE_EXT     = 0xFE

# ---- Sources ------------------------------------------------------------
TARGET       = main
VUSB_DIR     = usbdrv

SRC          = main.c $(VUSB_DIR)/usbdrv.c $(VUSB_DIR)/usbdrvasm.S $(VUSB_DIR)/oddebug.c
OBJ          = $(SRC:.c=.o)
OBJ         := $(OBJ:.S=.o)

# ---- Compiler flags -----------------------------------------------------
CFLAGS       = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL \
               -Os -std=gnu99 -Wall -Wextra \
               -funsigned-char -funsigned-bitfields \
               -fpack-struct -fshort-enums \
               -I. -I$(VUSB_DIR)

ASFLAGS      = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -x assembler-with-cpp -I. -I$(VUSB_DIR)
LDFLAGS      = -mmcu=$(MCU)

# ---- Rules --------------------------------------------------------------

all: $(TARGET).hex size

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $<

size: $(TARGET).elf
	@echo "---- Firmware size ----"
	$(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(TARGET).hex:i

fuse:
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) \
	  -U lfuse:w:$(FUSE_LOW):m \
	  -U hfuse:w:$(FUSE_HIGH):m \
	  -U efuse:w:$(FUSE_EXT):m

clean:
	rm -f *.o *.elf *.hex $(VUSB_DIR)/*.o

.PHONY: all flash fuse clean size
