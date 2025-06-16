/*
 * Count.c
 *
 *  Created on: Nov 4, 2024
 *      Author: selumala
 */
#include "global.h"
#include "probe.h"
#include "systick.h"
#include "sysclk.h"
#include "led.h"
#include "Init.h"
#include "term.h"
#include "Up-Time.h"
#include "uart.h"
#include "queue.h"
#include "el507.h"
#include "contact.h"
#include "switch.h"
#include "Reset_Cause.h"
#include"stdio.h"
#include "Contrast.h"

#define DEBOUNCE

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
#ifdef DEBOUNCE

CONTACT g_SW3; // PF0
CONTACT g_SW2; // PF4

#endif

static int32_t iTC = 0;

/*void UpdateTransitionCount( int32_t iTC )
 {
 // String buffer for message
 char sBuffer[32];

 TERM_SaveCursor();
 TERM_HideCursor();
 {
 TERM_SetPos( 11 , 10 );
 sprintf( sBuffer, "%+4d", iTC );
 UART_SendMessage( sBuffer );
 }
 TERM_RestoreCursor();
 TERM_ShowCursor();

 return;
 }*/

uint8_t Upcounter(void)
{
    // uint8_t x;
    if (!g_SW2.bOutput)
    {
        if (++iTC >= 4)
        {
            iTC = 1; // set the value to +25 ,dont count anything above 25
            // UpdateTransitionCount( iTC );
        }
    }
    else
    {
        // UpdateTransitionCount( iTC );
    }
    return iTC;
}
uint8_t Downcounter(void)
{
    if (!g_SW3.bOutput)
    {
        {
            if (--iTC <= 0)
            {
                iTC = 4; // set the value to -25 , dont go below the value -25
                // UpdateTransitionCount( iTC );
            }

            else
            {
                // UpdateTransitionCount( iTC );
            }
        }

    }
    return iTC;
}
