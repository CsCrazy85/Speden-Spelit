/*
 * Display.h
 *
 *  Created on: 23.4.2017
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <msp430.h>
#include <stdint.h>

#define BLANK   10
#define BCD_A   BIT0 // P1.0
#define BCD_B   BIT1 // P1.1
#define BCD_C   BIT6 // P2.6
#define BCD_D   BIT7 // P2.7

#define DISP_BIT0   BIT2 // P1.4
#define DISP_BIT1   BIT3 // P1.5

void displayOff();
void displayOn();
void updateDisplay();
void updateSegment(uint8_t segment, uint8_t number);
void setNumber(unsigned char num);
void selectSegment(unsigned char seg);

#endif /* DISPLAY_H_ */
