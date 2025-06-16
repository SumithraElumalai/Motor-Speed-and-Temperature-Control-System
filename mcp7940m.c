//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MCP7940M.C
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
// Real-Time Clock/Calendar with SRAM
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "mcp7940m.h"
#include "i2c.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Init( void )
// PURPOSE  : Initializes the RTC
//----------------------------------------------------------------------------

void MCP7940M_Init( void )
{
    uint8_t uiData;

    uiData = 0x80; // Seconds (ST = 1)
    MCP7940M_Write( 0x00, &uiData, 1 );

    uiData = 0x56; // Minute
    MCP7940M_Write( 0x01, &uiData, 1 );

    uiData = 0x01; // Hour (12-hour, AM)
    MCP7940M_Write( 0x02, &uiData, 1 );

    uiData = 0x04; // Weekday
    MCP7940M_Write( 0x03, &uiData, 1 );

    uiData = 0x10; // Day
    MCP7940M_Write( 0x04, &uiData, 1 );

    uiData = 0x12; // Month
    MCP7940M_Write( 0x05, &uiData, 1 );

    uiData = 0x24; // Year
    MCP7940M_Write( 0x06, &uiData, 1 );

    uiData = 0x43; // Control (MFP = 32.768 kHz)
    MCP7940M_Write( 0x07, &uiData, 1 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Read( uint8_t uiRegAddr, uint8_t* puiData, uint8_t uiNumRegs )
// PURPOSE  : Reads a specified number of registers from the RTC
//----------------------------------------------------------------------------

void MCP7940M_Read( uint8_t uiRegAddr, uint8_t* puiData, uint8_t uiNumRegs )
{
    uint8_t uiMCS;
    uint8_t uiRegsRead = 0;

    // Ensure the controller is idle
    I2C_WaitForControllerReady();

    if( uiNumRegs )
    {
        // Write slave address and R/W = 0
        HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 );

        // Write register address to start reading
        HWREG( I2C0_BASE + I2C_O_MDR ) = uiRegAddr;

        // Initiate I2C transaction
        uiMCS = I2C_MCS_RUN | I2C_MCS_START;
        HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

        // Wait until the controller is no longer busy
        I2C_WaitForControllerReady();

        // Write slave address and R/W = 1
        HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 ) | 0x01;

        do
        {
            // Prepare MCS
            uiMCS = I2C_MCS_RUN;

            // Include a restart condition if this is the first register to be read
            if( !uiRegsRead ) uiMCS |= I2C_MCS_START;

            // Include a stop condition if this is the last register to be read
            if( !( uiNumRegs - uiRegsRead - 1 ) )
            {
                // Prepare to terminate the transaction
                uiMCS |= I2C_MCS_STOP;
            }
            else
            {
                // Acknowledge data (more to come)
                uiMCS |= I2C_MCS_ACK;
            }

            // Update MCS to transfer data
            HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

            // Wait until the controller is no longer busy
            I2C_WaitForControllerReady();

            // Read register data
            *puiData++ = HWREG( I2C0_BASE + I2C_O_MDR );

        } while( uiNumRegs - ++uiRegsRead );
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Write( uint8_t uiRegAddr, uint8_t* puiData, uint8_t uiNumRegs )
// PURPOSE  : Write a specified number of registers to the RTC
//----------------------------------------------------------------------------

void MCP7940M_Write( uint8_t uiRegAddr, uint8_t* puiData, uint8_t uiNumRegs )
{
    uint8_t uiMCS;
    uint8_t uiRegsWritten = 0;

    // Ensure the controller is idle
    I2C_WaitForControllerReady();

    if( uiNumRegs )
    {
        // Write slave address and R/W = 0
        HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 );

        // Write register address to the transmit register
        HWREG( I2C0_BASE + I2C_O_MDR ) = uiRegAddr;

        // Initiate I2C transaction
        uiMCS = I2C_MCS_RUN | I2C_MCS_START;
        HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

        // Wait until the controller is no longer busy
        I2C_WaitForControllerReady();

        do
        {
            // Write register data
            HWREG( I2C0_BASE + I2C_O_MDR ) = *puiData++;

            // Continue with the I2C transaction
            uiMCS = I2C_MCS_RUN | ( !( uiNumRegs - ++uiRegsWritten ) ? I2C_MCS_STOP : 0 );
            HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

            // Wait until the controller is no longer busy
            I2C_WaitForControllerReady();

        } while( uiNumRegs - uiRegsWritten );
    }

    return;
}

//----------------------------------------------------------------------------
// END MCP7940M.C
//----------------------------------------------------------------------------
