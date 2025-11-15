//
// 2024.01.13
// Author: Póti Szabolcs
// 128x64 OLED Display with SSD1315 controller
//

#ifndef SSD128x64_Oled_SSD1315_C_
#define SSD128x64_Oled_SSD1315_C_

#include <math.h>
#include <stdint.h>
#include "SSD1315_128x64_Oled.h"

#ifdef _AVR_IO_H_
#include "TWI.h"
#else
#include "main.h"
#define ssd1315_stm32_HAL
//#define ssd1315_stm32_LL_implementation_1
//#define ssd1315_stm32_LL_implementation_2
#define I2C_WRITE	0x00
#define I2C_READ	0x01
#endif

extern volatile uint8_t disp_mat[pixels_y][pixels_x/8];

#ifdef _AVR_IO_H_

void print_disp_mat(void)
{
	uint8_t i=0, k=0;
	for(k=0; k<8; k++)
	{
		go_to_col_page(0, 7-k);
		i2c_start(Disp_Addr);
		i2c_write(Next_Will_Be_Data);
		for(i=0; i<pixels_y; i++)
		{
			i2c_write(disp_mat[i][k]);

		}
		i2c_stop();
	}
}

void delete_RAM(void)
{
	uint8_t i=0, k=0;
	for(k=0; k<8; k++)
	{
		go_to_col_page(0, k);
		i2c_start(Disp_Addr);
		i2c_write(Next_Will_Be_Data);
		for(i=0; i<pixels_y; i++)
		{
			i2c_write(0x00);

		}
		i2c_stop();
	}
}

void go_to_col_page(uint8_t x, uint8_t y)
{
	if((x >= 0) && (x <= 127))
	{
		oled_send_cmd(CMD_Set_Column_Address);
		oled_send_cmd(x);
		oled_send_cmd(127);
	}	else{}
	if((y >= 0) && (y <= 7))
	{
		oled_send_cmd(CMD_Set_Page_Address);
		oled_send_cmd(y);
		oled_send_cmd(7);
	}	else{}
}

void oled_init(void)
{
	oled_send_cmd(CMD_Display_off_sleep_mode);
	oled_send_cmd(CMD_Set_Display_Clock_Divide_Ratio__Oscillator_Frequency);
	oled_send_cmd(0xf0);
	oled_send_cmd(CMD_Set_Mux_Ratio);
	oled_send_cmd(63);//Screen_Height - 1
	oled_send_cmd(CMD_Set_Display_Offset);
	oled_send_cmd(0x00);
	oled_send_cmd(CMD_Set_Charge_Pump);
	oled_send_cmd(0x94);
	oled_send_cmd(CMD_Set_Display_Start_Line+0);
	oled_send_cmd(CMD_Set_Memory_Addressing_Mode);
	oled_send_cmd(0x00);
	oled_send_cmd(CMD_Set_Segment_Remap+1);
	oled_send_cmd(CMD_Set_COM_Output_Scan_Direction_Remapped);
	oled_send_cmd(CMD_Set_COM_Pins_Hardware_Config);
	oled_send_cmd(0x02);
	oled_send_cmd(CMD_Select_Internal_External_IREF);
	oled_send_cmd(0x20);
	oled_send_cmd(CMD_Set_Contrast_Control);
	oled_send_cmd(0x02);
	oled_send_cmd(CMD_Set_Pre_charge_Period);
	oled_send_cmd(0xF1);
	oled_send_cmd(CMD_Set_VCOMH_Deselect_Level);
	oled_send_cmd(0x20);
	delete_RAM();
	oled_send_cmd(CMD_Reset_Entire_Display_ON);
	oled_send_cmd(CMD_Set_Normal_Display);
	oled_send_cmd(0x2E);
	oled_send_cmd(CMD_Display_on_normal_mode);
}

void oled_send_cmd(uint8_t cmd)
{
	i2c_start(Disp_Addr);
	i2c_write(Next_Will_Be_Cmd);
	i2c_write(cmd);
	i2c_stop();
}

void oled_send_data(uint8_t data)
{
	i2c_start_wait(Disp_Addr);
	i2c_write(Next_Will_Be_Data);
	i2c_write(data);
	i2c_stop();
}

