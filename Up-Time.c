/*
 * Up-Time.c
 *
 *  Created on: Oct 21, 2024
 *      Author: selumala
 */

#include <stdio.h>
#include "Up-Time.h"
#include "term.h"
#include "uart.h"
#include "uart.h"

/** The Up- Time function is the function used to check for how long the system is running
  after system tick interrupt occurs*/



void Up_Time(void)
{
    char buffer[6];
    int one_sec = 1000;
    static Timer uptimer ={0,0,0};
    uptimer.m_seconds ++;
    if(uptimer.m_seconds >= one_sec )
    {
        uptimer.m_seconds =0;
        if(++uptimer.seconds >=60)
        {
            uptimer.seconds =0;
            uptimer.minutes++;
        }
        sprintf(buffer,"%02d:%02d",uptimer.minutes,uptimer.seconds); // Sending the minutes and seconds value to putty
        TERM_SaveCursor();
        UART_SendMessage( "\e(B");
        UART_SendMessage( "\e[0m");
        TERM_SetPos(11,9);
        UART_SendMessage( buffer);
        TERM_RestoreCursor();
    }

}
