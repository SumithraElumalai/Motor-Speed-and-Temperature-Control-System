//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : SWITCH.C
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
// Code to support switch inputs.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "switch.h"
#include "global.h"
#include "uart.h"
//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : void SWITCH_Init( void )
// PURPOSE  : Initializes the switches
//----------------------------------------------------------------------------

void SWITCH_Init( void )
{

    // Enable GPIO Port F
    HWREG(SYSCTL_RCGCGPIO) |= 0x20;

    // Unlock the Port F Commit Register to enable modification
    HWREG(GPIO_PORTF_BASE + GPIO_O_GPIOLOCk)  = 0x4C4F434B;
    HWREG(GPIO_PORTF_BASE + GPIO_O_GPIOCR  ) |= 0x01;

    // Configure PF0 (SW3) and PF4 (SW2) as inputs
    HWREG(GPIO_PORTF_BASE + GPIO_O_DEN) |= 0x11;
    HWREG(GPIO_PORTF_BASE + GPIO_O_DIR) &= ~0x11;
    HWREG(GPIO_PORTF_BASE + GPIO_O_PUR) |= 0x11;

    return;
}

//----------------------------------------------------------------------------
// END SWITCH.C
//----------------------------------------------------------------------------