#endif //_AVR_IO_H_


#ifdef ssd1315_stm32_HAL

extern I2C_HandleTypeDef hi2c2;

void print_disp_mat(void)
{
	volatile uint8_t i=0, k=0;
	uint8_t* tmp = calloc(pixels_y+1, sizeof(uint8_t));
	if(tmp == NULL)	{ Error_Handler();}//calloc fail
	tmp[0]=Next_Will_Be_Data;

	for(k=0; k<8; k++)
	{
		go_to_col_page(0, k);
		for(i=0; i<pixels_y; i++)
		{
			tmp[i+1]=disp_mat[i][k];
		}
		HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)Disp_Addr, tmp, pixels_y+1, 1000);
	}

	free(tmp);
	tmp = NULL;
}

void delete_RAM(void)
{
	volatile uint8_t k=0;
	uint8_t* tmp = calloc(pixels_y+1, sizeof(uint8_t));
	if(tmp == NULL)	{ Error_Handler();}//calloc fail
	tmp[0]=Next_Will_Be_Data;

	for(k=0; k<8; k++)
	{
		go_to_col_page(0, k);
		HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)Disp_Addr, tmp, pixels_y+1, 1000);
	}

	free(tmp);
	tmp = NULL;
}

void oled_send_cmd(uint8_t cmd)
{

	uint8_t tmp[2] = {0};
	tmp[0]=Next_Will_Be_Cmd;
	tmp[1]=cmd;
	HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)Disp_Addr, tmp, 2, 1000);
}

void oled_send_data(uint8_t data)
{
	uint8_t tmp[2] = {0};
	tmp[0]=Next_Will_Be_Data;
	tmp[1]=data;
	HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)Disp_Addr, tmp, 2, 1000);
}
#endif //ssd1315_stm32_HAL



#if defined (USE_HAL_DRIVER)	//STM32 mikrokontrollerhez


void go_to_col_page(uint8_t x, uint8_t y)
{
	if(x <= 127)
	{
		oled_send_cmd(CMD_Set_Column_Address);
		oled_send_cmd(x);
		oled_send_cmd(127);
	}	else{}
	if(y <= 7)
	{
		oled_send_cmd(CMD_Set_Page_Address);
		oled_send_cmd(y);
		oled_send_cmd(7);
	}	else{}
}

void oled_init(void)
{
	oled_send_cmd(CMD_Display_off_sleep_mode);

	oled_send_cmd(CMD_Set_Lower_Column_Start_Address_for_Page_Addressing_Mode+0);
	oled_send_cmd(CMD_Set_Higher_Column_Start_Address_for_Page_Addressing_Mode+0);
	oled_send_cmd(CMD_Set_Page_Start_Address_for_Page_Addressing_Mode);

	oled_send_cmd(CMD_Set_Display_Clock_Divide_Ratio__Oscillator_Frequency);
	oled_send_cmd(0xf0);
	oled_send_cmd(CMD_Set_Mux_Ratio);
	oled_send_cmd(63);//Screen_Height - 1
	oled_send_cmd(CMD_NOP);
	oled_send_cmd(CMD_Set_Display_Offset);
	oled_send_cmd(0x00);
	oled_send_cmd(CMD_Set_Charge_Pump);
	oled_send_cmd(0x94);
	oled_send_cmd(CMD_Set_Display_Start_Line+0);
	oled_send_cmd(CMD_Set_Memory_Addressing_Mode);
	oled_send_cmd(0x02);
	oled_send_cmd(CMD_Set_Segment_Remap+1);//horizontal mirror
	oled_send_cmd(CMD_Set_COM_Output_Scan_Direction_Remapped);//vertical mirror
	oled_send_cmd(CMD_Set_COM_Pins_Hardware_Config);
	oled_send_cmd(0x12);
	oled_send_cmd(CMD_Select_Internal_External_IREF);
	oled_send_cmd(0x00);
	oled_send_cmd(CMD_Set_Contrast_Control);
	oled_send_cmd(0xff);
	oled_send_cmd(CMD_Set_Pre_charge_Period);
	oled_send_cmd(0x22);
	oled_send_cmd(CMD_Set_VCOMH_Deselect_Level);
	oled_send_cmd(0x20);

	delete_disp_mat();
	oled_send_cmd(CMD_Reset_Entire_Display_ON);
	oled_send_cmd(CMD_Set_Normal_Display);
	oled_send_cmd(0x2E);//stop scroll
	delete_RAM();
	oled_send_cmd(CMD_Display_on_normal_mode);


}
#endif // (USE_HAL_DRIVER)	//STM32 mikrokontrollerhez

