/*
 * LM71_SPI_temp_sensor.h
 *
 *  Created on: 2024. aug. 19.
 *  Author: Póti Szabolcs
 *
 *__________________________________

  Max clock freq is 6.25MHz
  Note that 300 ms will have to pass for a conversion to complete before the LM71 actually transmits temperature data.

 	 	    __________
 	 	   |          |
 	 /CS ==| O        |== VCC
 	 	   |          |
 	 GND ==|   LM71   |
 	 	   |          |
 	SDIO ==|          |== SCK
 	 	   |__________|

 */


#ifndef LM71_SPI_temp_sensor_
#define LM71_SPI_temp_sensor_


#define LM71_LSB_VAL 	0.03125f
#define LM71_MIN_TEMP	(-40.0f)
#define LM71_MAX_TEMP	150.0f

#define LM71_DEVICE_ID	0x800f

float convertLM71RawVal2Temp(int16_t rawtemp);
int16_t LM71_read(uint16_t CS_pin, GPIO_TypeDef* CS_port);
int16_t LM71_init(uint16_t CS_pin, GPIO_TypeDef* CS_port);
void LM71_sleep(uint16_t CS_pin, GPIO_TypeDef* CS_port);

#endif //LM71_SPI_temp_sensor_
