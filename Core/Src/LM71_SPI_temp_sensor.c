/*
 * LM71_SPI_temp_sensor.c
 *
 *  Created on: 2024. aug. 19.
 *  Author: Póti Szabolcs
 */
#include "main.h"
#include "LM71_SPI_temp_sensor.h"

extern SPI_HandleTypeDef hspi5;

float convertLM71RawVal2Temp(int16_t rawtemp)
{
	float temp = ((rawtemp >> 2) * LM71_LSB_VAL);

	if ( (temp<LM71_MIN_TEMP) && (temp>LM71_MAX_TEMP) )
	{
		temp = 0;
	}

	return temp;
}

int16_t LM71_read(uint16_t CS_pin, GPIO_TypeDef* CS_port)
{
	uint8_t tmp[2]={0};
	int16_t val=0;

	HAL_GPIO_WritePin(CS_port, CS_pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(&hspi5, tmp, 2, 1000);
	HAL_GPIO_WritePin(CS_port, CS_pin, GPIO_PIN_SET);

	val = ( ((int16_t)tmp[0]<<8) | tmp[1] );

	return val;
}

int16_t LM71_init(uint16_t CS_pin, GPIO_TypeDef* CS_port)
{
	uint8_t tmp[2]={0};
	int16_t deviceID=0;

	HAL_GPIO_WritePin(CS_port, CS_pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(&hspi5, tmp, 2, 1000);//Read 16 bits of temperature data
	tmp[0] = 0xff;
	tmp[1] = 0xff;
	HAL_SPI_Transmit(&hspi5, tmp, 2, 1000);//Write 16 bits of data commanding shutdown
	HAL_SPI_Receive(&hspi5, tmp, 2, 1000);//Read 16 bits of Manufacture's/Device ID data
	deviceID = ( ((int16_t)tmp[0]<<8) | tmp[1] );
	tmp[0] = 0;
	tmp[1] = 0;
	HAL_SPI_Transmit(&hspi5, tmp, 2, 1000);//Write 8 to 16 bits of data commanding Conversion Mode
	HAL_GPIO_WritePin(CS_port, CS_pin, GPIO_PIN_SET);

	return deviceID;
}

void LM71_sleep(uint16_t CS_pin, GPIO_TypeDef* CS_port)
{
	uint8_t tmp[2]={0xff, 0xff};

	HAL_GPIO_WritePin(CS_port, CS_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi5, tmp, 2, 1000);
	HAL_GPIO_WritePin(CS_port, CS_pin, GPIO_PIN_SET);
}
