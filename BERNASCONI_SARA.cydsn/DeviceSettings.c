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
#include "DeviceSettings.h"
#include "I2C_Interface.h"
#include "stdio.h"
#include "project.h"
#include "Define.h"
#include "Debounce.h"



void HRMode(void)
{
    
     
    //Write register
    I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
        
        (LIS3DH_CTRL_FS_REG4<<4)|LIS3DH_CTRL_HR_REG4);
    
}
    
    
    
    
void SetFreq (void)
{
    
        
        
        
        
        
        I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_CTRL_REG1, 
            (Freq<<4)|LIS3DH_CTRL_REG1_AXIS_ENABLE);

}

/* [] END OF FILE */
