/*
 * LCD_Reset_Cause.h
 *
 *  Created on: Nov 8, 2024
 *      Author: selumala
 */
#include <stdint.h>
#include <stdbool.h>
#include"stdio.h"
#include "global.h"

#ifndef LCD_RESET_CAUSE_H_
#define LCD_RESET_CAUSE_H_

void LCD_Reset_Cause(uint8_t);
void ADCVoltage_Display(float,float );
void ADCPD2Temp_Display(float , float);
void ADCPD3Temp_Display(float,float);
void ADCIntTemp_Display(uint16_t *, float );
void Screentime(uint8_t *g_aRTCData);
void MotorSpeed(float speed);



#endif /* LCD_RESET_CAUSE_H_ */
