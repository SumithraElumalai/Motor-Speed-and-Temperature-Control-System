/*
 * SW_Reset.c
 *
 *  Created on: Oct 6, 2024
 *      Author: selumala
 */
#include "global.h"
#include "Init.h"
#include "Contrast.h"
#include "motor.h"

void Init(void)
{


            HWREG(SYSCTL_RCGCGPIO) |=(1<<5);// Initialization for RCGCGPIO

            HWREG(SYSCTL_RCGCWD)|=0x01; // Initialization for Watchdog

            HWREG( GPIO_O_PBORCTL ) |= 0x06; // Initialization for Brown-out

           HWREG(SYSCTL_MOSCCTL) |= 0x01;   //Main osc initialization

            HWREG(GPIO_PORTF_BASE + GPIO_O_GPIOLOCk) = 0x4C4F434B; //PF4 Unclock
            HWREG(GPIO_PORTF_BASE + GPIO_O_GPIOCR)|=(1<<0);  // PF4 OCIR reset


            HWREG( GPIO_PORTF_BASE + GPIO_O_DEN   ) |= (1<<4) | (1<<0);  // PortF Enable
            HWREG( GPIO_PORTF_BASE + GPIO_O_DIR   ) &= ~(1<<4) | (1<<0); // PortF Direction
            HWREG( GPIO_PORTF_BASE + GPIO_O_PUR  ) |= (1<<4) | (1<<0);   // Pull-up Resistor

}

