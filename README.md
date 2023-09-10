# AVR Programming Playground 

This is my playground for AVR programming where I rewrite my old AVR programs and practice bare-metal embedded programming skills. It could be beneficial for you as well.

To prepare your build environment first read this tutorial:

- [Getting started with AVR programming](https://github.com/m3y54m/start-avr)

All of the programs in this repository are written for Atmega328 on Arduino Uno / Nano but **without using Arduino Core** and just in bare-metal.
I used the Arduino bootloader on Atmega328 and AVRDUDE to write my programs on Atmega328. So there is no need 
for any external programmer.

![Arduino-uno-pinout](https://github.com/m3y54m/avr-playground/assets/1549028/a49ab57c-9edf-4b41-9790-9e601848c7b7)

![Atmega328P Pinout](https://github.com/m3y54m/start-avr/assets/1549028/7c222c32-0c19-44ef-be49-052d2cd0fc68)

Note that **Atmega328** is an old device and ***NOT RECOMMENDED FOR NEW DESIGNS***. I am using it only to practice programming.

Almost all projects are configured for 16MHz external crystal on Arduino board. You can see each project's configuration in [`src/my/project_config.h`](00-blinky/src/my/project_config.h)

## [Blinky](00-blinky)

Blinky is the "Hello World!" of embedded systems :)

## [Serial Port](01-serial-port)

Redirect STDIO streams to UART0 input and output so that you can write to serial port using `printf()` and read using `scanf()`

**Note 1: The implementation of printf() and scanf() is about 5KB in size, which is a problem for memory constrained systems.**

**Note 2: that if you are using Arduino Uno / Nano board, you do not need any external serial port to communicate with Atmega328, you can use the Arduino USB serial port to test the program. Just connect your board using USB port to your PC.**

## [Precise Timing](02-precise-timing)

Implement precise timing functions `millis()`, `micros()`, `precise_delay_ms()`, `precise_delay_us()`

## [HY-SRF05 Ultrasonic Sensor](03-hy-srf05-sensor)

Test HY-SRF05 ultrasonic sensor using my `precise_timing` library.

![image](https://github.com/m3y54m/avr-playground/assets/1549028/36f09749-cb5a-435f-aae5-30f6b80a32a2)

- [SRF05 - Ultra-Sonic Ranger](http://www.robot-electronics.co.uk/htm/srf05tech.htm)

## [Quadrature Rotary Encoder](04-rotary-encoder)

![image](https://github.com/m3y54m/avr-playground/assets/1549028/15d60fc0-2a20-49a8-9574-ae9115776be7)

Test a 20PPR incremental (quadrature) rotary encoder. A quadrature encoder gives 4x resolution using this method.
External interrupts (INT0 and INT1) are used for decoding the encoder signals.

![image](https://github.com/m3y54m/avr-playground/assets/1549028/aa2f9b30-1cdc-42c7-b92f-8844fbea0968)

- [Optical Quadrature Encoders](https://www.quantumdev.com/why-use-an-optical-quadrature-encoder-for-a-motor-encoder/)
- [How Rotary Encoder Works and Interface It with Arduino](https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/)

## [Quadrature Rotary Encoder Emulator](05-rotary-encoder-emulator)

Emulate a 20PPR incremental (quadrature) rotary encoder using TIMER0 and TIMER2 outputs and decode the generated signals. Decoded speed should be 91 RPM (122 pulses per second).

- [Output 2 PWM waveforms with 90 degree phase shift](https://electronics.stackexchange.com/questions/174656/output-2-pwm-waveforms-with-90-degree-phase-shift)

## [Measure VCC using ADC](06-adc-measure-vcc)

Use ADC to measure VCC voltage without using any I/O pin or any external component.

- [AN2447: Measure VCC/Battery Voltage Without Using I/O Pin on tinyAVR and megaAVR](https://ww1.microchip.com/downloads/en/Appnotes/00002447A.pdf)
- [AVR126: ADC of megaAVR® in Single-Ended Mode](http://ww1.microchip.com/downloads/en/AppNotes/00002538A.pdf)

## [Precise Capacitance Meter](07-precise-capacitance-meter)

Capacitance meter for 470uF to 18pF range with the highest accuracy for small capacitors.

- [How to Make an Arduino Capacitance Meter](https://www.circuitbasics.com/how-to-make-an-arduino-capacitance-meter/)
- [Capacitive Voltage Divider](http://www.learningaboutelectronics.com/Articles/Capacitive-voltage-divider.php)
- [RC Charging Circuit](https://www.electronics-tutorials.ws/rc/rc_1.html)

## [Sleep Mode](08-sleep-mode)

Low power control of an LED using a button. Exits Sleep on button press, turns on an LED and goes back to Sleep. On button release, exits Sleep, turns off
the LED and goes back to Sleep.

- [AVR® Low Power Sleep Modes](https://microchipdeveloper.com/8avr:avrsleep)
- [Getting Started with GPIO](https://ww1.microchip.com/downloads/en/Appnotes/90003229A.pdf)
- [AVR®: Using Pin Change Interrupts](https://microchipdeveloper.com/8avr:pin-change-interrupts)
- [<avr/interrupt.h>: Interrupts](https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html)

## [Watchdog Timer (Software Reset)](09-watchdog-timer)

Testing software reset using watchdog timer. Configured watchdog timer for a timeout of 4 seconds. Reset the watchdog timer 5 times when it reaches 3.9 seconds to avoid triggering a MCU reset. Eventually, triggered the MCU reset by allowing the watchdog timer to reach its timeout.

- [Software Reset](https://microchip.my.site.com/s/article/Software-Reset)
- [AVR® Watchdog Timer](https://microchipdeveloper.com/8avr:avrwdt)
- [<avr/wdt.h>: Watchdog timer handling](https://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html)
- [AVR132: Using the Enhanced Watchdog Timer](https://www.microchip.com/en-us/application-notes/avr132)

## [Reset Sources](10-reset-sources)

Determining the reset source (cause) of the MCU during startup.

- [AVR Reset Sources](https://microchipdeveloper.com/8avr:avrreset)
- [AVR® Reset Sources Example Project](https://microchipdeveloper.com/8avr:resetexample)

## Resources

- [ATmega48A/PA/88A/PA/168A/PA/328/P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf)
- [Introduction to Bare Metal Programming in Arduino Uno](https://www.hackster.io/milanistef/introduction-to-bare-metal-programming-in-arduino-uno-f3e2b4)
- [Developing with the 8-bit AVR® MCU](https://www.microchip.com/en-us/education/developer-help/learn-products/mcu-mpu/8bit-avr)
- [Fundamentals of Microcontrollers - Arduino bare-metal breakdown](https://www.youtube.com/playlist?list=PLNyfXcjhOAwOF-7S-ZoW2wuQ6Y-4hfjMR)
- [Using Standard IO streams in AVR GCC](https://embedds.com/using-standard-io-streams-in-avr-gcc/)
- [avr-libc - <stdio.h>: Standard IO facilities](https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#stdio_without_malloc)
- [How do I create a static library for the AVR Atmega328p?](https://electronics.stackexchange.com/questions/27325/how-do-i-create-a-static-library-for-the-avr-atmega328p)
- [avr-libc - A simple project](https://www.nongnu.org/avr-libc/user-manual/group__demo__project.html)
- [avr-libc - How to Build a Library ](https://www.nongnu.org/avr-libc/user-manual/library.html)
- [avr-millis-function](https://github.com/monoclecat/avr-millis-function)
- [Nested Interrupts](https://ucexperiment.wordpress.com/2013/05/20/nested-interrupts/)
- [avr_insights](https://github.com/MicrochipTech/avr_insights)

