/*
 * LCD_Reset_Cause.c
 *
 *  Created on: Nov 8, 2024
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
#include "stdio.h"
#include"count.h"
#include "lcd.h"
#include "LCD_Reset_Cause.h"
#include "Contrast.h"

char g_sBuffer[80];
uint16_t aValues[3];
uint16_t bValues[6];




enum LCD_Reset_Cause
{
    CSCRN_EL507 = 1, CSCRN_VOLTAGE, CSCRN_CURRENT, CSCRN_POWER, CSCRN_TEMP
};

void LCD_Reset_Cause(uint8_t m)
{
    uint8_t Cause = 0;
    Cause = m;

    switch ((uint8_t) Cause)
    {
    case 3:
    {
        TERM_SaveCursor();
        TERM_SetPos(17, 10);
        UART_SendMessage("EXTERNAL RESET\r\n");
        TERM_RestoreCursor();
        break;
    }
    case 4:
    {
        TERM_SaveCursor();
        TERM_SetPos(17, 10);
        UART_SendMessage("POWER FAILURE\r\n");
        TERM_RestoreCursor();
        break;
    }
    case 5:
    {
        TERM_SaveCursor();
        TERM_SetPos(17, 10);
        UART_SendMessage("BROWN-OUT\r\n");
        break;
    }

    case 7:
    {
        TERM_SaveCursor();
        TERM_SetPos(17, 10);
        //UART_SendMessage( "\e[K" );
        UART_SendMessage("SOFTWARE RESET\r\n");
        TERM_RestoreCursor();
        break;
    }
    case 6:
    {
        TERM_SaveCursor();
        TERM_SetPos(17, 10);
        UART_SendMessage("WATCHDOG TIMEOUT\r\n");
        TERM_RestoreCursor();
        break;
    }

    case 8:
    {
        TERM_SaveCursor();
        TERM_SetPos(17, 10);
        UART_SendMessage("OSCILLATOR FAILURE\r\n");
        TERM_RestoreCursor();
        break;
    }
    default:
        break;

    }

}

//**************************************//

// Function shows the voltage in the LCD Screen

//**************************************//

void Screentime(uint8_t *g_aRTCData)
{
    sprintf(g_sBuffer, "%02X:%02X:%02X", g_aRTCData[2]& 0x1f, g_aRTCData[1]& 0x7f, g_aRTCData[0]& 0x7f );
    LCD_SendInstruction( LCD_IC_DDRAMADDR + 0x00 + 6);
    LCD_SendMessage(g_sBuffer);

    sprintf(g_sBuffer, "20%02X:%02X:%02X",  g_aRTCData[6], g_aRTCData[5]& 0x1f,g_aRTCData[4]& 0x3f );
    LCD_SendInstruction( LCD_IC_DDRAMADDR + 0x40 + 5);
   LCD_SendMessage(g_sBuffer);


}
void MotorSpeed(float speed)
{
    char Motor_Speed[10];
    sprintf(Motor_Speed, "%5.1f", speed);
    LCD_SendInstruction( LCD_IC_DDRAMADDR + 0x00 + 7);
    LCD_SendMessage(Motor_Speed);
}



