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

//brief 7-bit I2C address of the slave device
#define LIS3DH_DEVICE_ADDRESS 0x18

/**
*   \brief Address of the WHO AM I register
*/
#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

/**
*   \brief Address of the Status register
*/
#define LIS3DH_STATUS_REG 0x27
/**

//brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20

#define LIS3DH_CTRL_REG4 0x23

#define LIS3DH_CTRL_HR_REG4 0x08
#define OUT_DATA 6

int main(void)
{
    
    //EPROM START
    EEPROM_Start();
    //READ AT 0X0000 ADDRESS 
    uint8 Freq=EEPROM_ReadByte(0x0000);
    EEPROM_UpdateTemperature();
    
    I2C_Peripheral_Start();
    UART_Debug_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    // String to print out messages on the UART
    char message[50] = {'\0'};
    
    UART_Debug_Start();
    UART_Debug_PutString("**************\r\n");
    UART_Debug_PutString("** I2C Scan **\r\n");
    UART_Debug_PutString("**************\r\n");
    
    CyDelay(100);
    
    uint32_t rval;
 
	// Setup the screen and print the header
	UART_Debug_PutString("\n\n   ");
	for(uint8_t i = 0; i<0x10; i++)
	{
        sprintf(message, "%02X ", i);
		UART_Debug_PutString(message);
	}
 
	// Iterate through the address starting at 0x00
	for(uint8_t i2caddress = 0; i2caddress < 0x80; i2caddress++)
	{
		if(i2caddress % 0x10 == 0 )
        {
            sprintf(message, "\n%02X ", i2caddress);
		    UART_Debug_PutString(message);
        }
 
		rval = I2C_Master_MasterSendStart(i2caddress, I2C_Master_WRITE_XFER_MODE);
        
        if(rval == I2C_Master_MSTR_NO_ERROR) // If you get ACK then print the address
		{
            sprintf(message, "%02X ", i2caddress);
		    UART_Debug_PutString(message);
		}
		else //  Otherwise print a --
		{
		    UART_Debug_PutString("-- ");
		}
        I2C_Master_MasterSendStop();
	}
	UART_Debug_PutString("\n\n");
//    
    //SETTING HIGH RESOLUTION MODE
    HRMode();
    //SET INITIAL FREQUENCY
    SetFreq();
    // Initializing isr flag
    Flag=0;
    CyGlobalIntEnable; /* Enable global interrupts. */
    DEBOUNCE_ISR_StartEx(Button_ISR);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {   
     /* Read WHO AM I REGISTER register */
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm\r\n");   
    }   

    

      
    
    
    
    
    
    
    
    //fai una funzione!
    if (Flag)
    {
        switch (Freq)
        {
            case 1:
             UART_Debug_PutString(" Sampling at : 1Hz");
            break;
            case 2:
             UART_Debug_PutString(" Sampling at : 10Hz");
            break;
            
            case 3:
             UART_Debug_PutString(" Sampling at : 25Hz");
            break;
            
            case 4:
             UART_Debug_PutString(" Sampling at : 50Hz");
            break;
            
            case 5:
             UART_Debug_PutString(" Sampling at : 100Hz");
            break;
            
            case 6:
             UART_Debug_PutString(" Sampling at : 200Hz");
            break;
            
            default:
            UART_Debug_PutString("Error");
            break;
        }
        Flag=0;
    }
    
     //WRITE AT 0X0000 A NEW VALUE
    EEPROM_WriteByte(Freq,0x0000);
    
    //WRITE NEW F IN CTRL4
    SetFreq();
    
 
 /// print outputs : FAI FUNZIONE PER CONVERSIONE
         uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray [1+OUT_DATA+1];
    OutArray[0] = header;
    OutArray[7] = footer;
    int16 conversion = 1;
    int16 dirtytrick = 1000;
    float outtempconv;
    
    uint8_t XData[2];
    uint8_t YData[2];
    uint8_t ZData[2];
    
    int16 OutTemp;
    
    for(;;)
    {
        CyDelay(100);
/*        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_ADC_3L,
                                            &TemperatureData[0]);
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_ADC_3H,
                                            &TemperatureData[1]);
*/
        error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                LIS3DH_OUT_ADC_3L,
                                                2,
                                                XData);
        if(error == NO_ERROR)
        {  
            OutX = (int16)((XData[0] | (XData[1]<<8)))>>6;
            outtempconv = OutX * conversion;
            
            OutX = (int16) (outtempconv * dirtytrick);
            
//            sprintf(message, "Temp output: %d\r\n", OutTemp);
//            UART_Debug_PutString(message);
            OutArray[1] = (uint8_t)(OutX & 0xFF);
            OutArray[2] = (uint8_t)(OutX >> 8);
            UART_Debug_PutArray(OutArray, 4);
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to read ADC 3 / temperature output registers\r\n");   
        }
        
        
    }
        
    }
}
    

/* [] END OF FILE */
