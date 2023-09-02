#include "serial_port.h"

#include "project_config.h"
#include <avr/io.h>
#include <util/setbaud.h>

static int uart0_write_byte(int write_byte, FILE *stream);
static int uart0_read_byte(FILE *stream);
static FILE uart0_stdin = FDEV_SETUP_STREAM(NULL, uart0_read_byte, _FDEV_SETUP_READ);
static FILE uart0_stdout = FDEV_SETUP_STREAM(uart0_write_byte, NULL, _FDEV_SETUP_WRITE);

void serial_port_init(void)
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
  stdin = &uart0_stdin;
  stdout = &uart0_stdout;
}

static int uart0_write_byte(int write_byte, FILE *stream)
{
  loop_until_bit_is_set(UCSR0A, UDRE0); // Wait for empty transmit buffer
  UDR0 = write_byte;                    // write register value (write one byte to UART0)
  return 0;
}

static int uart0_read_byte(FILE *stream)
{
  loop_until_bit_is_set(UCSR0A, RXC0); // Wait for incoming data
  return UDR0;                         // return register value (read one byte from UART0)
}