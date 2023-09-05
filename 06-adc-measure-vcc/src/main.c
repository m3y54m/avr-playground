/* Connections

RXD (PD0) <--- TTL Serial Port TX
TXD (PD1) ---> TTL Serial Port RX
      GND <--- TTL Serial Port GND
*/

// Project common config and definitions
// NOTE: put this before other includes
#include "my/project_config.h"

#include <avr/io.h>
#include "my/serial_port.h"
#include <util/delay.h>

// Calibrated value for 1.1V band-gap reference voltage
#define CALIBRATED_1V1_VBG 1.088f

int main(void)
{
  // initialize uart0 to use printf()
  serial_port_init();

  // print project config to serial port (USART)
  printf("F_CPU = %lu\r\n", F_CPU);
  printf("BAUD = %lu\r\n", BAUD);

  //*************** ADC Configuration *****************

  // Select AVCC as ADC reference and 1.1V internal reference voltage as ADC input
  // REFS: 01 => Vref: AVCC with external capacitor at AREF pin
  ADMUX |= (0b01 << REFS0);
  // MUX: 1110 => Vin: 1.1V (VBG)
  ADMUX |= (0b1110 << MUX0);

  // ADC clock prescaler
  // ADPS: 111 => Prescaler = 128
  ADCSRA |= (0b111 << ADPS0);
  // Enable ADC
  ADCSRA |= (1 << ADEN);

  //***************************************************

  while (1)
  {
    // start ADC conversion
    ADCSRA |= (1 << ADSC);
    // wait until done
    loop_until_bit_is_clear(ADCSRA, ADSC);
    // get ADC reading (10-bit)
    // NOTE: ADCL should be read first.
    // You can use ADC macro that itself takes care of reading ADC register:
    // unsigned int adc_reading =  ADC;
    unsigned int adc_reading =  ADCL | (ADCH << 8);

    // calculate the Vcc voltage
    float vcc_voltage_value = (1023.0f * CALIBRATED_1V1_VBG) / (adc_reading);

    printf("\r\nADC reading: %u\r\n", adc_reading);
    printf("VCC voltage: %.3f\r\n", vcc_voltage_value);

    _delay_ms(1000);
  }

  return 0;
}
