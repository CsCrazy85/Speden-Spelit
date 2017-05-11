/*
 * Analog.c
 *
 *  Created on: 23.4.2017
 *      Author: CsCrazy
 */


#include "Analog.h"
#include "Delay.h"

// Read analog input A3 / P1.3. Conversion takes 20 us. Function checks whether the internal reference is set
// and calculates the value accordingly. If the internal reference is not set, then the reference voltage must be
// told via the parameter in millivolts ie. 3.3 V. If 0 is inserted then the told reference is not used and the internal reference
// value is checked and that is used 1.5V or 2.5V depending which one is set.
uint16_t ReadAnalogInputMilliVolts(float reference) // TODO: move to RAM
{
    uint16_t mV = 0;
    uint16_t ADC_Conversion_Result;

    // ADC Start Conversion - Software trigger
    ADC10CTL0 |= ADC10SC;

    // wait for ADC conversion complete
    //__bis_SR_register(LPM0_bits + GIE);
    gotoSleep(1);
    // Read ADC conversion result from ADC10MEM
    ADC_Conversion_Result = ADC10MEM;

    if(reference == 0)
    {
        //check the reference bits
        if(ADC10CTL0 & (1 << 5)) // check the reference on -bit
        {
            if(ADC10CTL0 & (1 << 6)) // check the REF_2_5V bit. 1 = 2.5 Vref is used
            {
                reference = 2.5;
            }
            else
            {
                reference = 1.5;
            }
        }
    }

    // 2.5V reference, calculate the analog millivolts value
    // Equation is Result_mV = ( Reference in mV / 1023) * Result
    mV = ((float) (reference / 1023) * ADC_Conversion_Result) * 1000;

    return mV;
}

int16_t ReadTemperature()
{
    uint16_t ADC_Conversion_Result;

    // ADC Start Conversion - Software trigger
    ADC10CTL0 |= ADC10SC;

    // wait for ADC conversion complete
    //__bis_SR_register(LPM0_bits + GIE);
    gotoSleep(1);
    // Read ADC conversion result from ADC10MEM
    ADC_Conversion_Result = ADC10MEM;

    return(int16_t) ((ADC_Conversion_Result * 27069L - 18169625L) >> 16);

}
