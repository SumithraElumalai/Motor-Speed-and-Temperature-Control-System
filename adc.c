//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : ADC.C
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
// Analog-to-digital converter support functions.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "adc.h"
#include "uart.h"
#include "Contrast.h"

//----------------------------------------------------------------------------
// FUNCTION : ADC_SS0_IntHandler( void )
// PURPOSE  : Sample Sequencer 0 Interrupt Handler
//----------------------------------------------------------------------------

void ADC_SS0_IntHandler( void )
{
    uint32_t uiIntStatus = HWREG( ADC0_BASE + ADC_O_ISC );

    // Check for SS0 interrupt
    if( uiIntStatus & ( 1 << 0 ) )
    {
        // Clear interrupt
        HWREG( ADC0_BASE + ADC_O_ISC ) = ( 1 << 0 );

        // Flag ADC SS0 conversion complete
        GLOBAL_SetSysFlag( SYSFLAGS_ADC_SS0 );
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : ADC_Init( void )
// PURPOSE  : Initializes the ADC0 peripheral
//----------------------------------------------------------------------------

void ADC_Init( void )
{
    // From table 13-1 (page 801 of the TM4C123GH6PM datasheet)
    // AIN4 is assigned to pin PD3 (pin 64)

    // Enable the run-mode clock for ADC0
    HWREG( SYSCTL_RCGCADC ) |= 0x01;

    // Enable the run-mode clock for GPIO Port D
    HWREG( SYSCTL_RCGCGPIO ) |= 0x08;

    // Configure pin 64 (PD3) to be an analog input
    HWREG( GPIO_PORTD_BASE + GPIO_O_AFSEL ) |= 0x0C;
    HWREG( GPIO_PORTD_BASE + GPIO_O_DEN   ) &= ~0x0C;

    // Configure PD3 as analog (AIN4 as per table 23-5 on page 1351 of the TM4C123GH6PM datasheet)
    HWREG( GPIO_PORTD_BASE + GPIO_O_AMSEL ) |= 0x0C;

    // Disable SS0
    HWREG( ADC0_BASE + ADC_O_ACTSS ) &= ~( 1 << 0 );

    // Ensure trigger for SS0 is the processor
    HWREG( ADC0_BASE + ADC_O_EMUX ) &= 0x0000FFF0;

    // Assign priority for SS0 as highest
    HWREG( ADC0_BASE + ADC_O_SSPRI ) &= 0x00003330;

    // Configure Sample Sequencer 0 (two samples):
    //
    // Sample  Analog Input          Interrupt
    // ------  --------------------  ---------
    //   0     AIN4                  No
    //   1     Internal Temperature  Yes

    HWREG( ADC0_BASE + ADC_O_SSMUX0 ) = 0x00000054;
    HWREG( ADC0_BASE + ADC_O_SSCTL0 ) = 0x00000E00;

    // Enable hardware averaging (64x)
    HWREG( ADC0_BASE + ADC_O_SAC ) = 6;

    // Set the dither bit (noise reduction)
    HWREG( ADC0_BASE + ADC_O_CTL ) |= ( 1 << 6 );

    // Enable SS0
    HWREG( ADC0_BASE + ADC_O_ACTSS ) |= ( 1 << 0 );

    // ADC Interrupt Mask
    // Bit 0: SS0 Interrupt Mask
    HWREG( ADC0_BASE + ADC_O_ISC ) |= ( 1 << 0 );
    HWREG( ADC0_BASE + ADC_O_IM  ) |= ( 1 << 0 );

    // Enable Interrupts for ADC0 SS0 (Table 2-9 on page 104 of TM4C123GH6PM datasheet)
    HWREG( NVIC_EN0 ) = ( 1 << 14 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : ADC_SS0_Trigger( void )
// PURPOSE  : Triggers (starts) a sample sequence with SS0
//----------------------------------------------------------------------------

void ADC_SS0_Trigger( void )
{
    HWREG( ADC0_BASE + ADC_O_PSSI ) |= ( 1 << 0 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : ADC_SS0_Read( uint16_t* pValues, uint8_t uiNumValues )
// PURPOSE  : Reads converted values into an array
//----------------------------------------------------------------------------

uint8_t ADC_SS0_Read( uint16_t* pValues, uint8_t uiNumValues )
{
    uint8_t uiValuesRead = 0;

    while( !( HWREG( ADC0_BASE + ADC_O_SSFSTAT0 ) & ( 1 << 8 ) ) && uiNumValues )
    {
        pValues[ uiValuesRead++ ] = ( uint16_t )( HWREG( ADC0_BASE + ADC_O_SSFIFO0 ) & 0x00000FFF );
        uiNumValues--;
    }

    return uiValuesRead;
}

//----------------------------------------------------------------------------
// END ADC.C
//----------------------------------------------------------------------------
