//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : SYSCLK.C
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
// Configures the system clock.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "sysclk.h"
#include "uart.h"
//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : SYSCLK_Init( void )
// PURPOSE  : Initializes the system clock
//----------------------------------------------------------------------------

void SYSCLK_Init( void )
{
    // By default (after power-on or reset of the microcontroller) the
    // precision internal oscillator (PIOSC) is selected as the system clock
    // at 16 MHz - no initialization is required when using PIOSC.

    // To enable the main oscillator (MOSC), clear the main oscillator disable
    // bit (MOSCDIS) in the Run-Mode Clock Configuration (RCC) register
    // (page 254)
    __delay_cycles(8000000);
    HWREG( SYSCTL_RCC ) &= ~0x01;

    // Wait for main oscillator to start up by polling the MOSC Power-Up Raw
    // Interrupt Status bit (MOSCPUPRIS) in the Raw Interrupt Status (RIS)
    // register (page 244)
    while( !( HWREG( SYSCTL_RIS ) & ( 1 << 8 ) ) );

    // When the main oscillator has stabilized, it can be routed to the System
    // Clock using the oscillator source field (OSCSRC) in the Run-Mode Clock
    // Configuration (RCC) register (page 254)
    HWREG( SYSCTL_RCC ) &= ~( 0x03 << 4 );
    // The PLL block allows for operation at higher frequencies than the
     // oscillator produces. The microcontroller is capable of operating at
     // clock rates up to 80 MHz therefore care must be taken to ensure this
     // limit is not exceeded.
     // Assign the XTAL field value of 0x15 for a 16 MHz crystal. This value
     // is used by the PLL to produce a 400 MHz output from a 16 MHz input.
     // (Table 24-20, page 1380)
     HWREG( SYSCTL_RCC ) &= ~( 0x1F << 6 ); // Clear current crystal value
     HWREG( SYSCTL_RCC ) |= ( 0x15 << 6 ); // Configure for a 16 MHz crystal
     // The RCC2 register takes precedence over the RCC register for any
     // similarly named bits. The RCC2 register has bits required for PLL
     // configuration that do not exist in the RCC register (page 260).
     // The 400 MHz output of the PLL must be divided by 5 to produce an 80 MHz
     // output. The following statement writes the entire 32-bit value to the
     // RCC2 register therefore the PWNDN2 bit (bit 13) will also be cleared
     // thus powering-up the PLL block.
     HWREG( SYSCTL_RCC2 ) = ( 1 << 31 ) // Use RCC2
     | ( 1 << 30 ) // DIV400
    | ( 4 << 22 ) // Divisor = 5
    | ( 1 << 11 ) // Bypass the PLL for now
    | ( 1 << 14 ); // USB Power Down
     // Wait for PLL to lock
     while( !( HWREG( SYSCTL_PLLSTAT ) & 1 ) );
     // Wait until PLL has stabilized
     while( !( HWREG( SYSCTL_RIS ) & ( 1 << 6 ) ) );
     // Switch over to the PLL output as the system clock
      HWREG( SYSCTL_RCC2 ) &= ~( 1 << 11 );
      // The system clock is derived from the 16 MHz main oscillator producing
      // 400 MHz at the output of the PLL block and divided by 5 resulting in
      // an 80 MHz clock.


    return;
}

//----------------------------------------------------------------------------
// END SYSCLK.C
//----------------------------------------------------------------------------
