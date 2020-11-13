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
#include "I2C_Interface.h"
#include "stdio.h"
#include "DeviceSettings.h"
#include "Define.h"


int main(void)
{
     CyGlobalIntEnable; 
    /* Enable global interrupts. */
    DEBOUNCE_ISR_StartEx(Button_ISR); 
    
    //Start of components
    I2C_Peripheral_Start();
    UART_Deb_Start();
    EEPROM_Start();
    //Copy the last sampling frequency saved in EEPROM 
    Freq=EEPROM_ReadByte(EEPROM_ADDRESS_FREQUENCY);
    
    //If no valid frequency has been saved in EEPROM, set initial Frequency at 1Hz
    if( Freq<1 || Freq>6)
    {
        Freq=1;
    }
    
    //SETTING HIGH RESOLUTION MODE & FS
    ModeSettings();
    //SET INITIAL FREQUENCY
    SetFreq();
   
    // Initializing isr flag
    Flag=0;
    
    //Variables to take, convert and otuput Data
    uint8 statusreg;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray [1+OUT_DATA+1];
    OutArray[0] = header;
    OutArray[7] = footer;
    int16 dirtytrick = 1000;
    int16 OutConv[3];
    uint8_t NewData[6];
    int16 OutX;
    int16 OutY;
    int16 OutZ;
    int i;
    
    for(;;)
    {   
        //If the button has been pressed the Flag goes to 1
        if (Flag)
        { 
            Flag=0;
            EEPROM_UpdateTemperature();
            //Write new frequency value in EEPROM
            EEPROM_WriteByte(Freq,EEPROM_ADDRESS_FREQUENCY);
            //Set new frequency in device register
            SetFreq();
        }
        
        //Read status register to understand when there is a new set of available Data
        I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                    LIS3DH_STATUS_REG, 
                                    &statusreg);
        //If new Data available the 4th bit goes to 1
        if (statusreg & 0x08)
        {
            //MultiRead of register OUT_X_L,OUT_X_H,OUT_Y_L,OUT_Y_H,OUT_Z_L,OUT_Z_H
            //Save NewData in array
            I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_OUT_X_L,
                                             6,
                                             NewData);
        
            //Reconstruction of Data into 16 bit variables, considering samping in 12 bit
            OutX = (int16)((NewData[0] | (NewData[1]<<8)))>>4;
            OutY = (int16)((NewData[2] | (NewData[3]<<8)))>>4;
            OutZ = (int16)((NewData[4] | (NewData[5]<<8)))>>4;
            
            //Take data and multiply for sensitivity and g to obtain data in m/s^2
            //multiply data per 1000 to obtain an integer
            OutConv[0]=(int16)(conversion*OutX* dirtytrick);
            OutConv[1]=(int16)(conversion*OutY* dirtytrick);
            OutConv[2]=(int16)(conversion*OutZ* dirtytrick);
            
            //String to verify output in Coolterm
            //sprintf(message, "%d\n", outconv[2]);
            //UART_Deb_PutString(message);
           
            //Packet for sanding via Uart,2 byte for each axis
            for(i=0;i<=2;i++)
            {
                OutArray[i*2+1] = (uint8_t)(OutConv[i] & 0xFF);
                OutArray[i*2+2] = (uint8_t)((OutConv[i]>> 8 )&0XFF);
            }

            //Output the array, 6 bytes of Data+Header+Footer
            UART_Deb_PutArray(OutArray,1+OUT_DATA+1);
        }
    }
}
    

/* [] END OF FILE */
