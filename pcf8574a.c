//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : PCF8574A.C
// FILE VERSION : 1.0
// PROGRAMMER   : Selumala
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, 2024-12-10, Selumala
//   - Initial release
//
//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
// Remote 8-Bit I/O Expander
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "pcf8574a.h"
#include "i2c.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Init( void )
// PURPOSE  : Initializes the I/O expander
//----------------------------------------------------------------------------

void PCF8574A_Init( void )
{
    uint8_t uiData;

    // Configure LEDs and switches
    uiData = 0xE7;
    PCF8574A_Write( PCF8574A_SA, uiData );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Write( uint8_t uiSA, uint8_t uiData )
// PURPOSE  : Writes data to the I/O expander
//----------------------------------------------------------------------------

void PCF8574A_Write( uint8_t uiSA, uint8_t uiData )
{
    uint8_t uiMCS;

    // Write slave address and R/W = 0
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( uiSA << 1 );

    // Write data to transmit register
    HWREG( I2C0_BASE + I2C_O_MDR ) = uiData;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    I2C_WaitForControllerReady();

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Read( uint8_t uiSA, uint8_t* puiData )
// PURPOSE  : Reads data from the I/O expander
//----------------------------------------------------------------------------

void PCF8574A_Read( uint8_t uiSA, uint8_t* puiData )
{
    uint8_t uiMCS;

    // Write slave address and R/W = 1
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( uiSA << 1 ) | 0x01;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    I2C_WaitForControllerReady();

    // Read data from receive register
    *puiData = HWREG( I2C0_BASE + I2C_O_MDR );

    return;
}
void SwitchPressed_SW4(bool Toggle)
{
    uint8_t uiData;

    PCF8574A_Read( PCF8574A_SA, &uiData);
    uiData = Toggle ? uiData & ~PCF8574A_LED4 : uiData | PCF8574A_LED4;
    uiData |= PCF8574A_INPUTS;
    PCF8574A_Write( PCF8574A_SA, uiData);
}

void SwitchPressed_SW5(bool Toggle)
{
    uint8_t uiData;

    PCF8574A_Read( PCF8574A_SA, &uiData);
    uiData = Toggle ? uiData & ~PCF8574A_LED5 : uiData | PCF8574A_LED5;
    uiData |= PCF8574A_INPUTS;
    PCF8574A_Write( PCF8574A_SA, uiData);
}

void SwitchPressed_SW6(bool Toggle)
{
      uint8_t uiData;

      PCF8574A_Read( PCF8574A_SA, &uiData);
      uiData = Toggle ? uiData & ~PCF8574A_LED6 : uiData | PCF8574A_LED6;
      uiData |= PCF8574A_INPUTS;
      PCF8574A_Write( PCF8574A_SA, uiData);

}
//----------------------------------------------------------------------------
// END PCF8574A.C
//----------------------------------------------------------------------------


