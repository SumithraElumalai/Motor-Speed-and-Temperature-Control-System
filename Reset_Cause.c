/*
 * Reset_Cause.c
 *
 *  Created on: Oct 28, 2024
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
#include "Reset_Cause.h"
#include "uart.h"
#include"motor.h"

/** Reset-Cause is the function which is uesd to check the reset (RESC)
  register to perfrom the LED operation**/

extern uint8_t Mainosc_failure_flag;


uint8_t Reset_Cause(void)
{
    uint8_t x;
    //External-Reset
    if(HWREG(SYSCTL_RESC) & (1<<0))
       {
           LED_FSM(1,3);
           x=3;

       }


       // Power-On Reset
       if(HWREG(SYSCTL_RESC) & (1<<1))
           {
               LED_FSM(1,4);
               x=4;
           }

       // Brown Out reset

       if(HWREG(SYSCTL_RESC) & (1<<2))
              {
                  LED_FSM(1,5);
                  x=5;
              }

       //Software_Reset
       if(HWREG(SYSCTL_RESC) & (1<<4))
                  {
                      LED_FSM(1,7);
                      x=7;
                  }


      // Watch-Dog timer
       if(HWREG(SYSCTL_RESC) & (1<<3))
             {
                  LED_FSM(1,6);
                  x=6;
             }

       if(Mainosc_failure_flag == 1)
             {
                 LED_FSM(1,8);
                 x=8;
             }
                Mainosc_failure_flag = 0;

       HWREG(SYSCTL_RESC) = 0x00;

       return x;

}
