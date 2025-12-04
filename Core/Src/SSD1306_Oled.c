//
// 2019.01.06  20:31
// Author: Póti Szabolcs
// OLED Display with SSD1306 controller
//

#include <math.h>
#include <stdint.h>
#include "SSD1306_Oled.h"

#ifdef _AVR_IO_H_
#include "TWI.h"
#else
#include "main.h"

#define I2C_WRITE	0x00
#define I2C_READ	0x01
#endif

extern volatile uint8_t disp_mat[pixels_x][pixels_y/8];


#ifdef SSD1306_STM32_HAL

extern I2C_HandleTypeDef hi2c2;

void print_disp_mat(void)
{
	volatile uint8_t x=0, y=0;
	uint8_t row[pixels_x+1];

	row[0]=Next_Will_Be_Data;

	for(y=0; y<(pixels_y/8); y++)
	{
		go_to_col_page(0, y);
		for(x=0; x<pixels_x; x++)
		{
			row[x+1]=disp_mat[x][y];
		}
		HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)Disp_Addr, row, pixels_x+1, 1000);
	}
}

void delete_RAM(void)
{
	volatile uint8_t page=0;
	uint8_t row[pixels_x+1];

	row[0]=Next_Will_Be_Data;

	for(page=0; page<((pixels_y/8)); page++)
	{
		go_to_col_page(0, page);
		HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)Disp_Addr, row, pixels_x+1, 1000);
	}
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
#endif //SSD1306_STM32_HAL



#if defined (USE_HAL_DRIVER)	//STM32 mikrokontrollerhez


#ifdef HW_SSD1306_64X48_WEMOS_OLED
void go_to_col_page(uint8_t x, uint8_t y)
{
	if(y < (pixels_y/8))
	{
		oled_send_cmd(CMD_Set_Page_Address);
		oled_send_cmd(y);
		oled_send_cmd((pixels_y/8)-1);
	}	else{}
	if(x < pixels_x)
	{
		oled_send_cmd(CMD_Set_Column_Address);
		oled_send_cmd(32+x);
		oled_send_cmd(32+pixels_x-1);
	}	else{}
}
#else
void go_to_col_page(uint8_t x, uint8_t y)
{
	if(y < (pixels_y/8))
	{
		oled_send_cmd(CMD_Set_Page_Address);
		oled_send_cmd(y);
		oled_send_cmd((pixels_y/8)-1);
	}	else{}
	if(x < pixels_x)
	{
		oled_send_cmd(CMD_Set_Column_Address);
		oled_send_cmd(x);
		oled_send_cmd(pixels_x-1);
	}	else{}
}
#endif

