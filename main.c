/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include "Buttons.h"
#include "Sounds.h"
#include "Analog.h"
#include "Display.h"
#include "Delay.h"
#include "main.h"

#define GAMEDELAY_STARTUP_DEFAULT 1150
extern uint8_t segments[4]; // For the display
extern uint32_t ms;
extern uint8_t pressedKey;
uint16_t gameDelayMs = GAMEDELAY_STARTUP_DEFAULT; // Default startup delay before getting the next key.
                                                  // Also used in multiple while loops for delay. This variable
                                                  // is decremented in TIMER1_A0_VECTOR ISR
uint8_t gameDelayDecrementSpeed = 10; // 10 ms default start step
volatile uint16_t totalScore = 0;
volatile uint8_t gameState = WAITING_BUTTON_PRESS;

extern void Grace_init(void);

void playFanfare();
uint16_t calculateNewDelay(uint16_t currentDelay);
void increaseScore();
void getHighScore();
void resetScore();
void flash_write_highscore(uint16_t score);
uint16_t flash_read_highscore();



/*
 *  ======== main ========
 */
int main( void )
{
    uint16_t currentDelay = GAMEDELAY_STARTUP_DEFAULT;
    uint16_t analog;
    uint8_t rightKeyTable[] = {0,0,0,0,0,0,0}; // Max of 7 keys
    uint8_t rightKeyIndex = 0;
    uint8_t pressedKeyIndex = 0;
    uint8_t tmp = 0;
    uint8_t currentKey = 0;
    int16_t seed = 0;
    uint16_t highScore = 0;
    uint16_t allTimeHigh = 0;

    // Clear port interrupts
    P2IFG = 0;
    P1IFG = 0;
    // Stop WDT+
    WDTCTL = WDTPW + WDTHOLD; // Set hold bit and clear others

    Grace_init();                     // Activate Grace-generated configuration
    StopSound();


    // Check if alltimeHigh-score is FFFF then initialize it as zero. FFFF = nothing programmed into memory
    allTimeHigh = flash_read_highscore();
    if(allTimeHigh == 0xFFFF)
    {
        flash_write_highscore(0);
        allTimeHigh = 0;
    }


    // Check if Blue button is being pressed while powering. If so, then highscore is erased
    P2IE = 0;
    P1IE = 0;
    gameDelayMs = 100;
    displayOff();
    while(gameDelayMs)
    {
        if(BUTTON_1_KEPT_PRESSED)
        {
            gameState = GAME_ERASE_HIGHSCORE;
            break;
        }
        updateDisplay();
    }

    Grace_init();

    // Main loop
    displayOn();
    while(1)
    {

        switch(gameState)
        {
        case GAME_ERASE_HIGHSCORE:

            tmp = 8;
            displayOn();
            while(tmp--)
            {
                gameDelayMs = 200;
                segments[0] = segments[1] = segments[2] = segments[3] = 8;
                while(gameDelayMs)
                {
                    updateDisplay();
                }
                segments[0] = segments[1] = segments[2] = segments[3] = BLANK;
                gameDelayMs = 200;
                while(gameDelayMs)
                {
                    updateDisplay();
                }
            }
            flash_write_highscore(0);
            allTimeHigh = 0;
            gameState = WAITING_BUTTON_PRESS;
            break;
        case GAME_RUNNING:

            // Arvotaan random nappula ja soitetaan nappulan ääni ja laitetaan oikea valo palamaan.
            // Laitetaan myös nappulan tieto talteen
            currentKey = randomButton();

            if(rightKeyIndex > sizeof(rightKeyTable) - 1)
            {
                // ei päästetä indeksiä kasvamaan liian suureksi
                rightKeyIndex = 0;
            }

            if(rightKeyTable[rightKeyIndex] == 0) // jos paikalla on 0, niin mitään ei ole arvottu ja pelaaja on kerennyt painamaan oikein
            {
               rightKeyTable[rightKeyIndex++] = currentKey;
            }
            else
            {
               // Liian monta numeroa arvottu koska taulukossa ei ole tyhjää, toisin sanoen pelaaja on liian paljon jäljessä
               gameState = GAME_OVER;
               break;
            }
            currentDelay = gameDelayMs;
            switch (currentKey)
            {
                case 1:
                    SetSound(BUTTON1_SOUND);
                    BUTTON1_LED_ON;
                    break;
                case 2:
                    SetSound(BUTTON2_SOUND);
                    BUTTON2_LED_ON;
                    break;
                case 3:
                    SetSound(BUTTON3_SOUND);
                    BUTTON3_LED_ON;
                    break;
                case 4:
                    SetSound(BUTTON4_SOUND);
                    BUTTON4_LED_ON;
                    break;
                default:
                    break;
            }

            // Ennen seuraavan nappulan arvontaa odotellaan viiveellä,
            // joka pienenee koko ajan kun peli etenee. Tarkastetaan myös painoiko käyttäjä nappia vai
            // heräsikö peli viiveen loputtua
            currentDelay = gameDelayMs;
            while(gameDelayMs)
            {
                updateDisplay();

                // jos nappia on painettu niin ehto täyttyy
                if(pressedKey != NO_BUTTON)
                {

                    if(rightKeyTable[pressedKeyIndex] == pressedKey)
                    {
                        increaseScore();
                        rightKeyTable[pressedKeyIndex] = 0;
                    }
                    else
                    {
                        gameState = GAME_OVER;
                        gameDelayMs = 0;
                    }

                    pressedKeyIndex++;
                    if(pressedKeyIndex > sizeof(rightKeyTable) - 1)
                    {
                        pressedKeyIndex = 0;
                    }
                    pressedKey = NO_BUTTON;
                }
            }

            gameDelayMs = calculateNewDelay(currentDelay);

            BUTTON_LEDS_OFF;
            StopSound();

            break; // GAME_RUNNING

            case GAME_OVER:
                analog = BUTTON1_SOUND;
                tmp = 4;
                while(tmp--)
                {
                    gameDelayMs = 400;
                    BUTTON1_LED_ON;
                    BUTTON2_LED_ON;
                    BUTTON3_LED_ON;
                    BUTTON4_LED_ON;
                     analog += 5000;
                     SetSound( analog);
                    while(gameDelayMs)
                    {
                        updateDisplay();
                    }
                }
                StopSound();

                tmp = 4;
                while(tmp--)
                {
                    gameDelayMs = 500;
                    displayOn();
                    while(gameDelayMs)
                    {
                        updateDisplay();
                        BUTTON1_LED_ON;
                        BUTTON2_LED_ON;
                        BUTTON3_LED_ON;
                        BUTTON4_LED_ON;
                    }
                    gameDelayMs = 500;
                    while(gameDelayMs)
                    {
                        displayOff();
                        updateDisplay();
                        BUTTON_LEDS_OFF;
                    }
                }
                displayOn();
                // check for a new highscore and update if necessary
                if(totalScore > highScore)
                {
                    highScore = totalScore;
                    if(highScore > flash_read_highscore())
                    {
                        allTimeHigh = totalScore;
                        flash_write_highscore(allTimeHigh);
                        playFanfare();
                        tmp = 7;
                        while(tmp--)
                        {
                            gameDelayMs = 350;
                            displayOff();
                            while(gameDelayMs)
                            {
                                updateDisplay();
                            }
                            gameDelayMs = 350;
                            displayOn();
                            while(gameDelayMs)
                            {
                                updateDisplay();

                            }
                        }
                    }
                }

                StopSound();

                //pressedKey = NO_BUTTON;
                // change state to waiting
                gameState = WAITING_BUTTON_PRESS;

                break;
            case GAME_STARTING:
                displayOff();
                displayOn();
                gameDelayMs = 1000;
                // Initialize random
                seed = ReadAnalogInputMilliVolts(0);
                srand(seed);
                tmp = 5;
                while(tmp--)
                {
                    BUTTON1_LED_ON;
                    BUTTON2_LED_ON;
                    BUTTON3_LED_ON;
                    BUTTON4_LED_ON;
                    gameDelayMs = 250;
                    while(gameDelayMs)
                    {
                        updateDisplay();
                    }
                    gameDelayMs = 250;
                    BUTTON_LEDS_OFF;
                    while(gameDelayMs)
                    {
                        updateDisplay();
                    }

                }

                // reset variables
                for(tmp = 0; tmp < sizeof(rightKeyTable); tmp++)
                {
                    rightKeyTable[tmp] = 0;
                }
                rightKeyIndex = 0;
                pressedKeyIndex = 0;
                pressedKey = NO_BUTTON;
                resetScore();
                gameDelayMs = 3000;
                while(gameDelayMs)
                {
                    updateDisplay();
                }
                gameDelayMs = GAMEDELAY_STARTUP_DEFAULT;

                // Change state to running
                gameState = GAME_RUNNING;
                break;
            case WAITING_BUTTON_PRESS: // So called demo mode without sounds
                StopSound();
                displayOn();

                while(pressedKey == NO_BUTTON)
                {
                    if(allTimeHigh == 0)
                    {
                        segments[0] = BLANK;
                        segments[1] = random(0, 9);
                        segments[2] = random(0, 9);
                        segments[3] = BLANK;
                    }
                    else
                    {
                        if(highScore == 0)
                        {
                            // update high score to display while in "demo mode"
                            getHighScore(allTimeHigh);
                        }
                        else
                        {
                            getHighScore(highScore);
                        }
                    }
                    if(pressedKey != NO_BUTTON)
                    {
                        pressedKey = pressedKey;
                        break;
                    }
                    gameDelayMs = random(500, 1500);
                    currentKey = randomButton();
                    while(gameDelayMs)
                    {
                        switch (currentKey)
                        {
                            case 1:
                                //SetSound(BUTTON1_SOUND);
                                BUTTON1_LED_ON;
                                break;
                            case 2:
                                //SetSound(BUTTON2_SOUND);
                                BUTTON2_LED_ON;
                                break;
                            case 3:
                                //SetSound(BUTTON3_SOUND);
                                BUTTON3_LED_ON;
                                break;
                            case 4:
                                //SetSound(BUTTON4_SOUND);
                                BUTTON4_LED_ON;
                                break;
                        }
                        if(pressedKey != NO_BUTTON)
                        {
                            break;
                        }
                        updateDisplay();
                    }
                    BUTTON_LEDS_OFF;
                    if(pressedKey != NO_BUTTON)
                    {
                        break;
                    }
                }

                switch(pressedKey)
                {
                case BUTTON_1:
                    SetSound(BUTTON1_SOUND);
                    break;
                case BUTTON_2:
                    SetSound(BUTTON2_SOUND);
                    break;
                case BUTTON_3:
                    SetSound(BUTTON3_SOUND);
                    break;
                case BUTTON_4:
                    SetSound(BUTTON4_SOUND);
                    break;

                }
                displayOff();
                gameDelayMs = 100;
                while(gameDelayMs)
                {
                    updateDisplay();
                }
                StopSound();
                gameState = GAME_STARTING;
                BUTTON1_LED_ON;
                BUTTON2_LED_ON;
                BUTTON3_LED_ON;
                BUTTON4_LED_ON;
                gameDelayMs = random(1000, 3000);
                while(gameDelayMs)
                {
                    updateDisplay();
                }

                break;
            default:

                break;
        }
    }
}
void playFanfare()
{
    uint16_t notes[] = {BUTTON1_SOUND,BUTTON1_SOUND,BUTTON1_SOUND,BUTTON4_SOUND, BUTTON2_SOUND, BUTTON4_SOUND}; // 6 notes
    uint16_t noteLens[] = {300,300,300,450,200,650}; // 6 length
    uint8_t i=0;
    uint8_t pause = 120;

    while(i < (sizeof(notes) / 2))
    {
        SetSound(notes[i]);
        gameDelayMs = noteLens[i]-pause;
        while(gameDelayMs)
        {
            updateDisplay();
        }
        gameDelayMs = pause;
        while(gameDelayMs)
        {
            updateDisplay();
        }
        i++;
    }
    StopSound();
}
uint16_t calculateNewDelay(uint16_t currentDelay)
{

    if(totalScore < 20)
    {
        gameDelayDecrementSpeed = 15; // 300ms
    }
    if( (totalScore > 20) && (totalScore < 40))
    {
        gameDelayDecrementSpeed = 10; // 200 ms
    }
    if( (totalScore > 40) && (totalScore < 60))
    {
        gameDelayDecrementSpeed = 7; // 140 ms
    }
    if( (totalScore > 60) && (totalScore < 80))
    {
        gameDelayDecrementSpeed = 3; // 120 ms
    }

    currentDelay -= gameDelayDecrementSpeed;

    if( (currentDelay > 1400) || (currentDelay < 80)) // rullannut ympäri tai alle alarajan 80 ms
    {
        currentDelay = 80;
    }

    return currentDelay;
}

