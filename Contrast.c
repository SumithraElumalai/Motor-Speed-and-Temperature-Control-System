/*
 * Contrast.c
 *
 *  Created on: Nov 25, 2024
 *      Author: selumala
 */
#include "i2c.h"
#include "global.h"
#include "Contrast.h"
#include "uart.h"


void Contrast_adjustment(uint8_t m_Data,uint8_t m_Value)
{

    HWREG(I2C0_BASE+I2C_O_MSA) = (DAC_MAX518 << 1); // Select and write the slave address

   if(m_Value==0)
   {
    HWREG(I2C0_BASE+I2C_O_MDR )=0x00;  //DAC COmmand Byte
   }
   else
   {

    HWREG(I2C0_BASE+I2C_O_MDR )=0x01;
   }


    HWREG(I2C0_BASE+I2C_O_MCS )= (I2C_MCS_RUN|I2C_MCS_START ); // The I2C start data transaction

    I2C_WaitForControllerReady();   // wait for I2C command


    HWREG(I2C0_BASE+I2C_O_MDR )= m_Data ; // The Values of the Data Register values

    HWREG(I2C0_BASE+I2C_O_MCS ) = (I2C_MCS_RUN|I2C_MCS_STOP );

    I2C_WaitForControllerReady();   // wait for I2C command


}
