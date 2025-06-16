//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : GLOBAL.H
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

#ifndef GLOBAL_H_
#define GLOBAL_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void GLOBAL_InitSysFlags( void );
void GLOBAL_SetSysFlag( uint32_t uiSysFlag );
void GLOBAL_ClearSysFlag( uint32_t uiSysFlag );
bool GLOBAL_CheckSysFlag( uint32_t uiSysFlag );



//----------------------------------------------------------------------------
// MACROS
//----------------------------------------------------------------------------

// Hardware Register Access Macro
#define HWREG( x )  (*( ( volatile uint32_t* )( x ) ) )
#define NUM_ELEMENTS( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )

// Bit-Band Alias Macro
// a = address in the bit-band region:
//     0x20000000-0x200FFFFF
//     0x40000000-0x400FFFFF
// b = bit number:
//     0-31
#define BBA( a, b ) ( *( ( volatile uint32_t* )                       \
                      ( ( ( uint32_t )( a ) & 0x60000000 )            \
                      | 0x02000000                                    \
                      | ( ( ( ( uint32_t )( a ) & 0x000FFFFF ) << 5 ) \
                      + ( ( ( uint8_t )( b ) & 0x1F ) << 2 ) ) ) ) )

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define SYSCTL_RIS              0x400FE050  // Raw Interrupt Status
#define SYSCTL_RCC              0x400FE060  // Run-Mode Clock Configuration
#define SYSCTL_RCC2             0x400FE070  // Run-Mode Clock Configuration 2
#define SYSCTL_MOSCCTL          0x400FE07C  // Main Oscillator Control
#define SYSCTL_PLLSTAT          0x400FE168  // PLL Status

#define SYSCTL_RCGCGPIO         0x400FE608  // General-Purpose Input/Output Run
                                            // Mode Clock Gating Control

#define SYSCTL_RCGCUART         0x400FE618  // UART Run Mode Clock Gating
                                            // Control
#define SYSCTL_SCGCUART         0x400FE718  // UART Sleep Mode Clock Gating
                                            // Control
#define SYSCTL_RCGCADC          0x400FE638  // ADC Run Mode Clock Gating
                                            // Control

#define SYSCTL_RCGCPWM          0x400FE640  // Pulse Width Modulator Run Mode
                                            // Mode Clock Gating Control

#define SYSCTL_RCGCI2C          0x400FE620  // I2C Run Mode Clock Gating
                                            // Control
#define SYSCTL_SRQEI            0x400FE544  // QEI Software Reset

#define SYSCTL_RCGCGPIO         0x400FE608  // General-Purpose Input/Output Run
                                            // Mode Clock Gating Control
#define SYSCTL_RCGCPWM          0x400FE640  // Pulse Width Modulator Run Mode
                                            // Clock Gating Control
#define SYSCTL_RCGCQEI          0x400FE644  // Quadrature Encoder Interface Run
                                            // Mode Clock Gating Control

#define GPIO_PORTA_BASE         0x40004000  // GPIO Port A
#define GPIO_PORTB_BASE         0x40005000  // GPIO Port B
#define GPIO_PORTC_BASE         0x40006000  // GPIO Port C
#define GPIO_PORTD_BASE         0x40007000  // GPIO Port D
#define GPIO_PORTE_BASE         0x40024000  // GPIO Port E
#define GPIO_PORTF_BASE         0x40025000  // GPIO Port F

#define GPIO_O_DATA             0x00000000  // GPIO Data
#define GPIO_O_DIR              0x00000400  // GPIO Direction
#define GPIO_O_AFSEL            0x00000420  // GPIO Alternate Function Select
#define GPIO_O_PUR              0x00000510  // GPIO Pull-Up Select
#define GPIO_O_DEN              0x0000051C  // GPIO Digital Enable
#define GPIO_O_PCTL             0x0000052C  // GPIO Port Control

#define PWM0_BASE               0x40028000  // Pulse Width Modulator (PWM)

