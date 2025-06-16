//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : PCF8574A.H
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

#ifndef PCF8574A_H_
#define PCF8574A_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

// Slave Address
#define PCF8574A_SA 0x38

// P7 = 1: LED4
// P6 = 1: LED5
// P5 = 1: LED6
// P4 = 0: Not Used (Reserved 1)
// P3 = 0: Not Used (Reserved 2)
// P2 = 1: SW6
// P1 = 1: SW5
// P0 = 1: SW4

#define PCF8574A_LED4 0x80
#define PCF8574A_LED5 0x40
#define PCF8574A_LED6 0x20
#define PCF8574A_RES2 0x10
#define PCF8574A_RES1 0x08
#define PCF8574A_SW6  0x04
#define PCF8574A_SW5  0x02
#define PCF8574A_SW4  0x01

#define PCF8574A_OUTPUTS ( PCF8574A_LED4 | PCF8574A_LED5 | PCF8574A_LED6 | PCF8574A_RES1 | PCF8574A_RES2 )
#define PCF8574A_INPUTS ( PCF8574A_SW6 | PCF8574A_SW5 | PCF8574A_SW4 )

#define PCF8574A_CONFIG ( PCF8574A_OUTPUTS | PCF8574A_INPUTS )

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void PCF8574A_Init( void );
void PCF8574A_Write( uint8_t uiSA, uint8_t uiData );
void PCF8574A_Read( uint8_t uiSA, uint8_t* puiData );

void SwitchPressed_SW4(bool Toggle);
void SwitchPressed_SW5(bool Toggle);
void SwitchPressed_SW6(bool Toggle);

#endif // PCF8574A_H_

//----------------------------------------------------------------------------
// END PCF8574A.H
//----------------------------------------------------------------------------
