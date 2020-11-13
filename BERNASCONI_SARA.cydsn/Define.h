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
#ifndef __DEFINE_H__
    #define __DEFINE_H__
    
    //Device address
    #define LIS3DH_DEVICE_ADDRESS 0x18
    
    //Ctrl_Reg_4
    //Address
    #define LIS3DH_CTRL_REG4 0x23
    //Activate 4th bit
    #define LIS3DH_CTRL_HR_REG4 0x08
    //FS bits set to 00 (+-2g) and BDU,BLE to 0 
    #define LIS3DH_CTRL_FS_REG4 0X00
    
    //Conversion
    #define Sensitivity 1 //mg/digit
    #define conversion Sensitivity*0.001*9.81
    
    //Ctrl_Reg_1
    //Address
    #define LIS3DH_CTRL_REG1 0x20
    //Axis Enable (activate 1st,2nd,3rd bit)
    #define LIS3DH_CTRL_REG1_AXIS_ENABLE 0x07
    
    //Status Register(reading only)
    //Address
    #define LIS3DH_STATUS_REG 0x27
    //Data of x,y,z axis
    #define LIS3DH_OUT_X_L 0x28
    #define LIS3DH_OUT_Y_L 0x2A
    #define LIS3DH_OUT_Z_L 0x2C
    
    //Transmitting data byte(header and footer not considered)
    #define OUT_DATA 6
    
    //EEPROM address where the sampling frequency is saved
    #define EEPROM_ADDRESS_FREQUENCY 0x0000

#endif    
/* [] END OF FILE */