void increaseScore()
{
    uint16_t value;
    uint8_t digit=0;
    uint8_t index = 3;
    value = ++totalScore;

    while (value > 0) {
     digit = value % 10;
     // do something with digit
     segments[index--] = digit;
     value /= 10;
    }
}

void getHighScore(uint16_t score)
{
    uint16_t value;
    uint8_t digit=0;
    uint8_t index = 3;
    value = score;

    while (value > 0) {
     digit = value % 10;
     // do something with digit
     segments[index--] = digit;
     value /= 10;
    }
}

void resetScore()
{
    totalScore = 0;
    segments[3] = 0;
    segments[2]  = 0;
    segments[1]  = 0;
    segments[0] = 0;
}

void flash_write_highscore(uint16_t score)
{
    uint16_t *address;
    address = (uint16_t *) 0x1000;

    //erase
    __disable_interrupt();               // Disable interrupts. This is important, otherwise,
                                         // a flash operation in progress while interrupt may
                                         // crash the system.
    while(BUSY & FCTL3);                 // Check if Flash being used
    FCTL2 = FWKEY + FSSEL_1 + FN3;       // Clk = SMCLK/4
    FCTL1 = FWKEY + ERASE;               // Set Erase bit
    FCTL3 = FWKEY;                       // Clear Lock bit
    *address = 0;                           // Dummy write to erase Flash segment
    while(BUSY & FCTL3);                 // Check if Flash being used
    FCTL1 = FWKEY;                       // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                // Set LOCK bit

    address = (uint16_t *) 0x1000;
    FCTL3 = FWKEY;          // Clear lock bit
    FCTL1 = FWKEY + WRT;    // set WRT bit for byte/word write operation
    *address = score;       // store the score
    FCTL1 = FWKEY;          // Clear WRT bit
    FCTL3 = FWKEY + LOCK;    // Set LOCK

    __enable_interrupt();

}

uint16_t flash_read_highscore()
{
    uint16_t *address;
    address = (uint16_t*) 0x1000;
    return *address;
}
