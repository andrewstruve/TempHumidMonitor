/*
 * dht22.h
 *
 *  Created on: Feb 13, 2021
 *      Author: astruve
 */

#ifndef INC_TEMP_HUMID_SENSOR_H_
#define INC_TEMP_HUMID_SENSOR_H_

#include "stm32l0xx_hal.h"

void DHT_Collect_Sensor_Data();
float GetTemperatureFarenheit();
float GetHumidity();


#endif /* INC_TEMP_HUMID_SENSOR_H_ */
