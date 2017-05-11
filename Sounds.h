/*
 * Sounds.h
 *
 *  Created on: 21.4.2017
 *      Author: CsCrazy
 */

#ifndef SOUNDS_H_
#define SOUNDS_H_

#include <stdint.h>

// ######### Defines ############

// VALUES FOR TA0CCR0 register (16 MHz SMCLK)

#define CCR0_120HZ  16665  // CS4
#define CCR0_150HZ  13331  // DS4
#define CCR0_190HZ  10525  // FS4
#define CCR0_230HZ  8693  // GS4

// VALUES FOR TA0CCR1 register for ~50% pwm
#define CCR1_120HZ  8340
#define CCR1_150HZ  6672
#define CCR1_190HZ  5267
#define CCR1_230HZ  4350

#define BUTTON1_SOUND   CCR0_120HZ
#define BUTTON2_SOUND   CCR0_150HZ
#define BUTTON3_SOUND   CCR0_190HZ
#define BUTTON4_SOUND   CCR0_230HZ

#define BUTTON1_SOUND_  CCR1_120HZ
#define BUTTON2_SOUND_  CCR1_150HZ
#define BUTTON3_SOUND_  CCR1_190HZ
#define BUTTON4_SOUND_  CCR1_230HZ

// ############ Functions ##############
void SetSound(uint16_t button_sound);
void StopSound();

#endif /* SOUNDS_H_ */
