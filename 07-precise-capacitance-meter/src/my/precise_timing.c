#include "precise_timing.h"

#include "project_config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile unsigned long precise_timing_millis;

/*
NOTE: An unsigned long holds values from 0 to 4,294,967,295 (2^32 - 1).
It will roll over to 0 after reaching its maximum value.

The `volatile` keyword indicates that the variable `timer1_millis`
can be modified unexpectedly, and the compiler should not make assumptions
about its value or optimize accesses to it.
*/

// With f_timer1 = 2 Mhz, each timer1 count takes 0.5 microsecond
// when timer1 gets this value, 1ms has passed
static const unsigned int CTC_THRESHOLD = 1999;

ISR(TIMER1_COMPA_vect)
{
  precise_timing_millis++;
}

void precise_timing_init(void)
{
  // Configure Timer1 to count from 0 to 1999 with 2Mhz frequency:
  // WGM1 = 0100 => Enable CTC mode (Clears timer when timer counter equals to the value of OCR1A)
  // CS1  =  010 => Set clock divisor to 8
  TCCR1B |= (1 << WGM12) | (1 << CS11);

  // Assuming that (F_CPU = 16Mhz) => (f_timer1 = 2 Mhz)
  const unsigned int f_timer1 = (F_CPU / 8); // clock divisor = 8

  // high byte first, then low byte
  OCR1AH = (CTC_THRESHOLD >> 8);
  OCR1AL = CTC_THRESHOLD;

  // Enable the compare match interrupt
  TIMSK1 |= (1 << OCIE1A);

  // REMEMBER TO ENABLE GLOBAL INTERRUPTS AFTER THIS WITH `sei()`
}

unsigned long millis(void)
{
  // Ensure this cannot be disrupted
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    return precise_timing_millis;
  }
}

unsigned long micros(void)
{
  return millis() * 1000 + (TCNT1 / 2); // Each counting of TCNT1 takes 0.5 microsecond
}

void precise_delay_ms(unsigned long ms)
{
  const unsigned long start_ms = millis();

  while (1)
  {
    unsigned long current_ms = millis();

    unsigned long diff_ms = current_ms - start_ms;

    if (diff_ms >= ms)
    {
      // desired delay is applied
      break;
    }
  }
}

void precise_delay_us(unsigned char us)
{
  // According to timer1 configurations, TCNT1 counts from 0 to 1999 repeatedly

  const unsigned int start_us = TCNT1;

  while (1)
  {
    unsigned int current_us = TCNT1;

    signed int diff_us = (signed int)current_us - start_us;

    // if ctc overflow has happened after start_us
    if (current_us < start_us)
    {
      diff_us = diff_us + CTC_THRESHOLD + 1;
    }

    diff_us = diff_us / 2; // Each counting of TCNT1 takes 0.5 microsecond

    if (diff_us >= us)
    {
      // desired delay is applied
      break;
    }
  }
}