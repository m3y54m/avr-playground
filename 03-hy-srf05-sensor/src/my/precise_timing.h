#pragma once

#ifndef __PRECISE_TIMING_H__
#define __PRECISE_TIMING_H__

#include "precise_timing.h"

void precise_timing_init(void);
unsigned long millis(void);
unsigned long micros(void);
void precise_delay_ms(unsigned long ms);
void precise_delay_us(unsigned char us);

#endif // __PRECISE_TIMING_H__