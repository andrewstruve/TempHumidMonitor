/*
 * utility.c
 *
 *  Created on: Feb 13, 2021
 *      Author: astruve
 */

#include "stm32l0xx_hal.h"
#include "utility.h"

extern TIM_HandleTypeDef htim2;
void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim2) < us);  // wait for the counter to reach the us input in the parameter
}
void SetGreenLed(GPIO_PinState state)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, state);
}
