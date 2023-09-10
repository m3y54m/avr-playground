/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND
*/

// Project common config and definitions
// NOTE: put this before other includes
#include "my/project_config.h"

#include <avr/pgmspace.h>
#include <util/delay.h>
#include "my/serial_port.h"

// Helper macros for bit manipulation
#define bit_set(value, bit) ((value) |= (1 << (bit)))
#define bit_clear(value, bit) ((value) &= ~(1 << (bit)))
#define bit_toggle(value, bit) ((value) ^= (1 << (bit)))
#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_write(value, bit, bitvalue) ((bitvalue) ? bit_set(value, bit) : bit_clear(value, bit))

int main(void)
{
  // Note: For this application you need to remove Arduino bootloader from your MCU.
  //       That is because bootloader is executed before this program
  //       and it first reads MCUSR register to check the reset source and clears
  //       all the bits of MCUSR register.
  //       Then this program runs and it will read 0x00 from MCUSR register.
  //       You will need an external programmer (USBasp, AVRISP MKII, FT232H, etc.) to remove
  //       bootloader from your MCU.

  // Get initial value of MCU status register
  char status_register = MCUSR;

  // Clear MCUSR register (Because if we don't clear flags, as long as MCU is powered on reset flags won't be cleared)
  MCUSR = 0;

  // initialize uart0 to use printf()
  serial_port_init();

  // Wait some time for the user to get ready to see the serial output
  _delay_ms(1000);

  // print project config to serial port (USART)
  printf("\r\nF_CPU = %lu\r\n", F_CPU);
  printf("BAUD = %lu\r\n", BAUD);

  printf("MCUSR = 0x%02X\r\n", status_register);

  if (bit_read(status_register, WDRF))
  {
    printf_P(PSTR("Watchdog Reset\r\n"));
  }

  if (bit_read(status_register, BORF))
  {
    printf_P(PSTR("Brown-out Reset\r\n"));
  }

  if (bit_read(status_register, EXTRF))
  {
    printf_P(PSTR("External Reset\r\n"));
  }

  if (bit_read(status_register, PORF))
  {
    printf_P(PSTR("Power-on Reset\r\n"));
  }

  // if none of the flags are set, then the reset source is unknown
  // Theoretically, this should never happen
  if (status_register & 0x0f == 0)
  {
    printf_P(PSTR("Unknown Reset Source\r\n"));
  }

  return 0;
}
