//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LCD.C
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
// LCD Module Driver API.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <lcd.h>
#include <systick.h>
#include "uart.h"

//----------------------------------------------------------------------------
// FUNCTION : LCD_Init( void )
// PURPOSE  : Initializes the LCD module
//----------------------------------------------------------------------------

void LCD_Init( void )
{
    // GPIO Port A and E Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCGPIO ) |= 0x11;

    // Configure Data Bus (initially input)
    HWREG( GPIO_PORTA_BASE + GPIO_O_DEN  ) |= LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DIR  ) &= ~LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_PDR  ) |= LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DR4R ) |= LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( LCD_BUS << 2 ) ) = 0;

    // Configure Control Signals
    HWREG( GPIO_PORTE_BASE + GPIO_O_DEN  ) |= LCD_CONTROL;
    HWREG( GPIO_PORTE_BASE + GPIO_O_DIR  ) |= LCD_CONTROL;
    HWREG( GPIO_PORTE_BASE + GPIO_O_DR4R ) |= LCD_CONTROL;
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_CONTROL << 2 ) ) = 0;

    // Turn power off to the LCD module
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_PWR << 2 ) ) = 0;

    // Delay prior to power-up
    SYSTICK_Delay( 10 );

    // Power-up the LCD module
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_PWR << 2 ) ) = LCD_PWR;

    // Delay after power-up
    SYSTICK_Delay( 50 );

    // Set to 4-bit mode
    LCD_WriteNibble( 0, LCD_IC_FUNCTION );
    SYSTICK_Delay( 1 );

    // Configure LCD Module
    LCD_SendInstruction( LCD_IC_FUNCTION | LCD_IC_FUNCTION_2LINE | LCD_IC_FUNCTION_ON );
    LCD_SendInstruction( LCD_IC_DISPLAY | LCD_IC_DISPLAY_ON );
    LCD_SendInstruction( LCD_IC_CLEAR );
    LCD_SendInstruction( LCD_IC_ENTRYMODE | LCD_IC_ENTRYMODE_INC );
    LCD_Write( 0, LCD_IC_CGRAMADDR + 0x00 ); // Box Character (Accessible as 0x08)
           LCD_Write( 1, 0x06 ); // ..##.
           LCD_Write( 1, 0x09 ); // #...#
           LCD_Write( 1, 0x09 ); // #...#
           LCD_Write( 1, 0x06 ); // ..##.
           LCD_Write( 1, 0x00 ); // .....
           LCD_Write( 1, 0x00 ); // .....
           LCD_Write( 1, 0x00 ); // .....
           LCD_Write( 1, 0x00 ); // .....

           LCD_Write( 0, LCD_IC_DDRAMADDR ); // Get out of CGRAM mode
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_SendMessage( char* sMessage )
// PURPOSE  : Writes a zero-terminated string to the LCD module
//----------------------------------------------------------------------------

void LCD_SendMessage( char* sMessage )
{
    // Send entire message at the current cursor position until
    // the NULL character is reached

    while( *sMessage )
    {
        LCD_Write( 1, ( uint8_t )*sMessage++ );
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_SendInstruction( uint8_t uiInstruction )
// PURPOSE  : Writes one 8-bit instruction to the LCD module
//----------------------------------------------------------------------------

void LCD_SendInstruction( uint8_t uiInstruction )
{
    LCD_Write( 0, uiInstruction );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_WaitForReady( void )
// PURPOSE  : Returns only when the LCD module is ready
//----------------------------------------------------------------------------

void LCD_WaitForReady( void )
{
    while( LCD_Read( 0 ) & LCD_IC_STATUS_BUSY );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_Write( uint8_t uiRS, uint8_t uiData )
// PURPOSE  : Writes one byte to the LCD module
//----------------------------------------------------------------------------

void LCD_Write( uint8_t uiRS, uint8_t uiData )
{
    LCD_WaitForReady();

    LCD_WriteNibble( uiRS, uiData );
    LCD_WriteNibble( uiRS, uiData << 4 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_Read( uint8_t uiRS )
// PURPOSE  : Reads one byte from the LCD module
//----------------------------------------------------------------------------

uint8_t LCD_Read( uint8_t uiRS )
{
    uint8_t uiData;

    uiData = LCD_ReadNibble( uiRS );
    uiData |= ( ( LCD_ReadNibble( uiRS ) >> 4 ) & 0x0F );

    return uiData;
}

//----------------------------------------------------------------------------
// END LCD.C
//----------------------------------------------------------------------------
