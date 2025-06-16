//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : PROBE.C
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
// This module configures a probe signal that is derived from the system
// clock divided by 1000 and output at PB7.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "probe.h"
#include "uart.h"
//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : PROBE_Init( void )
// PURPOSE  : Initializes the probe signal
//----------------------------------------------------------------------------

void PROBE_Init(void)
{
    // Configure the ( System Clock / 1000 ) signal on PB7
    HWREG( SYSCTL_RCGCPWM )  |= 0x01; // Enable the PWM 0 module
    HWREG( SYSCTL_RCGCGPIO ) |= 0x10; // Enable GPIO Port B

    // Configure PB7 as M0PWM1
    HWREG( GPIO_PORTE_BASE + GPIO_O_DEN   ) |= 0x20;
    HWREG( GPIO_PORTE_BASE + GPIO_O_DIR   ) &= ~0x20;
    HWREG( GPIO_PORTE_BASE + GPIO_O_AFSEL ) |= 0x20;
    HWREG( GPIO_PORTE_BASE + GPIO_O_PCTL  ) &= 0xFF0FFFFF;
    HWREG( GPIO_PORTE_BASE + GPIO_O_PCTL  ) |= 0x00400000;

    // Configure PWM0 to produce a 50% duty cycle signal at System Clock/1000
    HWREG( PWM0_BASE + PWM_O_2_CTL  ) = 0;

    // Generator A not used
    HWREG( PWM0_BASE + PWM_O_2_GENA ) = 0x00000000;

    // Drive probe signal (M0PWM1) high on LOAD, low on crossing CMPA down
    HWREG( PWM0_BASE + PWM_O_2_GENB ) = 0x0000008C;

    // Probe signal is System Clock / 1000
    HWREG( PWM0_BASE + PWM_O_2_LOAD ) = 500-1;

    // Configure a 50% duty cycle
    HWREG( PWM0_BASE + PWM_O_2_CMPA ) = 250-1;

    // Comparator B is not used
    HWREG( PWM0_BASE + PWM_O_2_CMPB ) = 0;

    // Enable probe signal
    HWREG( PWM0_BASE + PWM_O_2_CTL  ) = 0x00000001;
    HWREG( PWM0_BASE + PWM_O_ENABLE ) |= 0x00000020;

    return;
}

//----------------------------------------------------------------------------
// END PROBE.C
//----------------------------------------------------------------------------
