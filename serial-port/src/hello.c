// Default clock source is internal 8MHz RC oscillator
#define F_CPU 8000000UL
#define BAUD 9600UL

#include <avr/io.h>
#include <util/setbaud.h>

/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND
*/

void initUSART(void);
void transmitByte(uint8_t data);
uint8_t receiveByte(void);
void printString(const char myString[]);

int main(void)
{
  char serialCharacter;

  // -------- Inits --------- //
  initUSART();

  printString("Hello World!\r\n"); /* to test */

  // ------ Event loop ------ //
  while (1)
  {
    serialCharacter = receiveByte();
    transmitByte(serialCharacter);
    /* display ascii/numeric value of character */
  }

  return 0;
}

void initUSART(void)
{
  /* requires BAUD */
  UBRR0H = UBRRH_VALUE; /* defined in setbaud.h */
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif
  /* Enable USART transmitter/receiver */
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8 data bits, 1 stop bit */
}

void transmitByte(uint8_t data)
{
  /* Wait for empty transmit buffer */
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = data; /* send data */
}

uint8_t receiveByte(void)
{
  loop_until_bit_is_set(UCSR0A, RXC0); /* Wait for incoming data */
  return UDR0;                         /* return register value */
}

// Example of a useful printing command
void printString(const char myString[])
{
  uint8_t i = 0;
  while (myString[i])
  {
    transmitByte(myString[i]);
    i++;
  }
}