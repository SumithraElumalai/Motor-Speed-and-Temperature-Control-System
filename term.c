//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : TERM.C
// FILE VERSION : 1.0
// PROGRAMMER   : Sumithra Elumalai
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
// Code to support a VT100 compatible terminal.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "term.h"
#include "Up-Time.h"
#include "stdio.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : TERM_Init( void )
// PURPOSE  : Initializes the terminal interface
//----------------------------------------------------------------------------

void TERM_Init( void )
{
    // Clear the screen and display information
    UART_SendMessage( "\e[2J\e[2H" ); // Clear screen and home cursor
   // UART_SendMessage( "\e(B" );  // ASCII Set
   // UART_SendMessage( "\e[0m" ); // Normal Attributes
    UART_SendMessage("-------------------------------------------------------\r\n");
    UART_SendMessage( "Firmware    :   EL 507 -LAB 12 I\r\n" );
    UART_SendMessage( "System      :   Speed Control\r\n" );
    UART_SendMessage( "Company     :   Confederation College\r\n" );
    UART_SendMessage( "Platform    :   TM4C123GH6PM\r\n" );
    UART_SendMessage( "Date        :   2024-12-10\r\n" );
    UART_SendMessage( "Programmer  :   Sumithra Elumalai\r\n" );
    UART_SendMessage("-------------------------------------------------------\r\n");
    UART_SendMessage( "SYSTEM RESTART : " );
    UART_SendMessage( "\r\n\n" );

   // UART_SendMessage("LED 1 :");
  //  UART_SendMessage("\r\nLED 2 :");
   // UART_SendMessage("\r\nLED 3 :\r\n\n");
   // UART_SendMessage("Up-Time : 00:00\r\n");
   // UART_SendMessage("Count   : +0\r\n");
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_SetPos( uint8_t x, uint8_t y )
// PURPOSE  : Moves the cursor to the specified x and y position
//----------------------------------------------------------------------------

void TERM_SetPos( uint8_t x, uint8_t y )
{
    char sBuffer[ 16 ];
    sprintf( sBuffer, "\e[%u;%uH", y, x );
    UART_SendMessage( sBuffer );
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_SaveCursor( void )
// PURPOSE  : Save cursor and attributes
//----------------------------------------------------------------------------

void TERM_SaveCursor( void )
{
    UART_SendMessage( "\e7" );
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_RestoreCursor( void )
// PURPOSE  : Restore cursor and attributes
//----------------------------------------------------------------------------

void TERM_RestoreCursor( void )
{
    UART_SendMessage( "\e8" );
}
//----------------------------------------------------------------------------
// FUNCTION : TERM_HideCursor( void )
// PURPOSE  : Hides the cursor
//----------------------------------------------------------------------------

void TERM_HideCursor( void )
{
    UART_SendMessage( "\e[?25l" );
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_ShowCursor( void )
// PURPOSE  : Shows the cursor
//----------------------------------------------------------------------------

void TERM_ShowCursor( void )
{
    UART_SendMessage( "\e[?25h" );
}

//----------------------------------------------------------------------------
// END TERM.C
//----------------------------------------------------------------------------
