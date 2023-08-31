# My AVR Programming Playground 

This is my playground repository for AVR programming where I rewrite my old AVR programs and practice basic embedded programming skills.

- Target microcontroller: **Atmega328P**
- Main clock frequency: **8Mhz (Internal RC oscillator)**

To prepare your build environment read this tutorial:

- [Getting started with AVR programming](https://github.com/m3y54m/start-avr)

## [Serial Port](01-serial-port)

Redirect STDIO streams to UART0 input and output so that you can write to serial port using printf() and read using scanf()

## [Precise Timing](02-precise-timing)

Implement precise timing functions millis(), micros(), precise_delay_ms(), precise_delay_us()

## [HY-SRF05 Ultrasonic Sensor](03-hy-srf05-sensor)

Test HY-SRF05 ultrasonic sensor using my `precise_timing` library.

## Resources

- [ATmega48A/PA/88A/PA/168A/PA/328/P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf)
- [Using Standard IO streams in AVR GCC](https://embedds.com/using-standard-io-streams-in-avr-gcc/)
- [avr-libc - <stdio.h>: Standard IO facilities](https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html)
- [How do I create a static library for the AVR Atmega328p?](https://electronics.stackexchange.com/questions/27325/how-do-i-create-a-static-library-for-the-avr-atmega328p)
- [avr-libc - A simple project](https://www.nongnu.org/avr-libc/user-manual/group__demo__project.html)
- [avr-libc - How to Build a Library ](https://www.nongnu.org/avr-libc/user-manual/library.html)
- [avr-millis-function](https://github.com/monoclecat/avr-millis-function)
- [SRF05 - Ultra-Sonic Ranger](http://www.robot-electronics.co.uk/htm/srf05tech.htm)