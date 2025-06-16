//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MOTOR.C
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
// Motor control support functions.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "motor.h"
#include "qei.h"
#include "uart.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------
MOTOR_CONTROL_PARAMS g_MCP;

//----------------------------------------------------------------------------
// FUNCTION : MOTOR_Init( MOTOR_CONTROL_PARAMS *pMCP )
// PURPOSE  : Motor interface initialization.
//----------------------------------------------------------------------------

void MOTOR_Init( MOTOR_CONTROL_PARAMS *pMCP )
{
    // Configure the Pulse Width Modulation peripheral
    HWREG( SYSCTL_RCGCPWM )  |= 0x00000001; // Enable Clock for PWM 0
    HWREG( SYSCTL_RCGCGPIO ) |= 0x00000002; // Enable Clock for Port B

    // Configure PB6 and PB7 for PWM 0
    HWREG( GPIO_PORTB_BASE + GPIO_O_DEN   ) |=  0xC0;
    HWREG( GPIO_PORTB_BASE + GPIO_O_DIR   ) &= ~0xC0;
    HWREG( GPIO_PORTB_BASE + GPIO_O_AFSEL ) |=  0xC0;
    HWREG( GPIO_PORTB_BASE + GPIO_O_PCTL  ) &=  0x00FFFFFF;
    HWREG( GPIO_PORTB_BASE + GPIO_O_PCTL  ) |=  0x44000000;
    HWREG( SYSCTL_RCC ) &= ~0x001E0000; // Clear PWM Divisor
    HWREG( SYSCTL_RCC ) |=  0x00100000; // Use PWM Divisor (System Clock / 2)

    // System Clock / 2 = 40 MHz
    // Clear PWM0 Control Register
    HWREG( PWM0_BASE + PWM_O_0_CTL  ) = 0;
    HWREG( PWM0_BASE + PWM_O_0_LOAD ) = 2000-1; // Set Period to 20 kHz (40 MHz / 2000)

    // Initialize Motor Control Parameters (change as required)
    pMCP->bDir = 1;

    pMCP->fSP  = 0.0f;
    pMCP->fPV  = 0.0f;

    pMCP->fKP  = 0.005f;
    pMCP->fKI  = 0.0f;
    pMCP->fKD  = 0.0f;

    pMCP->fIntegral  = 0.0f;
    pMCP->fPrevError = 0.0f;
    pMCP->fdt = 0.15f; // 1 s control interval

    // Start with the motor off and ready for operation
    MOTOR_SetDutyCycle( 0.0f, pMCP->bDir );

    // Enable PWM0 outputs
    HWREG( PWM0_BASE + PWM_O_0_CTL  ) = 0x00000001;
    HWREG( PWM0_BASE + PWM_O_ENABLE )|= 0x00000003; // pwm0A' and pwm0B'

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MOTOR_PID( MOTOR_CONTROL_PARAMS *pMCP )
// PURPOSE  : Provides motor PID control.
//----------------------------------------------------------------------------

void MOTOR_PID( MOTOR_CONTROL_PARAMS *pMCP )
{
    if( pMCP->fdt > 0.0f )
    {
        // Get the current speed
        pMCP->fPV = QEI_GetSpeed();

        // Determine error
        float fError = pMCP->fSP - pMCP->fPV;

        // Proportional
        float fPout = pMCP->fKP * fError;

        // Integral
        pMCP->fIntegral += fError * pMCP->fdt;
        float fIout = pMCP->fKI * pMCP->fIntegral;

        // As an option, if the error is within an acceptable limit, the
        // accumulated error (pMCP->fIntegral) can be zeroed.

        // Derivative
        float fDerivative = ( fError - pMCP->fPrevError ) / pMCP->fdt;
        float fDout = pMCP->fKD * fDerivative;

        // Update previous error
        pMCP->fPrevError = fError;

        // Determine control adjustment
        float fAdj = fPout + fIout + fDout;

        // Adjust the duty cycle of the motor
        float fDC = MOTOR_GetDutyCycle() + fAdj;
        MOTOR_SetDutyCycle( fDC, pMCP->bDir );
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MOTOR_GetDutyCycle( void )
// PURPOSE  : Returns the current normalized duty cycle (0.0 to 1.0).
//----------------------------------------------------------------------------

float MOTOR_GetDutyCycle( void )
{
    // Obtain the current duty cycle
    float fCMPA = HWREG( PWM0_BASE + PWM_O_0_CMPA );
    float fLOAD = HWREG( PWM0_BASE + PWM_O_0_LOAD );

    return fCMPA / fLOAD;
}

//----------------------------------------------------------------------------
// FUNCTION : MOTOR_SetDutyCycle( float fMotorDC, bool bMotorDir )
// PURPOSE : Sets the duty cycle and motor direction.
//----------------------------------------------------------------------------

void MOTOR_SetDutyCycle( float fMotorDC, bool bMotorDir )
{
    uint16_t uiPulse;
    uint16_t uiBSH = 50; // Time required to replenish bootstrap capacitor

    // Bootstrap (High Side)
    uint16_t uiPulseMax = ( uint16_t )( HWREG( PWM0_BASE + PWM_O_0_LOAD ) );

    // Set Direction
    if( bMotorDir )
    {
        HWREG( PWM0_BASE + PWM_O_0_GENA ) = 0x00000083; // Q6/Q8 PWM
        HWREG( PWM0_BASE + PWM_O_0_GENB ) = 0x000000C3; // Q5 OFF, Q7 ON
    }
    else
    {
        HWREG( PWM0_BASE + PWM_O_0_GENA ) = 0x000000C3; // Q6 OFF, Q8 ON
        HWREG( PWM0_BASE + PWM_O_0_GENB ) = 0x00000083; // Q5/Q7 PWM
    }

    // Verify minimum and maximum (0.0 to 1.0)
    fMotorDC = fMotorDC < 0.0f ? 0.0f : fMotorDC;
    fMotorDC = fMotorDC > 1.0f ? 1.0f : fMotorDC;

    // Calculate pulse width
    uiPulse = ( uint16_t )( uiPulseMax * fMotorDC + 0.5f );

    // Limit maximum pulse width so that the bootstrap capacitor can charge
    if( uiPulse > ( uiPulseMax - uiBSH ) ) uiPulse = uiPulseMax - uiBSH;

    // Set motor duty cycle
    HWREG( PWM0_BASE + PWM_O_0_CMPA ) = uiPulse;

    return;
}

//----------------------------------------------------------------------------
// END MOTOR.C
//----------------------------------------------------------------------------