#ifdef HW_SSD1306_64X48_WEMOS_OLED
void oled_init(void)
{
	//disp init sequence
	oled_send_cmd(CMD_Set_Disp_Off);
	oled_send_cmd(CMD_Set_Display_Clock_Divide_Ratio_Oscillator_Freq);
	oled_send_cmd(0xF0);
	oled_send_cmd(CMD_Set_Mux_Ratio);
	oled_send_cmd(pixels_y-1);
	oled_send_cmd(CMD_Set_Display_Offset);
	oled_send_cmd(0x00);
	oled_send_cmd(CMD_Set_Charge_Pump);
	oled_send_cmd(CMD_EN_Charge_Pump);
	oled_send_cmd(CMD_Set_Display_Start_Line);
	oled_send_cmd(CMD_Set_Memory_Addressing_Mode);
	oled_send_cmd(22);///////////////////////////////////////////////////
	oled_send_cmd(CMD_Set_Segment_Remap+1);
	oled_send_cmd(CMD_Set_COM_Output_Scan_Direction_Remapped);
	oled_send_cmd(CMD_Set_COM_Pins_Hardware_Config);
	oled_send_cmd(0x12);/////////////////////////////////////////////////
	oled_send_cmd(CMD_Contrast_Control);
	oled_send_cmd(0xFF);
	oled_send_cmd(CMD_Set_Pre_charge_Period);
	oled_send_cmd(0xF1);
	oled_send_cmd(CMD_Set_VCOMH_Deselect_Level);
	oled_send_cmd(0x20);
	oled_send_cmd(CMD_Entire_Disp_Off);
	oled_send_cmd(CMD_Set_Normal_Disp);
	oled_send_cmd(0x2E);
	oled_send_cmd(CMD_Set_Disp_ON);
	///////////disp init sequence end
	delete_RAM();
}
#else
void oled_init(void)
{
	oled_send_cmd(CMD_Set_Disp_Off);
	oled_send_cmd(CMD_Set_Display_Clock_Divide_Ratio_Oscillator_Freq);
	oled_send_cmd(0xF0);
	oled_send_cmd(CMD_Set_Mux_Ratio);
	oled_send_cmd(pixels_y-1);
	oled_send_cmd(CMD_Set_Display_Offset);
	oled_send_cmd(0x00);
	oled_send_cmd(CMD_Set_Charge_Pump);
	oled_send_cmd(CMD_EN_Charge_Pump);
	oled_send_cmd(CMD_Set_Display_Start_Line);
	oled_send_cmd(CMD_Set_Memory_Addressing_Mode);
	oled_send_cmd(22);///////////////////////////////////////////////////
	oled_send_cmd(CMD_Set_Segment_Remap+1);
	oled_send_cmd(CMD_Set_COM_Output_Scan_Direction_Remapped);
	oled_send_cmd(CMD_Set_COM_Pins_Hardware_Config);
	oled_send_cmd(0x02);/////////////////////////////////////////////////
	oled_send_cmd(CMD_Contrast_Control);
	oled_send_cmd(0xFF);
	oled_send_cmd(CMD_Set_Pre_charge_Period);
	oled_send_cmd(0xF1);
	oled_send_cmd(CMD_Set_VCOMH_Deselect_Level);
	oled_send_cmd(0x20);
	oled_send_cmd(CMD_Entire_Disp_Off);
	oled_send_cmd(CMD_Set_Normal_Disp);
	oled_send_cmd(0x2E);
	oled_send_cmd(CMD_Set_Disp_ON);
	///////////disp init sequence end
	delete_RAM();
}
#endif// HW_SSD1306_64X48_WEMOS_OLED

#endif// (USE_HAL_DRIVER)	//STM32 mikrokontrollerhez

#if defined (USE_FULL_LL_DRIVER)	//STM32 mikrokontrollerhez  LL driverrel

#ifdef SSD1306_STM32_LL_IMPLEMENTATION_1

void print_disp_mat(void)
{
	uint8_t i=0, k=0;
	for(k=0; k<(pixels_y/8); k++)
	{
		go_to_col_page(0, k);
		LL_I2C_HandleTransfer(I2C1, Disp_Addr, LL_I2C_ADDRESSING_MODE_7BIT, 129, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
		LL_I2C_TransmitData8(I2C1, Next_Will_Be_Data);
		while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
		{
			asm("nop");
		}
		for(i=0; i<pixels_x; i++)
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
	for(k=0; k<(pixels_y/8); k++)
	{
		go_to_col_page(0, k);
		LL_I2C_HandleTransfer(I2C1, Disp_Addr, LL_I2C_ADDRESSING_MODE_7BIT, 129, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
		LL_I2C_TransmitData8(I2C1, Next_Will_Be_Data);
		while(LL_I2C_IsActiveFlag_TXE(I2C1) == 0)
		{
			asm("nop");
		}
		for(i=0; i<pixels_x; i++)
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
#endif //SSD1306_STM32_LL_IMPLEMENTATION_1


#ifdef SSD1306_STM32_LL_IMPLEMENTATION_2

void print_disp_mat(void)
{
	uint8_t k=0, l=0;
	__IO uint32_t tmpreg;

	for(k=0; k<(pixels_y/8); k++)
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

		for(l=0; l<pixels_x; l++)
		{
			if(l==(pixels_x-1))
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

	for(k=0; k<(pixels_y/8); k++)
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

		for(l=0; l<pixels_x; l++)
		{
			if(l==(pixels_x-1))
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
#endif //SSD1306_STM32_LL_IMPLEMENTATION_2

#endif//(USE_FULL_LL_DRIVER)	//STM32 mikrokontrollerhez LL driverrel

