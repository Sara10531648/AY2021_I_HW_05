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

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20

#define LIS3DH_CTRL_REG4 0x23

#define LIS3DH_CTRL_HR_REG4 0x08


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
DEBOUNCE_ISR_StartEx(Button_ISR);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    


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
    
   
    
    for(;;)
    {
        
        ///if the isr has been called, we have to set the sequent frequency in device 
        //register and write it also in the EEPROM REGISTER
       
        //READ REGISTRI COINVOLTI IN HOGH RESOLUTION
        
        
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
//read ctrl1_REG TO FIND OLD F(LPEN=1 DEFAULT)
        /*        Read Control Register 1         */
    /******************************************/
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    

      //WRITE REGISTERS DEVICE CON NEW FREQUENCY
    
    
    //CTRL4 WRITE FOR HR 
    uint8_t ctrl_reg4;

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    
    ctrl_reg4 = LIS3DH_CTRL_HR_REG4; // must be changed to the appropriate value
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         ctrl_reg4);
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4 after being updated: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    //leggo ultima f salvata
    Freq=EEPROM_ReadByte(0x0000);      
    
    
    //fai una funzione!
    
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
    
    
    //
     //WRITE AT 0X0000 A NEW VALUE
    EEPROM_WriteByte(Freq,0x0000);
    
    //write new sampling frequency in 
    
        UART_Debug_PutString("\r\nWriting new values..\r\n");
        ctrl_reg1 = Freq;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
 /// print outputs
        
        
    }
}
    

/* [] END OF FILE */
