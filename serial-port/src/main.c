/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND
*/

#include <avr/pgmspace.h>
#include "my/uart.h"

int main(void)
{
  // Initialize UART0
  uart0_init();

  printf("Hello World!\r\n");

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
}
