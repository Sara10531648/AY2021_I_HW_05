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
#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "project.h"
    CY_ISR_PROTO(Button_ISR);
    //Isr flag
    volatile uint8 Flag;
    //Sampling frequancy variable
    uint8 Freq;
#endif
/* [] END OF FILE */
