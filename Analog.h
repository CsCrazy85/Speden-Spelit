/*
 * Analog.h
 *
 *  Created on: 23.4.2017
 */

#ifndef ANALOG_H_
#define ANALOG_H_

#include <msp430.h>
#include <stdlib.h>
#include <stdint.h>

uint16_t ReadAnalogInputMilliVolts(float reference);
int16_t ReadTemperature();

#endif /* ANALOG_H_ */
