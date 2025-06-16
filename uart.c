//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : UART.C
// FILE VERSION : 1.0
// PROGRAMMER   : selumala
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
// Provides code to support UART0 for a serial communication interface.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "uart.h"
#include "queue.h"
#include "term.h"
#include <stdio.h>
#include "motor.h"
#include "qei.h"
#include "led.h"


//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

QUEUE *g_pQueueTransmit;
QUEUE *g_pQueueReceive;
char g_sUARTBuffer[80];
uint8_t g_aRTCData[8];
extern MOTOR_CONTROL_PARAMS g_MCP;


enum
{
    CMSG_ATEMP = 1, CMSG_CCST, CMSG_EL507, CMSG_HEADER, CMSG_ITEMP, CMSG_VOLTAGE

};
//----------------------------------------------------------------------------
// FUNCTION : UART0_IntHandler( void )
// PURPOSE  : Interrupt handler
//----------------------------------------------------------------------------

void UART0_IntHandler(void)
{
    uint32_t uiIntStatus = HWREG(UART0_BASE + UART_O_MIS);

    // Check for transmit (FIFO empty) interrupt
    if (uiIntStatus & (1 << 5))
    {
        // Clear interrupt
        HWREG( UART0_BASE + UART_O_ICR ) = (1 << 5);

        // Continue transmitting anything left in the queue
        UART_TransmitFromQueue();
    }

    // Check for receive interrupt
    if (uiIntStatus & (1 << 4))
    {
        // Clear interrupt
        HWREG( UART0_BASE + UART_O_ICR ) = (1 << 4);

        // Transfer received data to queue
        UART_ReceiveToQueue();

        // Set a global flag to indicate that data was received
        GLOBAL_SetSysFlag( SYSFLAGS_UART_RXD);
    }
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_ReceiveToQueue( void )
// PURPOSE  : Receives information from UART0 and enqueues it to the queue
//----------------------------------------------------------------------------

void UART_ReceiveToQueue(void)
{
    uint8_t uiData;

    // Read data and queue
    uiData = HWREG(UART0_BASE + UART_O_DR);
    QUEUE_Enqueue(g_pQueueReceive, uiData);

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_TransmitFromQueue( void )
// PURPOSE  : Dequeues information from the a queue and sends it via UART0
//----------------------------------------------------------------------------

void UART_TransmitFromQueue(void)
{
    uint8_t uiData;

    // If the transmitter holding register is empty, start a transmission
    if ( HWREG( UART0_BASE + UART_O_FR ) & (1 << 7))
    {
        // Remove byte from queue and transmit
        if (QUEUE_Dequeue(g_pQueueTransmit, &uiData))
        {
            HWREG( UART0_BASE + UART_O_DR ) = uiData;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_GetChar( uint8_t *puiData )
// PURPOSE  : Gets one character from the receive queue
//----------------------------------------------------------------------------

bool UART_GetChar(uint8_t *puiData)
{
    bool bResult = false;

    UART0_IntDisable();
    {
        bResult = QUEUE_Dequeue(g_pQueueReceive, puiData);
    }
    UART0_IntEnable();

    return bResult;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_PutChar( uint8_t uiData )
// PURPOSE  : Places one character into the transmit queue
//----------------------------------------------------------------------------

bool UART_PutChar(uint8_t uiData)
{
    bool bResult = false;

    UART0_IntDisable();
    {
        if (QUEUE_Enqueue(g_pQueueTransmit, uiData))
        {
            UART_TransmitFromQueue();
            bResult = true;
        }
    }
    UART0_IntEnable();

    return bResult;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_SendMessage( char* sMessage )
// PURPOSE  : Writes a message to the transmit queue
//----------------------------------------------------------------------------

void UART_SendMessage(char *sMessage)
{
    // Enqueue entire message using the UART transmit queue
    while (*sMessage)
    {
        if (UART_PutChar(*(uint8_t*) sMessage))
        {
            sMessage++;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_SendEncodedMessage( char* sMessage, uint8_t uiKey )
// PURPOSE  : Writes an encoded message the transmit queue
//----------------------------------------------------------------------------

void UART_SendEncodedMessage(char *sMessage, uint8_t uiKey)
{
    while (*sMessage)
    {
        if (UART_PutChar(*(uint8_t*) sMessage ^ uiKey))
        {
            sMessage++;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_Init( void )
// PURPOSE  : Initializes the UART peripheral
//----------------------------------------------------------------------------

void UART_Init(void)
{
    // UART 0 Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCUART ) |= 0x01;

    // UART 0 Sleep Mode Clock Gating Control
    HWREG( SYSCTL_SCGCUART ) |= 0x01;

    // GPIO Port A Run Mode Clock Gating Control (Port A)
    HWREG( SYSCTL_RCGCGPIO ) |= 0x01;

    // GPIO Port A Digital Enable (UART U0Rx/U0Tx = PA0/PA1)
    HWREG( GPIO_PORTA_BASE + GPIO_O_DEN ) |= 0x03;

    // GPIO Port A Alternate Function Select (UART U0Rx/U0Tx = PA0/PA1)
    HWREG( GPIO_PORTA_BASE + GPIO_O_AFSEL ) |= 0x03;

    // GPIO Port A Port Control (assign as U0Rx and U0Tx)
    HWREG( GPIO_PORTA_BASE + GPIO_O_PCTL ) = (1 << 0) | (1 << 4);

    // Initialize control (default)
    HWREG( UART0_BASE + UART_O_CTL ) = 0;

    // Compute the fractional baud rate divisor
    uint32_t uiDivisor = ((( UART_CLOCK * 8) / UART_BAUDRATE) + 1) / 2;

    // Set the baud rate
    HWREG( UART0_BASE + UART_O_IBRD ) = uiDivisor / 64;
    HWREG( UART0_BASE + UART_O_FBRD ) = uiDivisor % 64;

    // Set parity, data length, and number of stop bits
    HWREG( UART0_BASE + UART_O_LCRH ) = (0x03 << 5);

    // Enable the UART in two operations
    // Bit 9: RXE
    // Bit 8: TXE
    // Bit 0: UARTEN
    HWREG( UART0_BASE + UART_O_CTL ) = ((1 << 9) | (1 << 8) | (1 << 0));

    // UART Interrupt Mask
    // Bit 5: TXIM
    // Bit 4: RXIM
    HWREG( UART0_BASE + UART_O_IM ) = ((1 << 5) | (1 << 4));

    // Enable Interrupts for UART0
    UART0_IntEnable();

    // Create a transmit queue
    g_pQueueTransmit = QUEUE_Create(1024);

    // Create a receive queue
    g_pQueueReceive = QUEUE_Create(32);

    return;
}

//----------------------------------------------------------------------------
// END UART.C
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//UART Display Using the dynamic Values
//----------------------------------------------------------------------------

void  UART_ReadChar(uint8_t uiData, float *UART_SS0Read)
{


    switch ((char) uiData)
    {

    case 0x17: //Watchdog Timer (WDT0) Reset (Ctrl+<R>)
    {

        HWREG( WATCHDOG0_BASE + WDT_O_LOCK ) = 0x1ACCE551; // Unlock
        HWREG( WATCHDOG0_BASE + WDT_O_LOAD ) = 0; // Load count
        HWREG( WATCHDOG0_BASE + WDT_O_CTL ) = 0x03; // Enable
        HWREG( WATCHDOG0_BASE + WDT_O_LOCK ) = 0; // Lock
        break;
    }

    case 0x12:   // Software Reset (Ctrl+<R>)
    {
        HWREG(APINT) = (0X05FA << 16) | (1 << 2);
        break;
    }

    case 'T': // if received character is A
    {

        UART_SendMessage("\e[K");
        sprintf(g_sUARTBuffer, "%4.1f", UART_SS0Read[1]);
        UART_SendMessage("Ambient Temperature: ");
        UART_SendMessage(g_sUARTBuffer); // Ambient Temperature
        UART_SendMessage(" \e(0");
        UART_SendMessage("f"); // display DEGREE
        UART_SendMessage("C\r"); // To display C
        UART_SendMessage("\e(B");  // ASCII
        UART_SendMessage("\e[0m"); // Normal Attributes

      sprintf(g_sUARTBuffer, "%5.1f", UART_SS0Read[3]);
      UART_SendMessage("\n"); // Display Kelvin
        UART_SendMessage("Internal Temperature: ");
        UART_SendMessage(g_sUARTBuffer);
        UART_SendMessage(" \e(0");
        UART_SendMessage("f"); // display DEGREE
        UART_SendMessage("C\r\n"); // To display C
        UART_SendMessage("\e(B");  // ASCII
        UART_SendMessage("\e[0m"); // Normal Attributes
        break;
    }

    case 'C':
    {

        UART_SendMessage("\e[K");
        sprintf(g_sUARTBuffer, "20%02X-%02X-%02XT%02X:%02X:%02X",
                g_aRTCData[6], g_aRTCData[5] & 0x1f, g_aRTCData[4] & 0x3f,g_aRTCData[2] & 0x1f, g_aRTCData[1] & 0x7f,g_aRTCData[0] & 0x7f);
        UART_SendMessage(g_sUARTBuffer);      // Internal Temperature
        UART_SendMessage("\r\n"); // Display Kelvin
        UART_SendMessage("\e(B");  // ASCII
        UART_SendMessage("\e[0m"); // Normal Attributes

        break;
    }
    case 'I':
    {

        //UART_SendMessage("\e[K");

        UART_SendMessage("EL507 Embedded Control Systems II\r\n");
        UART_SendMessage("Embedded System V1.0\r\n\n");
        sprintf(g_sUARTBuffer, "Date: 20%02X-%02X-%02X", g_aRTCData[6],
                g_aRTCData[5] & 0x1f, g_aRTCData[4] & 0x3f);
        UART_SendMessage(g_sUARTBuffer);
        UART_SendMessage("\r\n");
        sprintf(g_sUARTBuffer, "Time: %02X:%02X:%02X ", g_aRTCData[2] & 0x1f,
                g_aRTCData[1] & 0x7f, g_aRTCData[0] & 0x7f);
          UART_SendMessage(g_sUARTBuffer);          // Internal Temperature

        //UART_SendMessage("\e(B");  // ASCII
        //UART_SendMessage("\e[0m"); // Normal Attributes
        UART_SendMessage("\r\n");
        break;
    }
    case 'A':
    {
        UART_SendMessage("\e[K");
        UART_SendMessage("Mode : Automatic Control\r\n"); // Display Kelvin
        Motor_Mode =1;

        UART_SendMessage("\e(B");  // ASCII
        UART_SendMessage("\e[0m"); // Normal Attributes

        break;
    }
    case 'M':
        {
            UART_SendMessage("\e[K");
            UART_SendMessage("Mode : Manual Control\r\n"); // Display Kelvin
            Motor_Mode =0;
            UART_SendMessage("\e(B");  // ASCII
            UART_SendMessage("\e[0m"); // Normal Attributes
            break;
        }
    case 'R':
    {
        UART_SendMessage("\e[K");
        UART_SendMessage("Speed :"); // Display Kelvin
        g_MCP.fSP =QEI_GetSpeed();
        sprintf(g_sUARTBuffer, "%5.1f", g_MCP.fSP);
        UART_SendMessage(g_sUARTBuffer);
        UART_SendMessage("RPM\r\n");
        UART_SendMessage("\e(B");  // ASCII
        UART_SendMessage("\e[0m"); // Normal Attributes

        break;
    }
    case '?':
    {

        UART_SendMessage("\e[K");
        UART_SendMessage("Available Commands\r\n");
        UART_SendMessage("\n");
        UART_SendMessage("C - Display the date and time\r\n");
        UART_SendMessage(
                "T - Display the ambient and internal temperature\r\n");
        UART_SendMessage(
                "R - Display the speed of the output shaft in RPM\r\n");
        UART_SendMessage("0-9,F - Sets the speed of the output shaft\r\n");
        UART_SendMessage("A - Change the mode of control to automatic\r\n");
        UART_SendMessage("M - Change the mode of control to manual\r\n");
        UART_SendMessage("I - Display system information\r\n");
        UART_SendMessage("L - Toggles the state of LED3\r\n");
        UART_SendMessage("\n");
        UART_SendMessage("<Ctrl>+R-Reset the embedded system\r\n");

        break;
    }
    case 'F':
           {
               UART_SendMessage("\e[K");
               UART_SendMessage("Output shaft speed set to 100% of maximum(180.0 RPM)\r\n");
               g_MCP.fSP = 180.0f;
               UART_SendMessage("\e(B");  // ASCII
               UART_SendMessage("\e[0m"); // Normal Attributes
               break;
           }
    case 'L':
        {

            bool LED3=LED3_state();
            if(LED3)
                {
                    UART_SendMessage("LED3:ON\r\n");
                }
                else
                {
                    UART_SendMessage("LED3:OFF\r\n");
                }
           break;

    }

    }
    if(Motor_Mode==0)
    {
   if (((char) uiData >= '0' && (char) uiData <= '9')&&(Motor_Mode ==0))
       {
           switch ((char) uiData)
           {
           case '0':
           {
               UART_SendMessage("Output shaft speed set to 0% of maximum(0.0 RPM)\r\n");
               g_MCP.fSP = 0.0f;
               break;
           }
           case '1':
           {
               UART_SendMessage("Output shaft speed set to 10% of maximum(18.0 RPM)\r\n");
               g_MCP.fSP = 18.0f;
               break;
           }
           case '2':
           {
               UART_SendMessage("Output shaft speed set to 20% of maximum(36.0 RPM)\r\n");
               g_MCP.fSP = 36.0f;
               break;
           }
           case '3':
           {
               UART_SendMessage("Output shaft speed set to 30% of maximum(54.0 RPM)\r\n");
               g_MCP.fSP = 54.0f;
               break;
           }
           case '4':
           {
               UART_SendMessage("Output shaft speed set to 40% of maximum(72.0 RPM)\r\n");
               g_MCP.fSP = 72.0f;
               break;
           }
           case '5':
           {
               UART_SendMessage("Output shaft speed set to 50% of maximum(90.0 RPM)\r\n");
               g_MCP.fSP = 90.0f;
               break;
           }
           case '6':
           {
               UART_SendMessage("Output shaft speed set to 60% of maximum(108.0 RPM)\r\n");
               g_MCP.fSP = 108.0f;
               break;
           }
           case '7':
           {
               UART_SendMessage("Output shaft speed set to 70% of maximum(126.0 RPM)\r\n");
               g_MCP.fSP = 126.0f;
               break;
           }
           case '8':
           {
               UART_SendMessage("Output shaft speed set to 80% of maximum(144.0 RPM)\r\n");
               g_MCP.fSP = 144.0f;
               break;
           }
           case '9':
           {
               UART_SendMessage("Output shaft speed set to 90% of maximum(180.0 RPM)\r\n");
               g_MCP.fSP = 162.0f;
               break;
           }

           default:
           break;

           }
       }
    }



}