#define PWM_O_0_CTL             0x00000040  // PWM0 Control
#define PWM_O_0_LOAD            0x00000050  // PWM0 Load
#define PWM_O_0_COUNT           0x00000054  // PWM0 Counter
#define PWM_O_0_CMPA            0x00000058  // PWM0 Compare A
#define PWM_O_0_CMPB            0x0000005C  // PWM0 Compare B
#define PWM_O_0_GENA            0x00000060  // PWM0 Generator A Control
#define PWM_O_0_GENB            0x00000064  // PWM0 Generator B Control
#define PWM_O_ENABLE            0x00000008  // PWM Output Enable

#define NVIC_ST_CTRL            0xE000E010  // SysTick Control and Status
#define NVIC_ST_RELOAD          0xE000E014  // SysTick Reload Value Register
#define NVIC_ST_CURRENT         0xE000E018  // SysTick Current Value Register
#define  SYSCTL_RESC            0x400FE05C

#define GPIO_O_PBORCTL          0x400FE030   // Brown out Base+off

#define Interrput_EN0          0xE000E100   //NVIC Register for interrupt

#define GPIO_O_GPIOIM          0x40004410  //GPIO Interrupt Mask (GPIOIM)

#define GPIO_O_GPIOICR         0x4000441C  //GPIO Interrupt Clear (GPIOICR)

#define GPIO_O_GPIOLOCk        0X00000520 //GPIO Lock
#define GPIO_O_GPIOCR          0X00000524  //GPIO Commit
#define APINT                  0XE000ED0C   //Application Interrupt and Reset Control

#define SYSCTL_RCGCWD          0x400FE600  //Watchdog Timer Run Mode Clock Gating Control
#define WATCHDOG0_BASE         0x40000000
#define WDT_O_PRWD             0x400FEA00
#define WDT_O_LOCK             0x00000C00
#define WDT_O_LOAD             0x00000000
#define WDT_O_CTL              0x00000008
#define WDTCTL                 0x40000008
#define  GPIO_O_AMSEL           0x00000528



#define UART0_BASE              0x4000C000  // UART

#define UART_O_DR               0x00000000  // UART Data
#define UART_O_FR               0x00000018  // UART Flag
#define UART_O_IBRD             0x00000024  // UART Integer Baud-Rate Divisor
#define UART_O_FBRD             0x00000028  // UART Fractional Baud-Rate Divisor
#define UART_O_LCRH             0x0000002C  // UART Line Control
#define UART_O_CTL              0x00000030  // UART Control
#define UART_O_IM               0x00000038  // UART Interrupt Mask
#define UART_O_RIS              0x0000003C  // UART Raw Interrupt Status
#define UART_O_MIS              0x00000040  // UART Masked Interupt Status
#define UART_O_ICR              0x00000044  // UART Interrupt Clear

// System flags (bit positions)
#define SYSFLAGS_SYS_TICK       0

#define NVIC_ST_CTRL            0xE000E010  // SysTick Control and Status
#define NVIC_ST_RELOAD          0xE000E014  // SysTick Reload Value Register
#define NVIC_ST_CURRENT         0xE000E018  // SysTick Current Value Register

#define NVIC_EN0                0xE000E100  // Interrupt Set Enable
#define NVIC_DIS0               0xE000E180  // Interrupt Clear Enable
#define GPIO_O_DR8R             0x00000508  // GPIO 8-mA Drive Select
#define GPIO_O_DR4R             0x00000504  // GPIO 4-mA Drive Select
#define GPIO_O_DR2R             0x00000500  // GPIO 2-mA Drive Select
#define GPIO_O_PDR              0x00000514  // GPIO Pull-Down Select
#define ADC0_BASE               0x40038000  // ADC

#define ADC_O_ACTSS             0x00000000  // ADC Active Sample Sequencer
#define ADC_O_IM                0x00000008  // ADC Interrupt Mask
#define ADC_O_ISC               0x0000000C  // ADC Interrupt Status and Clear
#define ADC_O_EMUX              0x00000014  // ADC Event Multiplexer Select
#define ADC_O_SSPRI             0x00000020  // ADC Sample Sequencer Priority
#define ADC_O_PSSI              0x00000028  // ADC Processor Sample Sequence Initiate
#define ADC_O_SAC               0x00000030  // ADC Sample Averaging Control
#define ADC_O_CTL               0x00000038  // ADC Control
#define ADC_O_SSMUX0            0x00000040  // ADC Sample Sequence Input Multiplexer Select 0
#define ADC_O_SSCTL0            0x00000044  // ADC Sample Sequence Control 0
#define ADC_O_SSFIFO0           0x00000048  // ADC Sample Sequence Result FIFO 0
#define ADC_O_SSFSTAT0          0x0000004C  // ADC Sample Sequence FIFO 0 Status

