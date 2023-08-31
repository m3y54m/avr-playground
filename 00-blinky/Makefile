SRC_DIR = src
LIB_DIR = $(SRC_DIR)/my
BUILD_DIR = build

# The CC variable typically represents the C compiler that will be used during the
# compilation process. By assigning avr-gcc to CC, it indicates that the avr-gcc
# compiler is being used for compiling the C source files in the project.

CC = avr-gcc

# Compiler Options:

# -Wall:
# This flag enables a set of warning messages during compilation. It helps to
# catch potential issues or suspicious code constructs that might lead to bugs
# or undefined behavior.

# -Os:
# This flag optimizes the generated code for size. It instructs the compiler to
# perform various optimizations that reduce the code size, although it may have a
# slight impact on execution speed. This is often desirable for resource-constrained
# microcontrollers where minimizing code size is important.

# -mmcu=atmega328p:
# This flag specifies the target microcontroller for which the code is being compiled.
# In this case, it is set to the Atmega328P microcontroller. The compiler uses this
# information to set the appropriate processor-specific options and generate code
# tailored for the specified microcontroller.

# -std=c99:
# This flag sets the C language standard to C99. It instructs the compiler to use the C99
# standard for language features and syntax. The C99 standard introduced several new
# features and improvements over previous versions of the C language.

CFLAGS = -Wall -Os -mmcu=atmega328p -std=c99

all: program.hex

# create the build directory
$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

# create the object file for main.c
main.o: $(BUILD_DIR)
	avr-gcc $(CFLAGS) -o $(BUILD_DIR)/main.o -c $(SRC_DIR)/main.c

# build the program in hex format for Atmega328P
program.hex: $(BUILD_DIR) main.o
	avr-gcc $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/program.elf $(BUILD_DIR)/main.o
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/program.elf $(BUILD_DIR)/program.hex
	avr-size --format=avr --mcu=atmega328p $(BUILD_DIR)/program.elf

# upload the built program (hex file) to Atmega328P using USBasp
upload: program.hex
	avrdude -c usbasp-clone -p m328p -U flash:w:$(BUILD_DIR)/program.hex

# Remove build directory with all built files
clean:
	rm -rf $(BUILD_DIR) 
