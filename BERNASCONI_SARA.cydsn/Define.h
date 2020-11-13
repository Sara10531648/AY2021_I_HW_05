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
     #define LIS3DH_DEVICE_ADDRESS 0x18
    
    //ctrl4 ADDRESS
    #define LIS3DH_CTRL_REG4 0x23
    #define LIS3DH_CTRL_HR_REG4 0x08
    //ANCHE BDU BDE A 0
    #define LIS3DH_CTRL_FS_REG4 0X00
    //fs
    #define Sensitivity 1 //mg/digit
    
    #define conversion Sensitivity*0.001*9.81
    //ADDRESS REG1
    #define LIS3DH_CTRL_REG1 0x20
    //ENABLE AXIS
    #define LIS3DH_CTRL_REG1_AXIS_ENABLE 0x07
    #define LIS3DH_STATUS_REG 0x27
    #define LIS3DH_OUT_X_L 0x28
    #define LIS3DH_OUT_Y_L 0x2A
    #define LIS3DH_OUT_Z_L 0x2C
    #define OUT_DATA 6
    
    
    
    
#endif    
/* [] END OF FILE */
