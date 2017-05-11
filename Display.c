/*
 * Display.c
 *
 *  Created on: 23.4.2017
 *      Author: CsCrazy
 */
#include "Display.h"
#include "Delay.h"
#include "main.h"

/*
 * Kytketään 4x7-segmenttinäyttö MCU:lle. Multipleksataan näytettävien numeroiden anodit (P-mos tai PNP)
 * joko logiikkapiirillä tai diskreeteillä. Näytön päivitykseen pitää valjastaa yksi CCRx keskeytys.
 */
uint8_t segments[4] = {0,0,0,0}; // Segment[0] is the rightmost digit in the display
uint8_t onOff = 1;
extern uint32_t ms;
extern uint8_t gameState;
extern uint16_t gameDelayMs;

void displayOff()
{
    onOff = 0;
    //segments[0] = segments[1] = segments[2] = segments[3] = BLANK;
}
void displayOn()
{
    onOff = 1;
}
void updateDisplay()
{
    uint8_t i;
    uint32_t delay = 0;

    for(i = 4; i > 0; )
    {
        selectSegment(i);
        if(!onOff)
        {
            setNumber(BLANK);
        }
        else
        {
            setNumber(segments[i - 1]);
        }
        i--;

        gotoSleep(2);
        /*
        delay = ms + 5;
        while(delay != ms)
        {
        }
        */
    }
}
// Usage: updateSegment(1,5) <-- writes number 5 to first digit
void updateSegment(uint8_t segment, uint8_t number)
{
    if( (segment <= 4) && (segment > 0) )
    {
        segments[segment - 1] = number;
    }
}
void setNumber(uint8_t num)
{
    P1OUT &= ~(BCD_A + BCD_B);
    P2OUT &= ~(BCD_C + BCD_D);

    switch(num)
    {
    case 0:

    break;
    case 1:
        P1OUT |= BCD_A;
    break;
    case 2:
        P1OUT |= BCD_B;
    break;
    case 3:
        P1OUT |= BCD_A + BCD_B;
    break;
    case 4:
        P2OUT |= BCD_C;
    break;
    case 5:
        P1OUT |= BCD_A;
        P2OUT |= BCD_C;
    break;
    case 6:
        P1OUT |= BCD_B;
        P2OUT |= BCD_C;
    break;
    case 7:
        P1OUT |= BCD_B + BCD_A;
        P2OUT |= BCD_C;
    break;
    case 8:
        P2OUT |= BCD_D;
    break;
    case 9:
        P1OUT |= BCD_A;
        P2OUT |= BCD_D;
    break;
    case BLANK:
        P1OUT |= BCD_B + BCD_A;
        P2OUT |= BCD_C + BCD_D;
        break;
    }
}

void selectSegment(unsigned char seg)
{
    switch(seg)
    {
    case 1:
        P1OUT |= DISP_BIT0 + DISP_BIT1; // 11 NAND -> 00
        break;
    case 2:
        P1OUT |= DISP_BIT1;
        P1OUT &= ~DISP_BIT0;
        break;
    case 3:
        P1OUT &= ~DISP_BIT1;
        P1OUT |= DISP_BIT0;
        break;
    case 4:
        P1OUT &= ~DISP_BIT0;
        P1OUT &= ~DISP_BIT1;
        break;
    }
}
