//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MOTOR.H
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
// INCLUSION LOCK
//----------------------------------------------------------------------------

#ifndef MOTOR_H_
#define MOTOR_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"

//----------------------------------------------------------------------------
// STRUCTURES
//----------------------------------------------------------------------------

typedef struct tagMOTOR_CONTROL_PARAMS
{
    bool  bDir; // Direction

    float fSP;  // Setpoint (RPM)
    float fPV;  // Process Variable (RPM)

    float fKP;  // Proportional Constant
    float fKI;  // Integral Constant
    float fKD;  // Derivative Constant

    float fIntegral;    // Accumulated Error
    float fPrevError;   // Previous Error
    float fdt;          // Control Interval ("delta t")

} MOTOR_CONTROL_PARAMS;

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------
uint8_t Motor_Mode;
void  MOTOR_Init( MOTOR_CONTROL_PARAMS *pMCP );
void  MOTOR_PID( MOTOR_CONTROL_PARAMS *pMCP );

float MOTOR_GetDutyCycle( void );
void  MOTOR_SetDutyCycle( float fMotorSpeed, bool bMotorDir );

#endif // MOTOR_H_

//----------------------------------------------------------------------------
// END MOTOR.H
//----------------------------------------------------------------------------
