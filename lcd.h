//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LCD.H
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

#ifndef LCD_H_
#define LCD_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void       LCD_Init( void );
void       LCD_WaitForReady( void );
void       LCD_SendMessage( char* sMessage );
void       LCD_SendInstruction( uint8_t uiInstruction );

uint8_t    LCD_Read( uint8_t uiRS );
uint8_t    LCD_ReadNibble( uint8_t uiRS );

void       LCD_Write( uint8_t uiRS, uint8_t uiData );
void       LCD_WriteNibble( uint8_t uiRS, uint8_t uiData );

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

// Port A (Data)
#define LCD_BUS ( 0x80 | 0x40 | 0x20 | 0x10 )

// Port E (Control)
#define LCD_RS  0x08
#define LCD_RW  0x04
#define LCD_E   0x02
#define LCD_PWR 0x01

#define LCD_CONTROL ( LCD_RS | LCD_RW | LCD_E | LCD_PWR )

// Instruction Codes
#define LCD_IC_CLEAR       0x01 // Clear Display
#define LCD_IC_HOME        0x02 // Return Home
#define LCD_IC_ENTRYMODE   0x04 // Entry Mode Set
#define LCD_IC_DISPLAY     0x08 // Display On/Off Control
#define LCD_IC_CURSORSHIFT 0x10 // Cursor or Display Shift
#define LCD_IC_FUNCTION    0x20 // Function Set
#define LCD_IC_CGRAMADDR   0x40 // Set CGRAM Address
#define LCD_IC_DDRAMADDR   0x80 // Set DDRAM Address

// Instruction Code Bits
#define LCD_IC_ENTRYMODE_INC       0x02
#define LCD_IC_ENTRYMODE_SHIFT     0x01

#define LCD_IC_DISPLAY_ON          0x04
#define LCD_IC_DISPLAY_CURSOR      0x02
#define LCD_IC_DISPLAY_BLINK       0x01

#define LCD_IC_CURSORSHIFT_DISPLAY 0x08
#define LCD_IC_CURSORSHIFT_RIGHT   0x04

#define LCD_IC_FUNCTION_8BIT       0x10
#define LCD_IC_FUNCTION_2LINE      0x08
#define LCD_IC_FUNCTION_ON         0x04

#define LCD_IC_STATUS_BUSY         0x80

#endif // LCD_H_

//----------------------------------------------------------------------------
// END LCD.H
//----------------------------------------------------------------------------
