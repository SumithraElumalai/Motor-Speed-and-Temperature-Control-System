/*
 * Up-Time.h
 *
 *  Created on: Oct 21, 2024
 *      Author: selumala
 */

#ifndef UP_TIME_H_
#define UP_TIME_H_

#include "global.h"
void Up_Time(void);

typedef struct{
    uint16_t m_seconds;
    uint8_t  seconds;
    uint8_t  minutes;
}Timer;

#endif /* UP_TIME_H_ */
