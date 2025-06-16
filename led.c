//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LED.C
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
// Controls the LED and LED sequencer using a software encoded state machine.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "led.h"
#include "term.h"
#include "uart.h"
#include "pcf8574a.h"
#include "i2c.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : LED_Init( void )
// PURPOSE  : Initializes the GPIO pin to control the LED
//----------------------------------------------------------------------------

void LED_Init( void )
{
    // Enable GPIO Port F
    HWREG( SYSCTL_RCGCGPIO ) |= 0x20;

    // Configure PF1 as an output
    HWREG( GPIO_PORTF_BASE + GPIO_O_DEN ) |= 0x0e;
    HWREG( GPIO_PORTF_BASE + GPIO_O_DIR ) |= 0x0e;
    HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x0e << 2 ) ) = 0;

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LED_LED1( bool bOn )
// PURPOSE  : Controls the state of the LED
//----------------------------------------------------------------------------

void LED_LED1( bool bOn )
{
    // Control the state of the LED based on bOn
   bool LED1_State = (HWREG(GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x02 << 2 ) ) &(0x02));
   // bool LED1_State = ((HWREG(GPIO_PORTF_BASE + GPIO_O_DATA) & 0x02) != 0);
    if(LED1_State ^ bOn)
    {
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x02 << 2 ) ) = bOn ? 0x02 : 0;

    }


    return;
}

void LED_LED2( bool bOn )
{
    // Control the state of the LED based on bOn
    bool LED2_State=HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x04 << 2 ) ) = bOn ? 0x04 : 0;
    if(LED2_State ^ bOn)
        {
           HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x04 << 2 ) ) = bOn ? 0x04 : 0;

        }


    return;
}

void LED_LED3( bool bOn )
{
    // Control the state of the LED based on bOn
    HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x08 << 2 ) ) = bOn ? 0x08 : 0;

    return;
}
bool LED3_state(void)
{
    static  bool Led3_state =0;
    HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ((1<<3 )<<2 )) ^= 0x08;
    Led3_state =     HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + (1<<5)) & 0x08;
    return Led3_state;



}
//----------------------------------------------------------------------------
// FUNCTION : LED_FSM( bool bReset )
// PURPOSE  : LED Finite State Machine control
//----------------------------------------------------------------------------

void LED_FSM( bool bReset,uint8_t FPCount )
{
    static uint8_t uiSB = 0;
    static uint8_t current= 0;
    static uint16_t uiTimer = 0;

    // Reset the state machine if necessary
    if( bReset )
    {
        uiSB = LED_FSM_RESET;
        current = FPCount;
    }

    // Clock the state machine
    switch( uiSB )
    {

    case LED_FSM_RESET:

        LED_LED1( 0 );
        uiSB    = LED_FSM_PAUSE1;
        uiTimer = LED_TIME_PAUSE1;

        break;

    case LED_FSM_PAUSE1:

        LED_LED1( 0 );

        if( !--uiTimer )
        {
            uiSB    = LED_FSM_FPON;
            uiTimer = LED_TIME_FPON;
        }

        break;

    case LED_FSM_FPON:

            LED_LED1( 1 );

            if( !--uiTimer )
            {
                uiSB    = LED_FSM_FPOFF;
                uiTimer = LED_TIME_FPOFF;
            }

            break;

    case LED_FSM_FPOFF:

            LED_LED1( 0 );

            if( !--uiTimer )
            {
                --current;

                if(current >0)
                {
                    uiSB    = LED_FSM_FPON;
                    uiTimer = LED_TIME_FPON;
                }
                else
                {

                uiSB    = LED_FSM_PAUSE2;
                uiTimer = LED_TIME_PAUSE2;
            }

            }

            break;


    case LED_FSM_PAUSE2:

           LED_LED1( 0 );

           if( !--uiTimer )
           {
               uiSB    = LED_FSM_HBON;
               uiTimer = LED_TIME_HBON;
           }

           break;

    case LED_FSM_HBON:

        LED_LED1( 1 );

        if( !--uiTimer )
        {
            uiSB    = LED_FSM_HBOFF;
            uiTimer = LED_TIME_HBOFF;
        }

        break;

    case LED_FSM_HBOFF:

        LED_LED1( 0 );

        if( !--uiTimer )
        {
            uiSB    = LED_FSM_HBON;
            uiTimer = LED_TIME_HBON;
        }

        break;

    default: // Unknown State

        LED_LED1( 0 );

        uiSB = LED_FSM_RESET;

        break;

    }

    return;
}

//----------------------------------------------------------------------------
// END LED.C
//----------------------------------------------------------------------------
