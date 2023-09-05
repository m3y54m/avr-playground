/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND

       (PB5) <--- LED
PCINT8 (PC0) <--- Push Button

*/

// Project common config and definitions
// NOTE: put this before other includes
#include "my/project_config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/atomic.h>

// Helper macros for bit manipulation
#define bit_set(value, bit) ((value) |= (1 << (bit)))
#define bit_clear(value, bit) ((value) &= ~(1 << (bit)))
#define bit_toggle(value, bit) ((value) ^= (1 << (bit)))
#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_write(value, bit, bitvalue) ((bitvalue) ? bit_set(value, bit) : bit_clear(value, bit))

uint8_t just_woke_up;

// Any change on PCINT8 (PC0) triggers PCINT1_vect interrupt
ISR(PCINT1_vect)
{
  just_woke_up = 1;
}

void LED_init(void)
{
  // Config PB5 as output (LED)
  bit_set(DDRB, PB5);
}

void BUTTON_init(void)
{
  // Enable pin change interrupt on PCINT8 (PC0)
  PCMSK1 |= (1 << PCINT8);
  PCICR |= (1 << PCIE1);
  // Config PCINT8 (PC0) as input
  bit_clear(PORTC, PC0);
  // Config PC0 as input (all pins are configures as inputs by default at startup)
  // Enable internal pull-up resistor on push-button pin
  bit_set(PORTC, PC0);
}

int main(void)
{
  LED_init();
  BUTTON_init();

  // Get initial state of th LED from the button pin
  bit_write(PORTB, PB5, !bit_read(PINC, PC0));

  // Configure sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Globally enable interrupts
  sei();

  while (1)
  {
    // Go to sleep mode
    sleep_mode();

    // After waking up ISR is called and then the program continues from here

    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      // wait long enough until the start-up time required for the MCU has elapsed
      _delay_ms(10);
    }

    // This part of the code runs only once after waking up
    // So MCU consumes less power
    if (just_woke_up)
    {
      // If button is pressed (pulled to ground) turn on LED
      if (bit_read(PINC, PC0) == 0)
      {
        bit_set(PORTB, PB5);
      }
      else // otherwise turn it off
      {
        bit_clear(PORTB, PB5);
      }

      just_woke_up = 0;
    }
  }

  return 0;
}
