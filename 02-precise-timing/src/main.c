/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND

      PB0 ---> LED
*/

// Project common config and definitions
// put this before other includes
#include "my/project_config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "my/serial_port.h"
#include "my/precise_timing.h"

void main(void)
{
  // initialize uart0 to use printf()
  serial_port_init();

  // print project config
  printf("F_CPU = %lu\r\n", F_CPU);
  printf("BAUD = %lu\r\n", BAUD);

  // initialize timer1 to enable precise_timing functions
  precise_timing_init();

  sei(); // Enables interrupts by setting the global interrupt mask

  DDRB |= (1 << PB0);

  unsigned long last_millis = millis();

  while (1)
  {
    // Create 1 second delay using precise_delay_us()
    for (int i = 0; i < 10000; i++)
    {
      precise_delay_us(100);
    }

    PORTB ^= (1 << PB0); // Turn on / Turn off the LED
    printf("millis = %lu\r\n", millis());
  }
}