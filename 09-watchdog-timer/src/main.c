/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND
*/

// Project common config and definitions
// NOTE: put this before other includes
#include "my/project_config.h"

#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "my/serial_port.h"
#include "my/precise_timing.h"

// Helper macros for bit manipulation
#define bit_set(value, bit) ((value) |= (1 << (bit)))
#define bit_clear(value, bit) ((value) &= ~(1 << (bit)))
#define bit_toggle(value, bit) ((value) ^= (1 << (bit)))
#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_write(value, bit, bitvalue) ((bitvalue) ? bit_set(value, bit) : bit_clear(value, bit))

int main(void)
{
  // initialize uart0 to use printf()
  serial_port_init();

  // print project config to serial port (USART)
  printf("\r\nF_CPU = %lu\r\n", F_CPU);
  printf("BAUD = %lu\r\n", BAUD);

  printf_P(PSTR("MCU starting up...\r\n"));

  // initialize timer1 to enable precise_timing functions
  precise_timing_init();

  // configure watchdog timer for 4 seconds timeout
  wdt_enable(WDTO_4S);

  printf_P(PSTR("Configured watchdog timer for a timeout of 4 seconds\r\n"));

  unsigned int wdt_reset_postpone_counter = 0;
  unsigned long restart_time;
  unsigned long elapsed_time;

  while (1)
  {
    // Postpone the watchdog reset 5 times
    if (wdt_reset_postpone_counter < 5)
    {
      // Postpone the watchdog reset if 3.9 seconds is passed
      if (millis() - restart_time >= 3900)
      {
        // Postpone the watchdog reset (restart the timer of watchdog)
        wdt_reset();
        wdt_reset_postpone_counter++;

        // precise time measurement using millis()
        restart_time = millis();

        printf("\r\nPostponed the watchdog reset (%d of 5)\r\n", wdt_reset_postpone_counter);
      }
    }

    printf("Elapsed time from last restart: %lu ms\r\n", millis() - restart_time);

    _delay_ms(200);
  };

  return 0;
}
