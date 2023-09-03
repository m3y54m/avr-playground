/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX

      PB5 ---> LED

OC0A (PD6) ---> Rotary Encoder A
OC2A (PB3) ---> Rotary Encoder B

      PD2 <--- Rotary Encoder A
      PD3 <--- Rotary Encoder B
*/

// Project common config and definitions
// put this before other includes
#include "my/project_config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "my/serial_port.h"
#include "my/precise_timing.h"
#include <stdbool.h>

// Encoder resolution: 20 PPR
#define PULSES_PER_ROUND 20
// Quadrature encoder resolution
// Each rising or falling edge (2x) of both signals (2x) considered as a position step
#define QUADRATURE_PULSES_PER_ROUND (4 * PULSES_PER_ROUND)
// Encoder signals
#define SIGNAL_A ((PIND >> PD2) & 0x01)
#define SIGNAL_B ((PIND >> PD3) & 0x01)
// Interval for speed calculation in milliseconds
#define CALCULATION_INTERVAL_MS 500

bool last_signal_a = 0;
bool current_signal_b = 0;
// position_vector shows the movement of the encoder with its direction
signed int position_vector = 0;

int main(void)
{
  // initialize uart0 to use printf()
  serial_port_init();

  // print project config to serial port (USART)
  printf("F_CPU = %lu\r\n", F_CPU);
  printf("BAUD = %lu\r\n", BAUD);

  // initialize timer1 to enable precise_timing functions
  precise_timing_init();

  // Config LED pin as output
  DDRB |= (1 << PB5);

  //*********** Quadrature encoder emulator *************

  const bool encoder_emulator_direction = 0;

  // Quadrature encoder emulator pins as output
  DDRD |= (1 << PD6);
  DDRB |= (1 << PB3);

  // Configure Timer0 A output in Toggle Mode
  // COM0A: 01
  // CTC Waveform Generation Mode
  // TOP = OCRA
  // Note clock is left off for now
  // WGM0: 010
  TCCR0A = (1 << COM0A0) | (1 << WGM01);
  // CS0: 101 => Prescaler = 1024
  TCCR0B |= (1 << CS02) | (1 << CS00);

  OCR0A = 255;

  // Configure Timer2 A output in Toggle Mode
  // COM2A: 01
  // CTC Waveform Generation Mode
  // TOP = OCRA
  // Note clock is left off for now
  // WGM2: 010
  TCCR2A = (1 << COM2A0) | (1 << WGM21);
  // CS2: 111 => Prescaler = 1024
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

  OCR2A = 255;

  // Both timers have the same configurations
  // F_CPU = 16,000,000 Hz
  // Prescaler = 1024
  // f_timer = 16,000,000 / 1024 = 15,625 Hz
  // (OCRxA = 255 , Toggle Mode) => f_OCxA = 15,625 / ((255 + 1) * 2) = 30.51 Hz
  
  // Add 90 degrees phase shift
  if (encoder_emulator_direction)
  {
    TCNT0 = 0;
    TCNT2 = 127;
  }
  else
  {
    TCNT0 = 127;
    TCNT2 = 0;
  }

  //*****************************************************

  // *** External Interrupt(s) initialization ***
  // External Interrupt Mask Register
  // INT0: enabled
  // INT1: enabled
  EIMSK |= (1 << INT1) | (1 << INT0);
  // External Interrupt Control Register A
  // ISC0: 01 => Any logical change on INT0 generates an interrupt request
  // ISC1: 01 => Any logical change on INT1 generates an interrupt request
  EICRA = (0 << ISC11) | (1 << ISC10) | (0 << ISC01) | (1 << ISC00);
  // External Interrupt Flag Register
  // INT0 Flag: enabled
  // INT1 Flag: enabled
  EIFR = (1 << INTF1) | (1 << INTF0);

  sei(); // Enables interrupts by setting the global interrupt mask

  signed int last_position_vector;

  unsigned long last_millis = millis();

  while (1)
  {
    // Create a fixed interval equal to CALCULATION_INTERVAL_MS in milliseconds
    if (millis() - last_millis >= CALCULATION_INTERVAL_MS)
    {
      last_millis = millis();

      // save position_vector
      last_position_vector = position_vector;
      // reset position_vector
      position_vector = 0;
      // calculate pulses per interval
      signed int speed_vector = position_vector - last_position_vector;

      // Convert speed to PPS (pulses per second)
      signed long speed_vector_pps = speed_vector * 1000 / CALCULATION_INTERVAL_MS;

      // Convert speed to RPM (rounds per minute)
      signed long speed_vector_rpm = speed_vector_pps * 60 / QUADRATURE_PULSES_PER_ROUND;

      printf("Speed: %ld RPM (%ld pulses per second)\r\n\r\n", speed_vector_rpm, speed_vector_pps);
    }
  }

  return 0;
}

// External Interrupt 0 service routine
ISR(INT0_vect)
{
  if (current_signal_b != last_signal_a)
    position_vector++;
  else
    position_vector--;

  last_signal_a = SIGNAL_A;
}

// External Interrupt 1 service routine
ISR(INT1_vect)
{
  current_signal_b = SIGNAL_B;

  if (current_signal_b != last_signal_a)
    position_vector++;
  else
    position_vector--;
}