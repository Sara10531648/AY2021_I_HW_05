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
#include "project.h"
#include "Debounce.h"


#define DEBOUNCE_DELAY 100000

//BUTTON PRESS--->LOGIC STATE LOW
#define BUTTON_PRESSED 0
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
DEBOUNCE_ISR_StartEx(Button_ISR);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //EPROM START
    EEPROM_Start();
    //READ AT 0X0000 ADDRESS 
    uint8_t Last_f=EEPROM_ReadByte(0x0000);
    EEPROM_UpdateTemperature();
    //WRITE AT 0X0000 A NEW VALUE
    EEPROM_WriteByte(0x98,0x0000);
    Last_f=EEPROM_ReadByte(0x0000);
    
    
   
    
    for(;;)
    {
        
    }
}
    

/* [] END OF FILE */
