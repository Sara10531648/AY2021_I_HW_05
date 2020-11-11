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
 extern uint8 Freq;
CY_ISR(Button_ISR)
    {
   //variabile che tiene conto di pressione tasto
     if (Freq<6)
    Freq++;
    else
    Freq=1;
     }   
/* [] END OF FILE */
