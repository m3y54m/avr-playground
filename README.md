# AVR Programming Playground 

This is my playground for AVR programming where I rewrite my old AVR programs and practice bare-metal embedded programming skills. It could be beneficial for you as well.

To prepare your build environment first read this tutorial:

- [Getting started with AVR programming](https://github.com/m3y54m/start-avr)

All of the programs in this repository are written for Atmega328 on Arduino Uno / Nano but **without using Arduno Core and libraries**.
I used the Arduino bootloader on Atmega328 and AVRDUDE to write my programs on Atmega328. So there is no need 
for any external programmer.

![Arduino-uno-pinout](https://github.com/m3y54m/avr-playground/assets/1549028/a49ab57c-9edf-4b41-9790-9e601848c7b7)

Note that **Atmega328** is an old device and ***NOT RECOMMENDED FOR NEW DESIGNS***. I am using it only to practice programming.

Almost all projects are configured for 16MHz external crystal on Arduino board. You can see each project's configuration in [`src/my/project_config.h`](00-blinky/src/my/project_config.h)

## [Blinky](00-blinky)

Blinky is the "Hello World!" of embedded systems :)

## [Serial Port](01-serial-port)

Redirect STDIO streams to UART0 input and output so that you can write to serial port using `printf()` and read using `scanf()`

## [Precise Timing](02-precise-timing)

Implement precise timing functions `millis()`, `micros()`, `precise_delay_ms()`, `precise_delay_us()`

## [HY-SRF05 Ultrasonic Sensor](03-hy-srf05-sensor)

Test HY-SRF05 ultrasonic sensor using my `precise_timing` library.

![motion-pinout](https://github.com/m3y54m/avr-playground/assets/1549028/b5a98cea-d2ce-4b3a-b8fd-65c0a5b4f2f7)

- [SRF05 - Ultra-Sonic Ranger](http://www.robot-electronics.co.uk/htm/srf05tech.htm)

## Resources

- [ATmega48A/PA/88A/PA/168A/PA/328/P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf)
- [Introduction to Bare Metal Programming in Arduino Uno](https://www.hackster.io/milanistef/introduction-to-bare-metal-programming-in-arduino-uno-f3e2b4)
- [Using Standard IO streams in AVR GCC](https://embedds.com/using-standard-io-streams-in-avr-gcc/)
- [avr-libc - <stdio.h>: Standard IO facilities](https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#stdio_without_malloc)
- [How do I create a static library for the AVR Atmega328p?](https://electronics.stackexchange.com/questions/27325/how-do-i-create-a-static-library-for-the-avr-atmega328p)
- [avr-libc - A simple project](https://www.nongnu.org/avr-libc/user-manual/group__demo__project.html)
- [avr-libc - How to Build a Library ](https://www.nongnu.org/avr-libc/user-manual/library.html)
- [avr-millis-function](https://github.com/monoclecat/avr-millis-function)

