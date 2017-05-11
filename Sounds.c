/*
 * Sounds.c
 *
 *  Created on: 21.4.2017
 *      Author: CsCrazy
 */
#include <msp430.h>
#include "Sounds.h"
#include <stdint.h>

void SetSound(uint16_t button_sound)
{
    // Stop and clear Timer_A
    TA0CTL &= ~(MC1 + MC0); // Clear MCx bits to stop timer
    TA0CTL = TACLR;

    // Set timer interval register
    TA0CCR0 = button_sound;

    // set PWM duty cycle approximately to 50%
    switch (button_sound) {
        case BUTTON1_SOUND:
            TA0CCR1 = BUTTON1_SOUND_;
            break;
        case BUTTON2_SOUND:
            TA0CCR1 = BUTTON2_SOUND_;
            break;
        case BUTTON3_SOUND:
            TA0CCR1 = BUTTON3_SOUND_;
            break;
        case BUTTON4_SOUND:
            TA0CCR1 = BUTTON4_SOUND_;
            break;
        default:
            // nothing
            break;
    }

    // Start Timer_A
    TA0CTL |= MC_1 + TASSEL_2 + ID_3; // Start timer in up mode
}
void StopSound()
{
    // Stop and clear Timer_A
    TA0CTL &= ~(MC1 + MC0); // Clear MCx bits to stop timer
    TA0CTL = TACLR;
}
