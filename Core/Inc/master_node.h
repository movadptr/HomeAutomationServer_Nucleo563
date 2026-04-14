/*
 * node.h
 *
 *  Created on: Jul 13, 2025
 *  Author: movadptr
 *
 *  message format:
 *  +-----------+-----+-----+-----------------------+---------------+---------------+---------------+---------------+   +-----+-------------------------------+
 *  | Node addr | DLC | CMD | function(if there is) | data "word" 0 | data "word" 1 | data "word" 2 | data "word" x |...| CRC |terminating 0x1ff (idle frame) |
 *  +-----------+-----+-----+-----------------------+---------------+---------------+---------------+---------------+   +-----+-------------------------------+
 *
 *	0x100 is the address of the master
 *  0x1fe is the broadcast address
 *	0x1ff is the termination (idle) frame
 *
 *	-master initiates data transfer by addressing a slave
 *	-address word MSB is 1, data word MSB is 0
 *	-a "word" is 9 bit wide data on the bus, but 16 bit wide containers used in the buffer
 *	-DLC includes every word (including Node addr and termination)
 *	-whole msg length can be maximum 255 word
 */

#ifndef INC_MASTER_NODE_H_
#define INC_MASTER_NODE_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

/////user config////////////////////////////////////////////////////////////////////////
//determine which type of device to compile for
#define N_NODE_AMOUNT	2U//how many nodes are present in the network

//////user config end///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#define N_MASTER_ADDR	0x0100

#define N_DATA			0x0000
#define N_ADDR			0x0100

#define N_IDLE_CHAR		0x01ff
#define N_DLC_POS		1U

//buffer sizes
#define N_MAX_RX_BUFF	50U
#define N_MAX_TX_BUFF	50U

//commands
#define N_CMD_READ				0x0001
#define N_CMD_WRITE_NACK		0x0002
#define N_CMD_WRITE_ACK			0x0003
#define N_CMD_GET_CAPABILITIES	0x0004

//node functions
#define N_FUNCTION_NONE					0x00

#define N_EAST_SHADER					0x01
#define N_SHADER_OPEN_POS	0
#define N_SHADER_CLOSED_POS	(-100)

#define N_WEST_SHADER 					0x02
#define N_NORTH_SHADER					0x03
#define N_SOUTH_SHADER					0x04
#define N_LIVING_ROOM_SMALL_LIGHT		0x05
#define N_BEDROOM_SMALL_LIGHT			0x06
#define N_ROOM_TEMPERATURE_SENSOR		0x07
#define N_OUTSIDE_TEMPERATURE_SENSOR	0x08
#define N_FRONT_DOOR_POS_SENSOR			0x09
#define N_INSIDE_LIGHT_SENSOR			0x0A
#define N_OUTSIDE_LIGHT_SENSOR			0x0B
#define N_INDICATOR_LED					0x0C

/////////////////////////
//usart state defines
#define UART_READY				0x00
#define UART_RX_STARTED			0x01
#define UART_WAIT_FOR_PROCESS	0x02
#define UART_TRANSMITTING		0x03

#define UART_TIMEOUT_VAL_S		1U //sec
#define UART_TIMEOUT_OFF		0xff//shows that the timeout counting is not going on

void delay100us_b(uint32_t us100);

void sec_timer_interrupt_callback(void);								

void N_UART_Transmit(uint16_t* txbuff, uint8_t len);
int8_t N_UART_receive(uint8_t* len);
void N_MasterStoreCapabilities(volatile uint16_t* rxbuff, uint8_t rxlen, volatile uint16_t*** capabilitiesf_ppp, uint8_t addr, volatile uint32_t*** node_data_ppp);
void N_MasterInitNodeNetwork(volatile uint16_t*** capabilitiesf_ppp, volatile uint32_t*** node_data_ppp);
int8_t N_MasterWriteNodeData(uint8_t nodeaddr, uint8_t function, uint32_t data);
int8_t N_MasterReadNodeData(uint8_t nodeaddr, uint8_t function, volatile uint32_t* data);
void N_MasterReadFirstRelevantNodeData(uint8_t function, volatile uint32_t** data, volatile uint16_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp);
void N_WriteEveryRelevantNode(uint8_t function, uint32_t data, volatile uint16_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp);
void N_MasterRefreshAllNodeData(volatile uint16_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp);

int N_AppendTxBuff(uint8_t* txbuff, uint8_t* offset, uint8_t* data, uint8_t datalen);
void N_MasterGetFirstRelevantNodeData(uint8_t function, volatile uint32_t* data, volatile uint16_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp);
int8_t N_GetNodeData(volatile uint32_t* nd, uint32_t* data, uint8_t data_type, volatile uint16_t* capabilitiesf);
int8_t N_StoreNodeData(volatile uint32_t* nd, uint32_t data, uint8_t data_type, volatile uint16_t* capabilitiesf, volatile uint8_t* datachangef);

#endif /* INC_MASTER_NODE_H_ */
