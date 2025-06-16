//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MAIN.C
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
// Main program entry.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

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
#include"count.h"
#include "lcd.h"
#include "LCD_Reset_Cause.h"
#include "adc.h"
#include "i2c.h"
#include "mcp7940m.h"
#include "pcf8574a.h"
#include "Contrast.h"

#include "motor.h"
#include "qei.h"

extern char g_sBuffer[80];
extern MOTOR_CONTROL_PARAMS g_MCP;

enum LCD_Reset_Cause
{
    CSCRN_EL507 = 1, CSCRN_TIME, CSCRN_RPM

};

// LCD Menu
extern const char g_aLCDScreens[3][2][17] = { { { "EL 507 : Lab 12" }, // Screen 1, Line 1
        { "Embedded Systems" }  // Screen 1, Line 2
},
                                              { { "Time: HH:MM:SS" }, // Screen 2, Line 1
                                                { "Date:2024-12-10" } // Screen 2, Line 2
                                              },
                                              { { "Speed :###.# RPM" }, // Screen 2, Line 1

        },

};

//extern char* QUOTE_Retrieve( void );

#define DEBOUNCE
#define USE_RTC

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
#ifdef DEBOUNCE

CONTACT g_SW3; // PF0
CONTACT g_SW2; // PF4
CONTACT g_SW4;
CONTACT g_SW5;
CONTACT g_SW6;

#endif

#ifdef USE_RTC
extern uint8_t g_aRTCData[8];
uint16_t g_uiRTCCounter;

#endif

 uint16_t aValues[3];  // array for the ADC
uint16_t bValues[2];
float test;
float fAIN4;
float fAIN4_Conv;

float UART_SS0Read[5] = { 0 };

void delay(int16_t timer)
{
    while (timer < 0)
    {
        --timer;
    }
}

//----------------------------------------------------------------------------
// FUNCTION : Initialize( void )
// PURPOSE  : Initializes the embedded system
//----------------------------------------------------------------------------

void Initialize(void)
{

    // Initialize all system flags
    GLOBAL_InitSysFlags();
    SYSTICK_Init();  // SYSTICK Initialization
    SYSCLK_Init();  //SYSCLK Initialization
    // Initialize peripherals
    PROBE_Init();    // PROBE Initialization
    LED_Init();      //LED Initialization
    UART_Init();    //UART Initialization
    TERM_Init();    // Term_Initialization
    Init();         // Initialization
    SWITCH_Init(); // Switch Initialization
    uint8_t a = Reset_Cause();  // Reset_Cause Initialization
    LCD_Reset_Cause(a); // LCD_Reset_Functionality
    LCD_Init();  // LCD Initialization
    ADC_Init();
    I2C_Init();
    PCF8574A_Init();
    Contrast_adjustment(18,0);
#ifdef USE_RTC
    g_uiRTCCounter = 1;
    MCP7940M_Init();
#endif
    MOTOR_Init(&g_MCP);

    QEI_Init(g_MCP.fdt);
    return;
}
void LCD_Display(uint8_t ITC)
{

    uint8_t uiScreen = ITC;
    switch (ITC)
    {
    case CSCRN_EL507:
    {

        // Write initial static (unchanging) portions of the screen
        LCD_SendInstruction( LCD_IC_CLEAR);
        LCD_SendMessage((char*) g_aLCDScreens[uiScreen - 1][0]);

        LCD_SendInstruction( LCD_IC_DDRAMADDR + 0x40);
        LCD_SendMessage((char*) g_aLCDScreens[uiScreen - 1][1]);
        break;
    }
    case CSCRN_TIME:
    {
        delay(1000);

        // Write initial static (unchanging) portions of the screen
        LCD_SendInstruction( LCD_IC_CLEAR);
        LCD_SendMessage((char*) g_aLCDScreens[uiScreen - 1][0]);

        LCD_SendInstruction( LCD_IC_DDRAMADDR + 0x40);
        LCD_SendMessage((char*) g_aLCDScreens[uiScreen - 1][1]);
        break;
    }
    case CSCRN_RPM:
    {
        delay(250);

        // Write initial static (unchanging) portions of the screen
        LCD_SendInstruction( LCD_IC_CLEAR);
        LCD_SendMessage((char*) g_aLCDScreens[uiScreen - 1][0]);

        LCD_SendInstruction( LCD_IC_DDRAMADDR + 0x40);
        LCD_SendMessage((char*) g_aLCDScreens[uiScreen - 1][1]);
        break;
    }

    }
}

