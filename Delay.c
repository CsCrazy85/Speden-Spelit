/*
 * Delay.c
 *
 *  Created on: 23.4.2017
 */
#include "Delay.h"
#include "Display.h"

uint8_t sleepLevel = 0;

// Sleep for one millisecond, check and decrement delay. Returns
void delayMs(uint16_t delay)
{
    while(delay--)
    {
        // Enter LPM with global interrupt enabled
        __bis_SR_register(LPM0_bits + GIE);
    }
}
void gotoSleep(uint16_t time)
{
    while(time--)
    {
        sleepLevel = 1;
        __bis_SR_register(LPM0_bits + GIE);

        sleepLevel = 0;
    }
}


