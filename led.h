//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LED.H
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

#ifndef LED_H_
#define LED_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void LED_Init( void );

void LED_LED1( bool bOn );
void LED_LED2( bool bOn );
void LED_LED3( bool bOn );
bool LED3_state(void);

void LED_FSM( bool bReset,uint8_t FP );

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define LED_TIME_HBON    250 // Heartbeat On-Time
#define LED_TIME_HBOFF   750// Heartbeat Off-Time

#define LED_TIME_FPON    125 // FastPulse On-Time
#define LED_TIME_FPOFF   375 // Fastpulse Off-Time

#define LED_TIME_PAUSE1  2000 // Pause 1 Time
#define LED_TIME_PAUSE2  2000 // Pause 2 Time


enum LED_FSM_STATES
{
    LED_FSM_RESET = 0,
    LED_FSM_PAUSE1,
    LED_FSM_FPON,
    LED_FSM_FPOFF,
    LED_FSM_PAUSE2,
    LED_FSM_HBON,
    LED_FSM_HBOFF,

};

#endif // LED_H_

//----------------------------------------------------------------------------
// END LED.H
//----------------------------------------------------------------------------
