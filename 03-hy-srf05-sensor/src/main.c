/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX

      PB5 ---> LED

      PB1 ---> HY-SRF05 Trigger
      PB2 <--- HY-SRF05 Echo
*/

// Project common config and definitions
// put this before other includes
#include "my/project_config.h"

#include <avr/io.h>
#include <util/delay.h>
#include "my/serial_port.h"
#include "my/precise_timing.h"

int main(void)
{
  // initialize uart0 to use printf()
  serial_port_init();

  // print project config to serial port (USART)
  printf("F_CPU = %lu\r\n", F_CPU);
  printf("BAUD = %lu\r\n", BAUD);

  // initialize timer1 to enable precise_timing functions
  precise_timing_init();

  sei(); // Enables interrupts by setting the global interrupt mask

  // LED pin as output
  DDRB |= (1 << PB5);

  // Trigger pin as output
  DDRB |= (1 << PB1);
  // Echo pin as input (default)

  while (1)
  {
    // Output a 10us "Trigger" pulse to SRF05
    PORTB |= (1 << PB1); // Trigger = 1
    precise_delay_us(10);
    PORTB &= ~(1 << PB1); // Trigger = 0

    // Wait for the rising edge of "Echo" input signal 
    loop_until_bit_is_set(PINB, PB2); // while (Echo == 0);
    const unsigned int start_us = micros();

    PORTB ^= (1 << PB5); // Toggle the LED

    // Wait for the falling edge of "Echo" input signal 
    loop_until_bit_is_clear(PINB, PB2); // while (Echo == 1);
    const unsigned int stop_us = micros();

    PORTB ^= (1 << PB5); // Toggle the LED

    unsigned int duration = stop_us - start_us;
    float distance = duration / 58.0f;

    printf("\r\nPulse Duration: %u us\r\n", duration);
    printf("Distance: %.2f cm\r\n", distance);

    _delay_ms(1000);
  }

    return 0;
}