void Automatic_mode(float fAIN4_Conv)
{

    float rpm = 180.0 * (fAIN4_Conv / (3.307f - 0.00f));
    g_MCP.fSP = rpm;
    return;

}
//----------------------------------------------------------------------------
// FUNCTION : main( void )
// PURPOSE  : Program entry
//----------------------------------------------------------------------------

void main(void)
{

    // static uint16_t flag_LED3 = 0;

    uint8_t uiScreen = 1;
    uint8_t uiConvInterval = 1;
    static uint16_t g_timer =0;
    bool g_MotorState;
    // Initialize the system
    Initialize();
    g_MCP.fSP = 0.0f;
    static uint8_t ITC = 0;
    uint16_t g_timer2 =0;
    float UART_SS0Read[5] = { 0 };

    CONTACT_Init(&g_SW3, 25);
    CONTACT_Init(&g_SW2, 25);
    CONTACT_Init(&g_SW4, 25);
    CONTACT_Init(&g_SW5, 25);
    CONTACT_Init(&g_SW6, 25);



    // Create custom characters and store them in CGRAM

    // Send "Hello World!" on line 1
    LCD_SendInstruction( LCD_IC_CLEAR);
    LCD_SendMessage((char*) g_aLCDScreens[uiScreen - 1][0]);

    LCD_SendInstruction( LCD_IC_DDRAMADDR + 0x40);
    LCD_SendMessage((char*) g_aLCDScreens[uiScreen - 1][1]);

    // Loop forever
    while (1)

    {
        asm( " wfi" );
        // Check if 1 ms has passed (check the system tick flag)
        if (GLOBAL_CheckSysFlag( SYSFLAGS_SYS_TICK))
        {
            // Clear the system tick flag
            GLOBAL_ClearSysFlag( SYSFLAGS_SYS_TICK);

            //Up_Time();

            // Process a 1 ms interval in the state machine
            LED_FSM(0, 0);

            if (!--uiConvInterval)
            {
                // Start a new set of ADC conversions on SS0
                ADC_SS0_Trigger();

                uiConvInterval = 100;
            }

               if(g_MotorState==0)
               {
                   g_timer++;
               }
                if(g_timer2)
                {
                    if( !--g_timer2 )
                    {
                        LED_LED2(0);
                    }
                }

            uint8_t uiData;

            PCF8574A_Read( PCF8574A_SA, &uiData);
            if (CONTACT_Sample(&g_SW5, uiData & (0x02)))
            {
                if (!g_SW5.bOutput)
                {
                    SwitchPressed_SW5(1);
                }
                else
                {
                    SwitchPressed_SW5(0);
                }

            }
            PCF8574A_Read( PCF8574A_SA, &uiData);
            if (CONTACT_Sample(&g_SW4, uiData & (0x01)))
            {
                if (!g_SW4.bOutput)
                {
                    SwitchPressed_SW4(1);
                    if (Motor_Mode == 0)
                    {
                        g_MotorState = 1;
                    }
                }
                else
                {
                    SwitchPressed_SW4(0);
                    if (g_timer < 1000 && g_timer > 0)
                    {
                        if (g_MCP.fSP > 180.0f)
                        {
                            g_MCP.fSP = 180.0f;
                        }
                        else
                        {
                            g_MCP.fSP = g_MCP.fSP + 1.0f;
                        }
                    }
                    if (g_timer >= 1000)
                    {
                        if ((g_MCP.fSP + 20.0f) > 180.0f)
                        {
                            g_MCP.fSP = 180.0f;
                        }
                        else
                        {
                            g_MCP.fSP = g_MCP.fSP + 20.0f;
                        }
                    }

                }
                g_timer = 0;
                g_MotorState = 0;

            }
          PCF8574A_Read( PCF8574A_SA, &uiData);
            if (CONTACT_Sample(&g_SW6, uiData & (0x04)))
            {
                if (!g_SW6.bOutput)
                {
                    SwitchPressed_SW6(1);
                    if (Motor_Mode == 0)
                    {
                        g_MotorState = 1;
                    }
                }
                else
                {
                    SwitchPressed_SW6(0);
                    if (g_timer < 1000 && g_timer > 0)
                    {
                        if (g_MCP.fSP - 1.0f < 0)
                        {
                            g_MCP.fSP = 0;
                        }
                        else
                        {
                            g_MCP.fSP = g_MCP.fSP - 1.0f;
                        }
                    }
                    if (g_timer >= 1000)
                    {
                        if ((g_MCP.fSP - 20.0f) < 0)
                        {
                            g_MCP.fSP = 0.0f;
                        }
                        else
                        {
                            g_MCP.fSP = g_MCP.fSP - 20.0f;
                        }
                    }

                }
                g_timer = 0;
                g_MotorState = 0;

            }

#ifdef USE_RTC
                if (!--g_uiRTCCounter)
                {
                    g_uiRTCCounter = 500;

                    // Read the time from the MCP7940M
                    MCP7940M_Read(0, g_aRTCData, 8);
                    sprintf(g_sBuffer, "Time: %02X:%02X:%02X  ",
                            g_aRTCData[2] & 0x1f, g_aRTCData[1] & 0x7f,
                            g_aRTCData[0] & 0x7f);
                    sprintf(g_sBuffer, "Date: 20%02X-%02X-%02X", g_aRTCData[6],
                            g_aRTCData[5] & 0x1f, g_aRTCData[4] & 0x3f);
                }
#endif

            }

            if (CONTACT_Sample(
                    &g_SW2, HWREG(GPIO_PORTF_BASE + GPIO_O_DATA + (0x10 << 2))))
            {
                // UART_SendMessage("switch 3");
                ITC = Upcounter();  // upcount by 1
                uiScreen = ITC;
                LCD_Display(ITC);

            }

            if (CONTACT_Sample(
                    &g_SW3,
                    HWREG(GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x01 << 2))))
            {
                // UART_SendMessage("switch 2");
                ITC = Downcounter(); // downcount by 1
                uiScreen = ITC;
                LCD_Display(ITC);
            }
            if (GLOBAL_CheckSysFlag( SYSFLAGS_ADC_SS0))
            {
                // Clear system ADC SS0 event
                GLOBAL_ClearSysFlag( SYSFLAGS_ADC_SS0);

                // Read ADC data

                if (ADC_SS0_Read(
                        aValues,
                        NUM_ELEMENTS(aValues)) == NUM_ELEMENTS( aValues ))
                {
                    // Update the dynamic (changing) portions of the screen
                    // Ensure VREFP is accurate - measure!

                    float fVREFP = 3.307f;
                    float fVREFN = 0.0f;

                    fAIN4 = aValues[0];
                    float fAIN4_Conv = (fAIN4 / 4096.0f) * (fVREFP - fVREFN);
                    UART_SS0Read[0] = fAIN4_Conv;

                    float fAIN5 = aValues[1];
                    float fAIN5_Celsius = fAIN5 / 81.92; // (Converted to celsius)
                    UART_SS0Read[1] = fAIN5_Celsius;

                    fAIN5 = aValues[1];
                    float fAIN5_Farenheit = fAIN5 / 81.771;
                    fAIN5_Farenheit = (fAIN5_Farenheit * 1.81) + 32.0; // (Converted to fAIN5_Farenheit)

                    float fIntTemp = aValues[2];
                    fIntTemp =
                         147.5f - (75.0f * (fVREFP - fVREFN)
                                            * (fIntTemp / 4096.0f)); //(Internal Temperature in Kelvin)

                    float m_Value = (UART_SS0Read[1] / 10.0f)*(256.0f/4.98f);

                    Contrast_adjustment(m_Value,1);

                    //fIntTemp = fIntTemp + 273.15;
                    UART_SS0Read[3] = fIntTemp + 3.0f;

                    if (Motor_Mode == 1)
                    {
                        Automatic_mode(fAIN4_Conv);
                    }

                    // Format AIN4 reading
                    // float fAIN4 = aValues[0];
                    if (uiScreen == CSCRN_TIME)  //Internal Temperature Screen
                    {
                        Screentime(g_aRTCData);

                    }
                    if (uiScreen == CSCRN_RPM)  //Internal Temperature Screen
                    {
                      float speed = QEI_GetSpeed();
                      // MotorSpeed( g_MCP.fSP);
                       MotorSpeed( speed);

                    }

                    while (ADC_SS0_Read(aValues, 1));
                }
            }

            //-----------------------------------------------------------------

            if (GLOBAL_CheckSysFlag( SYSFLAGS_UART_RXD))
            {

                // Clear the UART received data flag
                GLOBAL_ClearSysFlag( SYSFLAGS_UART_RXD);

                // Extract data from the queue and transmit
                uint8_t uiData;
                //  uint16_t uiTimer_UART = 0;
                while (UART_GetChar(&uiData))
                {
                    LED_LED2(1);
                    g_timer2 =500;
                    UART_ReadChar(uiData, UART_SS0Read);
                //    EL507_ToUpper((char*) &uiData);


                }


            }


        }
    }

//----------------------------------------------------------------------------
// END MAIN.C
//----------------------------------------------------------------------------
