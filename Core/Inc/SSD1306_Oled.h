//
// 2019.01.06  20:31
// Author: Póti Szabolcs
// OLED Display with SSD1306 controller
//

#ifndef SSD1306_OLED_H_
#define SSD1306_OLED_H_

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//SET SCREEN SIZE
//in horiontal orientation
#define pixels_x	128
#define pixels_y	32

//SET HARDWARE
//#define HW_SSD1306_64X48_WEMOS_OLED
//#define HW_SSD1306_128X64_OLED
#define HW_SSD1306_128X32_OLED

//SET DISPLAY ADDRESS
#define Disp_Addr	0x78	//display address for write

//SET DRIVER IMPLEMENTATION
#define SSD1306_STM32_HAL
//#define SSD1306_STM32_LL_IMPLEMENTATION_1
//#define SSD1306_STM32_LL_IMPLEMENTATION_2
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

#define Next_Will_Be_Data				0x40
#define Next_Will_Be_Cmd				0x80

//Commands:
#define CMD_Contrast_Control			0x81//value after this cmd
#define CMD_Entire_Disp_On				0xA5
#define CMD_Entire_Disp_Off				0xA4
#define CMD_Set_Normal_Disp				0xA6
#define CMD_Set_Inverse_Disp			0xA7
#define	CMD_Set_Disp_ON					0xAF
#define	CMD_Set_Disp_Off				0xAE

#define CMD_Set_Lower_Column_Start_Address_for_Page_Addressing_Mode		0x00//+value
#define CMD_Set_Higher_Column_Start_Address_for_Page_Addressing_Mode	0x10//+value
#define CMD_Set_Memory_Addressing_Mode									0x20//value after this cmd
#define CMD_Set_Column_Address											0x21//2 value after this cmd
#define CMD_Set_Page_Address											0x22//2 value after this cmd
#define CMD_Set_Page_Start_Address_for_Page_Addressing_Mode				0xB0//+value
#define CMD_Set_Display_Start_Line										0x40//+value
#define CMD_Set_Segment_Remap											0xA0//+value
#define CMD_Set_Mux_Ratio												0xA8//value after this cmd
#define CMD_Set_COM_Output_Scan_Direction_Normal						0xc0
#define CMD_Set_COM_Output_Scan_Direction_Remapped						0xc8
#define CMD_Set_Display_Offset											0xD3//value after this cmd
#define CMD_Set_COM_Pins_Hardware_Config								0xDA//value after this cmd
#define CMD_Set_Display_Clock_Divide_Ratio_Oscillator_Freq				0xD5//value after this cmd
#define CMD_Set_Pre_charge_Period										0xD9//value after this cmd
#define CMD_Set_VCOMH_Deselect_Level									0xDB//value after this cmd
#define CMD_NOP															0xE3
#define CMD_Set_Charge_Pump												0x8D//value after this cmd
#define CMD_EN_Charge_Pump												0x14//send it after set charge pump

///////////////commands end//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/************************************************************************/
/* initializes display after power on                                   */
/************************************************************************/
void oled_init(void);

/************************************************************************/
/* send command to display                                              */
/************************************************************************/
void oled_send_cmd(uint8_t data);

/************************************************************************/
/* sends data to display ram                                            */
/************************************************************************/
void oled_send_data(uint8_t data);

/************************************************************************/
/*  sets col and page start address and set sets the end address to max */
/************************************************************************/
void go_to_col_page(uint8_t x, uint8_t y);

/************************************************************************/
/* deletes RAM content and sets the display pointer to 0,0 (x,y)        */
/************************************************************************/
void delete_RAM(void);

/************************************************************************/
/*  prints the whole display content stored in the AVR                  */
/************************************************************************/
void print_disp_mat(void);


#endif //SSD1306_OLED_H_
