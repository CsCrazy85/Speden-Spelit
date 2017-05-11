/*
 * Following connections are made in the circuit:
 *
 * For the LEDs (output)
 * Button 1 (Blue) = P1.7
 * Button 2 (Yellow) = P2.5
 * Button 3 (Green) = P2.4
 * Button 4 (Red) = P2.3
 *
 * For the buttons (input)
 * Button goes HIGH when pressed.
 * P1.5
 * P2.0
 * P2.1
 * P2.2
 *
 * */
#include <msp430.h>
#include "Buttons.h"
extern uint32_t ms;

// Draw a random button (1-4)
uint8_t randomButton()
{
    uint8_t key = 0;
    static uint8_t oldkey; // static so that this is remembered in RAM

    // initialize random first key TODO: use srand with something variable to initialize a better random sequence
    key = random(1, 4);

    // get a new randow key which is different than before
    while(key == oldkey)
    {
        key = random(1, 4);
    }
    oldkey = key;
    return key;
}

uint16_t random(uint16_t min, uint16_t max)
{
    return rand() % (max + 1 - min) + min; // TODO: move execution to RAM
}

