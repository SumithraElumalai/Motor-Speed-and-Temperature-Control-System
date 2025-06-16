//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : I2C.C
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
// I2C support functions.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "i2c.h"
#include "Contrast.h"

//----------------------------------------------------------------------------
// FUNCTION : I2C0_IntHandler( void )
// PURPOSE  : I2C 0 Interrupt Handler
//----------------------------------------------------------------------------

void I2C0_IntHandler( void )
{
    uint32_t uiMCS;

    // Check if an I2C interrupt occurred
    if( HWREG( I2C0_BASE + I2C_O_MMIS ) & ( 1 << 0 ) )
    {
        // Acknowledge interrupt (clear)
        HWREG( I2C0_BASE + I2C_O_MICR ) = ( 1 << 0 );

        // Flag an I2C event
        GLOBAL_SetSysFlag( SYSFLAGS_I2C_READY );

        // Halt on error (error bits valid only if busy is not set)
        uiMCS = HWREG( I2C0_BASE + I2C_O_MCS );
        if( !( uiMCS & I2C_MCS_BUSY ) )
        {
            if( uiMCS & ( I2C_MCS_CLKTO | I2C_MCS_ERROR ) )
            {
                while( 1 );
            }
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : I2C_Init( void )
// PURPOSE  : Initializes the I2C0 peripheral
//----------------------------------------------------------------------------

void I2C_Init( void )
{
    // GPIO Port B Run Mode Clock Gating Control (Port B)
    HWREG( SYSCTL_RCGCGPIO ) |= 0x02;

    // Configure GPIO Port B (PB3/PB2 = I2C0SDA/I2C0SCL)
    HWREG( GPIO_PORTB_BASE + GPIO_O_AFSEL ) |= 0x0C;
    HWREG( GPIO_PORTB_BASE + GPIO_O_DEN   ) |= 0x0C;
    HWREG( GPIO_PORTB_BASE + GPIO_O_PCTL  ) &= 0xFFFF00FF;
    HWREG( GPIO_PORTB_BASE + GPIO_O_PCTL  ) |= 0x00003300;
    HWREG( GPIO_PORTB_BASE + GPIO_O_ODR   ) |= 0x08;

    // I2C 0 Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCI2C ) |= 0x01;

    // Assign the microcontroller as master
    HWREG( I2C0_BASE + I2C_O_MCR  ) = ( 1 << 4 ) | ( 1 << 6 );
    HWREG( I2C0_BASE + I2C_O_MCR2 ) = ( 0x07 << 4 );

    // Assign the bus frequency to 100 kHz:
    //
    // TPR = ( SYSCLK / ( 2 * ( 6 + 4 ) * SCL     ) ) - 1
    // TPR = ( 80 MHz / ( 2 * ( 6 + 4 ) * 100 kHz ) ) - 1
    // TPR = 39
    //
    HWREG( I2C0_BASE + I2C_O_MTPR ) = 39;

    // I2C0 Master Interrupt Mask
    // Bit 0: IM
    HWREG( I2C0_BASE + I2C_O_MIMR ) = ( 1 << 0 );

    // Enable Interrupts for I2C0
    // I2C0_IntEnable();

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : I2C_WaitForControllerReady( void )
// PURPOSE  : A blocking function to wait until the I2C controller is ready
//----------------------------------------------------------------------------

void I2C_WaitForControllerReady( void )
{
    while( !I2C_IsControllerReady() );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : I2C_IsControllerReady( void )
// PURPOSE  : A non-blocking function to check if the I2C controller is ready
//----------------------------------------------------------------------------

bool I2C_IsControllerReady( void )
{
    bool bReady = false;

    if( !( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY ) )
    {
        bReady = true;
    }

    return bReady;
}

//----------------------------------------------------------------------------
// END I2C.C
//----------------------------------------------------------------------------
