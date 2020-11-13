/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "Debounce.h"
 
CY_ISR(Button_ISR)
    {
        //If the button is press an interrupt occurs
        //If the sampling frequency is below 200 Hz, Freq is set to the next sampling frequency 
        if (Freq<6)
        {
            Freq++;
        }
        //If the last sampling frequency is 200Hz, it is set again to 1 Hz
        else
        {
            Freq=1;
        }
        //Activate interrupt flag
        Flag=1;
    }   
/* [] END OF FILE */