#if defined (USE_FULL_LL_DRIVER)	//STM32 mikrokontrollerhez  LL driverrel

#ifdef ssd1315_stm32_LL_implementation_1

void print_disp_mat(void)
{
	uint8_t i=0, k=0;
	for(k=0; k<8; k++)
	{
		go_to_col_page(0, k);
		LL_I2C_HandleTransfer(I2C1, Disp_Addr, LL_I2C_ADDRESSING_MODE_7BIT, 129, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
		LL_I2C_TransmitData8(I2C1, Next_Will_Be_Data);
		while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
		{
			asm("nop");
		}
		for(i=0; i<pixels_y; i++)
		{
			LL_I2C_TransmitData8(I2C1, disp_mat[i][k]);
			while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
			{
				asm("nop");
			}

		}
	}
}

void delete_RAM(void)
{
	uint8_t i=0, k=0;
	for(k=0; k<8; k++)
	{
		go_to_col_page(0, k);
		LL_I2C_HandleTransfer(I2C1, Disp_Addr, LL_I2C_ADDRESSING_MODE_7BIT, 129, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
		LL_I2C_TransmitData8(I2C1, Next_Will_Be_Data);
		while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
		{
			asm("nop");
		}
		for(i=0; i<pixels_y; i++)
		{
			LL_I2C_TransmitData8(I2C1, 0x00);
			while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
			{
				asm("nop");
			}
		}
	}
}

void oled_send_cmd(uint8_t cmd)
{
	LL_I2C_HandleTransfer(I2C1, Disp_Addr, LL_I2C_ADDRESSING_MODE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
	LL_I2C_TransmitData8(I2C1, Next_Will_Be_Cmd);
	while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
	{
		asm("nop");
	}
	LL_I2C_TransmitData8(I2C1, cmd);
	while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
	{
		asm("nop");
	}
}

void oled_send_data(uint8_t data)
{
	LL_I2C_HandleTransfer(I2C1, Disp_Addr, LL_I2C_ADDRESSING_MODE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
	LL_I2C_TransmitData8(I2C1, Next_Will_Be_Data);
	while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
	{
		asm("nop");
	}
	LL_I2C_TransmitData8(I2C1, data);
	while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
	{
		asm("nop");
	}
}
#endif //ssd1315_stm32_LL_implementation_1


#ifdef ssd1315_stm32_LL_implementation_2

void print_disp_mat(void)
{
	uint8_t k=0, l=0;
	__IO uint32_t tmpreg;

	for(k=0; k<8; k++)
	{
		go_to_col_page(0, k);

		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
		LL_I2C_GenerateStartCondition(I2C1);
		while(!LL_I2C_IsActiveFlag_SB(I2C1)) { __NOP();}
		tmpreg = I2C1->CR1;
		(void) tmpreg;
		LL_I2C_TransmitData8(I2C1, (uint8_t)Disp_Addr | I2C_WRITE);
		while(!LL_I2C_IsActiveFlag_ADDR(I2C1))	{ __NOP();}
		LL_I2C_ClearFlag_ADDR(I2C1);
		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

		while(! LL_I2C_IsActiveFlag_TXE(I2C1))	{ __NOP();}
		LL_I2C_TransmitData8(I2C1, Next_Will_Be_Data);

		for(l=0; l<pixels_y; l++)
		{
			if(l==(pixels_y-1))
			{
				LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
			}
			else
			{
				LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
			}

			while(! LL_I2C_IsActiveFlag_BTF(I2C1))	{ __NOP();}
			while(! LL_I2C_IsActiveFlag_TXE(I2C1))	{ __NOP();}

			LL_I2C_TransmitData8(I2C1, disp_mat[l][k]);
		}

		while(! LL_I2C_IsActiveFlag_BTF(I2C1))	{ __NOP();}

		LL_I2C_GenerateStopCondition(I2C1);
	}
}

void delete_RAM(void)
{
	uint8_t k=0, l=0;
	__IO uint32_t tmpreg;

	for(k=0; k<8; k++)
	{
		go_to_col_page(0, k);

		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
		LL_I2C_GenerateStartCondition(I2C1);
		while(!LL_I2C_IsActiveFlag_SB(I2C1)) { __NOP();}
		tmpreg = I2C1->CR1;
		(void) tmpreg;
		LL_I2C_TransmitData8(I2C1, (uint8_t)Disp_Addr | I2C_WRITE);
		while(!LL_I2C_IsActiveFlag_ADDR(I2C1))	{ __NOP();}
		LL_I2C_ClearFlag_ADDR(I2C1);
		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

		while(! LL_I2C_IsActiveFlag_TXE(I2C1))	{ __NOP();}
		LL_I2C_TransmitData8(I2C1, Next_Will_Be_Data);

		for(l=0; l<pixels_y; l++)
		{
			if(l==(pixels_y-1))
			{
				LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
			}
			else
			{
				LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
			}

			while(! LL_I2C_IsActiveFlag_BTF(I2C1))	{ __NOP();}
			while(! LL_I2C_IsActiveFlag_TXE(I2C1))	{ __NOP();}

			LL_I2C_TransmitData8(I2C1, 0x00);
		}

		while(! LL_I2C_IsActiveFlag_BTF(I2C1))	{ __NOP();}

		LL_I2C_GenerateStopCondition(I2C1);
	}
}

void oled_send_cmd(uint8_t cmd)
{
	__IO uint32_t tmpreg;

	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
	LL_I2C_GenerateStartCondition(I2C1);

	while(!LL_I2C_IsActiveFlag_SB(I2C1))	{ __NOP();}
	tmpreg = I2C1->CR1;
	(void) tmpreg;
	LL_I2C_TransmitData8(I2C1, (uint8_t)Disp_Addr | I2C_WRITE);
	while(!LL_I2C_IsActiveFlag_ADDR(I2C1))	{ __NOP();}
	LL_I2C_ClearFlag_ADDR(I2C1);

	while(! LL_I2C_IsActiveFlag_TXE(I2C1))	{ __NOP();}

	LL_I2C_TransmitData8(I2C1, Next_Will_Be_Cmd);

	while(! LL_I2C_IsActiveFlag_BTF(I2C1))	{ __NOP();}
	while(! LL_I2C_IsActiveFlag_TXE(I2C1))	{ __NOP();}

	LL_I2C_TransmitData8(I2C1, cmd);

	while(! LL_I2C_IsActiveFlag_BTF(I2C1))	{ __NOP();}

	LL_I2C_GenerateStopCondition(I2C1);
}

void oled_send_data(uint8_t data)
{
	__IO uint32_t tmpreg;

	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
	LL_I2C_GenerateStartCondition(I2C1);

	while(!LL_I2C_IsActiveFlag_SB(I2C1))	{ __NOP();}
	tmpreg = I2C1->CR1;
	(void) tmpreg;
	LL_I2C_TransmitData8(I2C1, (uint8_t)Disp_Addr | I2C_WRITE);
	while(!LL_I2C_IsActiveFlag_ADDR(I2C1))	{ __NOP();}
	LL_I2C_ClearFlag_ADDR(I2C1);

	while(! LL_I2C_IsActiveFlag_TXE(I2C1))	{ __NOP();}

	LL_I2C_TransmitData8(I2C1, Next_Will_Be_Data);

	while(! LL_I2C_IsActiveFlag_BTF(I2C1))	{ __NOP();}
	while(! LL_I2C_IsActiveFlag_TXE(I2C1))	{ __NOP();}

	LL_I2C_TransmitData8(I2C1, data);

	while(! LL_I2C_IsActiveFlag_BTF(I2C1))	{ __NOP();}

	LL_I2C_GenerateStopCondition(I2C1);
}
#endif //ssd1315_stm32_LL_implementation_2

#endif//(USE_FULL_LL_DRIVER)	//STM32 mikrokontrollerhez LL driverrel



#endif //SSD128x64_Oled_SSD1315_C_
