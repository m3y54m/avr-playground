/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND
*/

// Project common config and definitions
// NOTE: put this before other includes
#include "my/project_config.h"

#include <avr/pgmspace.h>
#include "my/serial_port.h"

int main(void)
{
  // initialize uart0 to use printf()
  serial_port_init();

  printf("Hello World!\r\n");

  float pi = 3.14159265359;

  printf("\r\nThis is a floating point number: %f\r\n", pi);

  while (1)
  {
    // Print a string stored in program memory (flash)
    printf_P(PSTR("\r\nPress any key:\r\n"));

    // scan standard stream (UART0)
    char input_byte;
    scanf("%c", &input_byte);

    // print scanned character and its code
    printf("\r\nYou pressed: %c (ASCII Code = %u)\r\n", input_byte, input_byte);
  }

    return 0;
}
