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

void ModeSettings(void)
{
    /*Write register: (1st bist is LSB) 4th bit of ctrl_reg_4 to 1 to set High Resolution Mode
                      5th and 6th bit to default value 00 to obtain a FullScaleRange of +-2g
    */
    
    I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                 LIS3DH_CTRL_REG4,
                                (LIS3DH_CTRL_FS_REG4<<4)|LIS3DH_CTRL_HR_REG4);
}
    
void SetFreq (void)
{
    /*Write in ctrl_reg_1: enable 3 axis (1st, 2nd, 3rd bit to 1)
                           write sampling frenquency in DataRateSelection (5th,6th,7th,8th bit)
    */
    I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                     LIS3DH_CTRL_REG1, 
                                    (Freq<<4)|LIS3DH_CTRL_REG1_AXIS_ENABLE);
}

/* [] END OF FILE */
