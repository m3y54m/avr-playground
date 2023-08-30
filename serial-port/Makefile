SRC_DIR = src
LIB_DIR = $(SRC_DIR)/my
BUILD_DIR = build
CC = avr-gcc
CFLAGS = -Wall -Os -mmcu=atmega328p -DF_CPU=8000000UL

all: program.hex

# create the build directory
$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

# create the static library for uart.c
libuart.a: $(BUILD_DIR)
	avr-gcc $(CFLAGS) -o $(BUILD_DIR)/uart.o -c $(LIB_DIR)/uart.c
	avr-ar rcs $(BUILD_DIR)/libuart.a $(BUILD_DIR)/uart.o

# create the object file for main.c
main.o: $(BUILD_DIR)
	avr-gcc $(CFLAGS) -o $(BUILD_DIR)/main.o -c $(SRC_DIR)/main.c

# build the program in hex format for Atmega328P
program.hex: $(BUILD_DIR) main.o libuart.a
	avr-gcc $(CFLAGS) -o $(BUILD_DIR)/program.elf $(BUILD_DIR)/main.o $(BUILD_DIR)/libuart.a
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/program.elf $(BUILD_DIR)/program.hex

# upload the built program (hex file) to Atmega328P using USBasp
upload: program.hex
	avrdude -c usbasp-clone -p m328p -U flash:w:$(BUILD_DIR)/program.hex

# Remove build directory with all built files
clean:
	rm -rf $(BUILD_DIR) 
