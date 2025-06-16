//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : QEI.C
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
// Quadrature encoder interface support functions.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "qei.h"
#include "motor.h"
#include "uart.h"
//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

extern MOTOR_CONTROL_PARAMS g_MCP;

//----------------------------------------------------------------------------
// FUNCTION : QEI0_IntHandler( void )
// PURPOSE  : Interrupt handler for QEI0 (control loop timer)
//----------------------------------------------------------------------------

void QEI0_IntHandler( void )
{
    // Acknowledge the interrupt
    HWREG( QEI0_BASE + QEI_O_ISC ) |= ( 1 << 1 );

    // Control the motor
    MOTOR_PID( &g_MCP );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : QEI_Init( float fdt )
// PURPOSE  : Quadrature encoder interface initialization.
//----------------------------------------------------------------------------

void QEI_Init( float fdt )
{
    // Configure the Quadrature Encoder Interface peripheral
    HWREG( SYSCTL_SRQEI ) |=  0x00000001; // Reset the QEI peripheral
    HWREG( SYSCTL_SRQEI ) &= ~0x00000001; // Remove reset

    HWREG( SYSCTL_RCGCQEI  ) |= 0x00000001; // Enable Clock for QEI 0
    HWREG( SYSCTL_RCGCGPIO ) |= 0x00000008; // Enable Clock for Port D

    // PD7 is a special pin (locked by default) - unlock PD7
    HWREG( GPIO_PORTD_BASE + GPIO_O_GPIOLOCk ) = 0x4C4F434B;
    HWREG( GPIO_PORTD_BASE + GPIO_O_GPIOCR   ) = 0x80;

    // Configure PD6 and PD7 for QEI 0
    HWREG( GPIO_PORTD_BASE + GPIO_O_DEN   ) |=  0xC0;
    HWREG( GPIO_PORTD_BASE + GPIO_O_DIR   ) &= ~0xC0;
    HWREG( GPIO_PORTD_BASE + GPIO_O_AFSEL ) |=  0xC0;
    HWREG( GPIO_PORTD_BASE + GPIO_O_PCTL  ) &=  0x00FFFFFF;
    HWREG( GPIO_PORTD_BASE + GPIO_O_PCTL  ) |=  0x66000000;

    // Calculate the load value based on the "delta t" argument fdt:
    uint32_t uiLoad = ( uint32_t )( ( 80000000.0f * fdt ) + 0.5f ) - 1UL;

    // Configure QEI0 (80 MHz System Clock)
    HWREG( QEI0_BASE + QEI_O_CTL  )  = 0x00000628;
    HWREG( QEI0_BASE + QEI_O_LOAD )  = uiLoad;
    HWREG( QEI0_BASE + QEI_O_CTL  ) |= 0x00000001;

    // Enable the QEI0 interrupt (timer only)
    HWREG( QEI0_BASE + QEI_O_INTEN ) = ( 1 << 1 );
    HWREG( NVIC_EN0 ) = ( 1 << 13 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : QEI_GetSpeed( void )
// PURPOSE  : Returns the speed of the motor output shaft in RPM.
//----------------------------------------------------------------------------

float QEI_GetSpeed( void )
{
    // Calculate the speed of the output shaft in RPM (rotations per minute)
    // The motor encoder produces 7 pulses per motor shaft revolution
    // QEI0 is configured to produce 4 counts per pulse:
    //      1 count for PhA rising
    //      1 count for PhB rising
    //      1 count for PhA falling
    //      1 count for PhB falling
    // The gear reduction for the output shaft is 60:1
    //
    // The SPEED register contains the number of counts within the timer interval. The
    // timer interval (LOAD register) contains 80 MHz – 1 meaning that the SPEED register
    // will be updated every second containing the number of counts/s.
    //
    // The number of rotations of the motor shaft per second is:
    //
    //     RPSmot = SPEED / ( 4 * 7 )
    //
    // The number of rotations of the motor shaft per minute is:
    //
    //     RPMmot = RPSmot * 60
    //
    // The number of rotations of the output shaft per minute is:
    //
    //     RPMout = RPMmot / GRmot
    //
    // Note: This last division is dependent on the gear reducer. For example
    //       the gear reduction (GRmot) is,
    //
    //       Model         Ratio     GRmot
    //       -----------   ------    -----
    //       SPG30E-20K    1:20        20
    //       SPG30E-30K    1:30        30
    //       SPG30E-60K    1:60        60 <-- Example (see below)
    //       SPG30E-150K   1:120      120
    //       SPG30E-200K   1:200      200
    //       SPG30E-300K   1:270      270

    float fSPEED  = ( float )HWREG( QEI0_BASE + QEI_O_SPEED );
    fSPEED/=g_MCP.fdt;
    float fRPSmot = fSPEED / ( 4.0f * 7.0f );
    float fRPMmot = fRPSmot * 60.0f;
    float fGRmot  = 20.0f; // SPG30E-60K
    float fRPMout = fRPMmot / fGRmot;

    return fRPMout;
}

//----------------------------------------------------------------------------
// END QEI.C
//----------------------------------------------------------------------------
