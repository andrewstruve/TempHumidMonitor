/*
 * dht22.c
 *
 *  Created on: Feb 13, 2021
 *      Author: astruve
 */
#include "temp_humid_sensor.h"
#include "stm32l0xx_hal.h"

#define DHT_PORT GPIOA
#define DHT_PIN GPIO_PIN_3

float TemperatureCelcius;
float TemperatureFarenheit;
float HumidityPercentage;


void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

float GetTemperatureFarenheit()
{
	return TemperatureFarenheit;

}
float GetHumidity()
{
	return HumidityPercentage;
}

void DHT_StartConversion()
{
	// set the pin as output
	Set_Pin_Output(DHT_PORT, DHT_PIN);
	// pull the pin low for 2 milliseconds
	HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, 0);
	HAL_Delay(2);
	 // pull the pin high
    HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, 1);
    // wait 20 useconds
    delay_us(20);
    // set the pin as input
	Set_Pin_Input(DHT_PORT, DHT_PIN);
}
#define RESPONSE_SUCCESS 1
#define RESPONSE_FAIL 0
uint8_t DHT_Check_Response(void)
{
	uint8_t response = RESPONSE_FAIL;
	delay_us(40);
	if (!(HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)))
	{
		delay_us(80);
		if ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)))
		{
			response = RESPONSE_SUCCESS;
		}
		else {
			response = RESPONSE_FAIL;
		}
	}
	// TODO: what if this pin never goes low?
	while ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));   // wait for the pin to go low

	return response;
}
uint8_t DHT_Read_Byte (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		// TODO: what if this pin never goes high?
		while (!(HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));   // wait for the pin to go high
		delay_us(40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)))   // if the pin is low
		{
			// write 0
			i &= ~(1<<(7-j));
		}
		else
		{
			// if the pin is high, write 1
			i |= (1<<(7-j));
		}
		// TODO: what if this pin never goes low?
		while ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));  // wait for the pin to go low
	}
	return i;
}

void DHT_Collect_Sensor_Data()
{
	uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
	uint16_t checksum;
	uint16_t calculated_checksum;
	uint8_t response = 0;
    DHT_StartConversion();
	response = DHT_Check_Response();
	if(response == RESPONSE_SUCCESS)
	{
		Rh_byte1 = DHT_Read_Byte();
		Rh_byte2 = DHT_Read_Byte();
		Temp_byte1 = DHT_Read_Byte();
		Temp_byte2 = DHT_Read_Byte();
		checksum = DHT_Read_Byte();
		//
		calculated_checksum = (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2) & 0xFF;
		if (checksum == calculated_checksum)
		{
			TemperatureCelcius = ((Temp_byte1<<8)|Temp_byte2) / 10.0;
			// if the MSB is 1 of the high byte, make the number negative.
			if((Temp_byte1 & 0x8) == 1)
			{
				TemperatureCelcius *= -1;
			}
			TemperatureFarenheit = (TemperatureCelcius * (9.0/5.0)) + 32;
			HumidityPercentage = ((Rh_byte1<<8)|Rh_byte2) / 10.0;
		}
	}
	else
	{
		// was not able to communicate with DHT22 sensor
	}

}