#define I2C0_BASE               0x40020000  // I2C0

#define I2C_O_MSA               0x00000000  // I2C Master Slave Address
#define I2C_O_MCS               0x00000004  // I2C Master Control/Status
#define I2C_O_MDR               0x00000008  // I2C Master Data
#define I2C_O_MTPR              0x0000000C  // I2C Master Timer Period
#define I2C_O_MIMR              0x00000010  // I2C Master Interrupt Mask
#define I2C_O_MRIS              0x00000014  // I2C Master Raw Interrupt Status
#define I2C_O_MMIS              0x00000018  // I2C Master Masked Interrupt Status
#define I2C_O_MICR              0x0000001C  // I2C Master Interrupt Clear
#define I2C_O_MCR               0x00000020  // I2C Master Configuration
#define I2C_O_MCLKOCNT          0x00000024  // I2C Master Clock Low Timeout Count
#define I2C_O_MBMON             0x0000002C  // I2C Master Bus Monitor
#define I2C_O_MCR2              0x00000038  // I2C Master Configuration 2
#define GPIO_O_ODR              0x0000050C  // GPIO Open Drain Select

#define DAC_MAX518              0x0000002C  // Slave address
#define PWM0_BASE               0x40028000  // Pulse Width Modulator (PWM)

#define PWM_O_ENABLE            0x00000008  // PWM Output Enable
#define PWM_O_INVERT            0x0000000C  // PWM Output Inversion
#define PWM_O_0_CTL             0x00000040  // PWM0 Control
#define PWM_O_0_LOAD            0x00000050  // PWM0 Load
#define PWM_O_0_COUNT           0x00000054  // PWM0 Counter
#define PWM_O_0_CMPA            0x00000058  // PWM0 Compare A
#define PWM_O_0_CMPB            0x0000005C  // PWM0 Compare B
#define PWM_O_0_GENA            0x00000060  // PWM0 Generator A Control
#define PWM_O_0_GENB            0x00000064  // PWM0 Generator B Control

#define PWM_O_2_CTL             0x000000C0  // PWM0 Control
#define PWM_O_2_LOAD            0x000000D0  // PWM0 Load
#define PWM_O_2_COUNT           0x000000D4  // PWM0 Counter
#define PWM_O_2_CMPA            0x000000D8  // PWM0 Compare A
#define PWM_O_2_CMPB            0x000000DC  // PWM0 Compare B
#define PWM_O_2_GENA            0x000000E0  // PWM0 Generator A Control
#define PWM_O_2_GENB            0x000000E4  // PWM0 Generator B Control

#define QEI0_BASE               0x4002C000  // QEI0

#define QEI_O_CTL               0x00000000  // QEI Control
#define QEI_O_LOAD              0x00000010  // QEI Timer Load
#define QEI_O_INTEN             0x00000020  // QEI Interrupt Enable
#define QEI_O_ISC               0x00000028  // QEI Interrupt Status and Clear
#define QEI_O_SPEED             0x0000001C  // QEI Velocity

#define NVIC_ST_CTRL            0xE000E010  // SysTick Control and Status
#define NVIC_ST_RELOAD          0xE000E014  // SysTick Reload Value Register
#define NVIC_ST_CURRENT         0xE000E018  // SysTick Current Value Register

#define NVIC_EN0                0xE000E100  // Interrupt Set Enable
#define NVIC_DIS0               0xE000E180  // Interrupt Clear Enable



// System flags (bit positions)
#define SYSFLAGS_SYS_TICK       0
#define SYSFLAGS_UART_RXD       1
#define SYSFLAGS_ADC_SS0        2
#define SYSFLAGS_I2C_READY      3

#endif // GLOBAL_H_

//----------------------------------------------------------------------------
// END GLOBAL.H
//----------------------------------------------------------------------------
