//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : SYSTICK.C
// FILE VERSION : 1.0
// PROGRAMMER   :  selumala
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
// Configures a 1 ms system tick signal that also appears at PA2.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "systick.h"
#include <lcd.h>
#include "uart.h"
//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : SYSTICK_IntHandler( void )
// PURPOSE  : Interrupt handler
//----------------------------------------------------------------------------

void SYSTICK_IntHandler( void )
{
    // Generate a tick pulse at PA2
    HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( 0x04 << 2 ) ) = 0x04;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( 0x04 << 2 ) ) = 0;

    // Set a global flag to indicate that a system tick interval has elapsed
    GLOBAL_SetSysFlag( SYSFLAGS_SYS_TICK );
}

//----------------------------------------------------------------------------
// FUNCTION : SYSTICK_Init( void )
// PURPOSE  : Initializes the system tick (and external signal at PA2)
//----------------------------------------------------------------------------

void SYSTICK_Init( void )
{
    // Enable GPIO Port A
    HWREG( SYSCTL_RCGCGPIO  ) |= 0x01;

    // Configure PA2 as an output (TICK signal)
    HWREG( GPIO_PORTA_BASE + GPIO_O_DEN   ) |= 0x04;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DIR   ) |= 0x04;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( 0x04 << 2 ) ) = 0x04;

    // Configure System Tick
    HWREG( NVIC_ST_CURRENT ) = 0;
    HWREG( NVIC_ST_RELOAD  ) = 79998 - 1;
    HWREG( NVIC_ST_CTRL    ) = 0x07;

    return;
}
//----------------------------------------------------------------------------
// FUNCTION : SYSTICK_Delay( uint32_t uiDelay )
// PURPOSE  : Returns when the specified number of ticks has elapsed
//

void SYSTICK_Delay( uint32_t uiDelay )
{
    while( uiDelay )
    {
        while( !GLOBAL_CheckSysFlag( SYSFLAGS_SYS_TICK ) );
        GLOBAL_ClearSysFlag( SYSFLAGS_SYS_TICK );
        uiDelay--;
    }

    return;
}

//----------------------------------------------------------------------------
// END SYSTICK.C
//----------------------------------------------------------------------------
