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
    //EPROM START
    EEPROM_Start();
    //READ AT 0X0000 ADDRESS 
    Freq=EEPROM_ReadByte(0x0000);
    
    
    I2C_Peripheral_Start();
    UART_Deb_Start();
   
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    if( Freq<1 || Freq>6)
    {
        Freq=1;
        
    }
    
    
    
    //SETTING HIGH RESOLUTION MODE
    HRMode();
    //SET INITIAL FREQUENCY
    SetFreq();
     
 
        // Initializing isr flag
    Flag=0;
    
   
  uint8 statusreg;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray [1+OUT_DATA+1];
    OutArray[0] = header;
    OutArray[7] = footer;
    int16 dirtytrick = 1000;
    int16 outconv[3];
    float P[3];
    uint8_t RData[6];

    int16 OutX;
    int16 OutY;
    int16 OutZ;
    
    for(;;)
    {   

    if (Flag)
    { 
        Flag=0;
        EEPROM_UpdateTemperature();
         //WRITE AT 0X0000 A NEW VALUE
        EEPROM_WriteByte(Freq,0x0000);
        SetFreq();
    }
    
   I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_STATUS_REG, 
                                                  &statusreg);
    if (statusreg & 0x08)
{

    

    I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                LIS3DH_OUT_X_L,
                                                6,
                                                RData);
        
            //CONVERSIONE PER OTTENERE I FLOATING IN OUTPUT
            OutX = (int16)((RData[0] | (RData[1]<<8)))>>4;
            OutY = (int16)((RData[2] | (RData[3]<<8)))>>4;
            OutZ = (int16)((RData[4] | (RData[5]<<8)))>>4;
            
            P[0]=(conversion*OutX);
            P[1]=(conversion*OutY);
            P[2]=(conversion*OutZ);
            
            
            
            
            outconv[0]=(int16)(P[0]* dirtytrick);
            outconv[1]=(int16)(P[1]* dirtytrick);
            outconv[2]=(int16)(P[2]* dirtytrick);
            
            //sprintf(message, "%d\n", outconv[2]);
           //UART_Deb_PutString(message);
           
         
            OutArray[1] = (uint8_t)(outconv[0] & 0xFF);
            OutArray[2] = (uint8_t)((outconv[0]>> 8 )&0XFF);
            OutArray[3] = (uint8_t)(outconv[1] & 0xFF);
            OutArray[4] = (uint8_t)((outconv[1] >> 8)&0XFF);
            OutArray[5] = (uint8_t)(outconv[2] & 0xFF);
            OutArray[6] = (uint8_t)((outconv[2]>> 8)&0XFF);
            
            
            
            
            UART_Deb_PutArray(OutArray, 8);
       
        
        
    }
       
    }
}
    

/* [] END OF FILE */
