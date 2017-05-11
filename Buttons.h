#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdlib.h>
#include "Sounds.h"

#define NO_BUTTON   99
#define BUTTON_1    1
#define BUTTON_2    2
#define BUTTON_3    3
#define BUTTON_4    4

#define BUTTON_1_KEPT_PRESSED (P1IN & (1 << 5))

#define BUTTON1_LED_ON (P1OUT |= BIT7)
#define BUTTON2_LED_ON (P2OUT |= BIT5)
#define BUTTON3_LED_ON (P2OUT |= BIT4)
#define BUTTON4_LED_ON (P2OUT |= BIT3)
#define BUTTON1_LED_OFF (P1OUT &= ~BIT7)
#define BUTTON2_LED_OFF (P2OUT &= ~BIT5)
#define BUTTON3_LED_OFF (P2OUT &= ~BIT4)
#define BUTTON4_LED_OFF (P2OUT &= ~BIT3)
#define BUTTON_LEDS_OFF {   \
                            BUTTON1_LED_OFF; \
                            BUTTON2_LED_OFF; \
                            BUTTON3_LED_OFF; \
                            BUTTON4_LED_OFF; \
                        }

uint8_t randomButton();
uint16_t random(uint16_t min, uint16_t max);

#endif // BUTTONS_H_
