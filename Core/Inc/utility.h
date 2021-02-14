/*
 * utility.h
 *
 *  Created on: Feb 13, 2021
 *      Author: astruve
 */

#ifndef INC_UTILITY_H_
#define INC_UTILITY_H_
#include "stm32l0xx_hal.h"
void delay_us (uint16_t us);
void SetGreenLed(GPIO_PinState state);


#endif /* INC_UTILITY_H_ */
