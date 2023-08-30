#pragma once

#ifndef __UART_H__
#define __UART_H__

#include "uart.h"

#define BAUD 9600UL

#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>

void uart0_init(void);

#endif // __UART_H__