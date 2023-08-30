// Define these macros before including any libraries
#define F_CPU 8000000UL
#define BAUD 9600UL

/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/setbaud.h>
#include <stdio.h>

// UART0 basic functions
void uart0_init(void);
int uart0_write_byte(int write_byte, FILE *stream);
int uart0_read_byte(FILE *stream);
// UART0 I/O stream pointer
FILE uart0_io_stream = FDEV_SETUP_STREAM(uart0_write_byte, uart0_read_byte, _FDEV_SETUP_RW);

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

void uart0_init(void)
{
  // requires BAUD
  UBRR0H = UBRRH_VALUE; // defined in setbaud.h
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);   // Enable UART0 both transmitter and receiver
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 data bits, no parity, 1 stop bit

  // Assign our UART0 stream to standard I/O streams
  stdin = stdout = &uart0_io_stream;
}

int uart0_write_byte(int write_byte, FILE *stream)
{
  loop_until_bit_is_set(UCSR0A, UDRE0); // Wait for empty transmit buffer
  UDR0 = write_byte;                    // write register value (write one byte to UART0)
  return 0;
}

int uart0_read_byte(FILE *stream)
{
  loop_until_bit_is_set(UCSR0A, RXC0); // Wait for incoming data
  return UDR0;                         // return register value (read one byte from UART0)
}