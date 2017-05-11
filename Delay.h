/*
 * Delay.h
 *
 *  Created on: 23.4.2017
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <msp430.h>
#include <stdint.h>

void delayMs(uint16_t delay);
void gotoSleep();

#endif /* DELAY_H_ */
