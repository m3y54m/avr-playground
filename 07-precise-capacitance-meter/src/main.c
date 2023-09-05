/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND

ADC2 (PC2) <--- Unknown Capacitor Positive (+) Lead
ADC0 (PC0) <--- Unknown Capacitor Negative (-) Lead

*/

// Project common config and definitions
// NOTE: put this before other includes
#include "my/project_config.h"

#include <avr/io.h>
#include "my/serial_port.h"
#include "my/precise_timing.h"
#include <math.h>

// Helper macros for bit manipulation
#define bit_set(value, bit) ((value) |= (1 << (bit)))
#define bit_clear(value, bit) ((value) &= ~(1 << (bit)))
#define bit_toggle(value, bit) ((value) ^= (1 << (bit)))
#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_write(value, bit, bitvalue) ((bitvalue) ? bit_set(value, bit) : bit_clear(value, bit))

// Capacitor pins
#define CAP_P PC2 // ADC2 (Positive Lead)
#define CAP_N PC0 // ADC0 (Negative Lead)

// Calibrated value for internal stray capacitance of ADC input in Atmega328
#define STRAY_CAP_TO_GND 24.48 // in pico-farad
// Calibrated value for internal pull-up resistor in Atmega328
#define R_PULLUP 34.8f // in kilo-ohm

#define MAX_ADC_VALUE 1023

unsigned int adc_read(unsigned char channel)
{
  // Select ADC channel
  ADMUX = (0xf0 & ADMUX) | channel;
  // start ADC conversion
  ADCSRA |= (1 << ADSC);
  // wait until done
  loop_until_bit_is_clear(ADCSRA, ADSC);
  // get ADC reading (10-bit)
  // NOTE: ADCL should be read first.
  // ADC macro takes care of reading ADC register:
  return ADC;
}

int main(void)
{
  // initialize uart0 to use printf()
  serial_port_init();

  // initialize timer1 to enable precise_timing functions
  precise_timing_init();

  // print project config to serial port (USART)
  printf("F_CPU = %lu\r\n", F_CPU);
  printf("BAUD = %lu\r\n", BAUD);

  //*************** ADC Configuration *****************

  // Select AVCC as ADC reference and 1.1V internal reference voltage as ADC input
  // REFS: 01 => Vref: AVCC with external capacitor at AREF pin
  ADMUX |= (0b01 << REFS0);

  // ADC clock prescaler
  // ADPS: 111 => Prescaler = 128
  ADCSRA |= (0b111 << ADPS0);
  // Enable ADC
  ADCSRA |= (1 << ADEN);

  //***************************************************

  while (1)
  {
    // Config CAP_N as input
    bit_clear(DDRC, CAP_N);

    // Config CAP_P as output
    bit_set(DDRC, CAP_P);
    // Write 1 to CAP_P
    bit_set(PORTC, CAP_P);
    unsigned int adc_reading = adc_read(CAP_N);
    // Write 0 to CAP_P
    bit_clear(PORTC, CAP_P);

    // Config CAP_N as output
    bit_set(DDRC, CAP_N);

    // For capacitors smaller that 1064 pF ~ 1nF
    if (adc_reading < 1000)
    {
      // Capacitive Voltage Divider equation
      float capacitance = (float)adc_reading * STRAY_CAP_TO_GND / (float)(MAX_ADC_VALUE - adc_reading);

      printf("Capacitance Value = %.3f pF (%u)\r\n", capacitance, adc_reading);
    }
    else // For capacitors bigger that 1064 pF ~ 1nF
    {
      precise_delay_ms(1);

      // Config CAP_P as input
      bit_clear(DDRC, CAP_P);
      // Enable pull-up resistor on CAP_P
      bit_set(PORTC, CAP_P);

      int digital_value_of_cap_p;
      unsigned long discharge_time_us;

      unsigned long t1 = micros();

      // Wait until digital value of CAP_P reads as logic 1
      while (1)
      {
        digital_value_of_cap_p = bit_read(PINC, CAP_P); // Read CAP_P

        unsigned long t2 = micros();
        discharge_time_us = (t2 > t1) ? (t2 - t1) : (t1 - t2);

        if (digital_value_of_cap_p == 1)
        {
          break;
        }

        // timeout
        if (discharge_time_us >= 400000L)
        {
          break;
        }
      }

      // Disable pull-up on CAP_P
      bit_clear(PORTC, CAP_P);

      adc_reading = adc_read(CAP_P);

      // Capacitor Charging equation:
      // C = -t / (R * ln(1 - Vc / Vs))
      // Vc: voltage across the capacitor
      // Vs: supply voltage (max voltage)
      // t:  elapsed time since the application of the supply voltage
      // R:  resistor value of the RC charging circuit
      float capacitance = -(float)discharge_time_us / R_PULLUP / log(1.0 - (float)adc_reading / (float)MAX_ADC_VALUE);

      printf("Capacitance Value = ");
      if (capacitance > 1000.0)
      {
        printf("%.2f uF", capacitance / 1000.0);
      }
      else
      {
        printf("%.2f nF", capacitance);
      }

      printf(" (");
      printf(digital_value_of_cap_p == 1 ? "Normal" : "HighVal");
      printf(", discharge_time_us= %lu us, ADC= %u)\r\n", discharge_time_us, adc_reading);

      // Discharge the capacitor

      // Write 1 to CAP_N
      bit_set(PORTC, CAP_N);

      int discharge_time_ms = (int)(discharge_time_us / 1000L) * 5;

      precise_delay_ms(discharge_time_ms);

      // Config CAP_P as output
      bit_set(DDRC, CAP_P);
      // Write 0 to CAP_P
      bit_clear(PORTC, CAP_P);

      // Write 0 to CAP_N
      bit_clear(PORTC, CAP_N);
    }

    // Wait until this iteration of main loop takes 1000 ms (1 second)
    while (millis() % 1000 != 0)
      ;
  }

  return 0;
}
