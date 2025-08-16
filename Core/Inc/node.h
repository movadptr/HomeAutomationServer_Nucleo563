/*
 * node.h
 *
 *  Created on: Jul 13, 2025
 *  Author: asdman
 */

#ifndef INC_NODE_H_
#define INC_NODE_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

/////user config////////////////////////////////////////////////////////////////////////
//determine which type of device to compile for
#define N_MASTER_SIDE
//#define N_SLAVE_SIDE

#define N_NODE_AMOUNT	1U//how many nodes are present in the network
//////user config end///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#define N_MASTER_ADDR	0x00

#define N_DATA_MSB		0x00
#define N_ADDR_MSB		0x01

//buffer sizes
#define N_MAX_RX_BUFF	100U
#define N_MAX_TX_BUFF	100U

#define N_BUFF_SECTION_SIZE	0x12//how many bytes is a section in tx or rx buffer (cmd + dataid + data)

//commands
#define N_CMD_READ				0x01
#define N_CMD_WRITE				0x02
#define N_CMD_GET_CAPABILITIES	0x03

//node functions
#define N_FUNCTION_NONE					0x00
#define N_EAST_SHADER					0x01
#define N_SHADER_OPEN_POS	(-100)
#define N_SHADER_CLOSED_POS	100

#define N_WEST_SHADER 					0x02
#define N_NORTH_SHADER					0x03
#define N_SOUTH_SHADER					0x04
#define N_LIVING_ROOM_SMALL_LIGHT		0x05
#define N_BEDROOM_SMALL_LIGHT			0x06
#define N_ROOM_TEMPERATURE_SENSOR		0x07
#define N_OUTSIDE_TEMPERATURE_SENSOR	0x08
#define N_FRONT_DOOR_LOCK_POS_SENSOR	0x09
#define N_INSIDE_LIGHT_SENSOR			0x0A
#define N_OUTSIDE_LIGHT_SENSOR			0x0B
#define N_INDICATOR_LED					0x0C

//compile switches for node functionality
//TODO use this
#define USE_N_EAST_SHADER					0x0001
#define USE_N_WEST_SHADER 					0x0002
#define USE_N_NORTH_SHADER					0x0004
#define USE_N_SOUTH_SHADER					0x0008
#define USE_N_LIVING_ROOM_SMALL_LIGHT		0x0010
#define USE_N_BEDROOM_SMALL_LIGHT			0x0020
#define USE_N_ROOM_TEMPERATURE_SENSOR		0x0040
#define USE_N_OUTSIDE_TEMPERATURE_SENSOR	0x0080
#define USE_N_FRONT_DOOR_LOCK_POS_SENSOR	0x0100
#define USE_N_INSIDE_LIGHT_SENSOR			0x0200
#define USE_N_OUTSIDE_LIGHT_SENSOR			0x0400
#define USE_N_INDICATOR_LED					0x0800

void sec_timer_interrupt_callback(void);								

int N_MasterHandleRxData(uint8_t* rxbuff, uint8_t rxlen, uint32_t* node_data_p, uint8_t* capabilitiesf_p);
void N_MasterStoreCapabilities(uint8_t* rxbuff, uint8_t rxlen, uint8_t*** capabilitiesf_ppp, uint8_t addr, uint32_t*** node_data_ppp);
void N_MasterInitNodeNetwork(uint8_t*** capabilitiesf_ppp, uint32_t*** node_data_ppp);
void N_WriteNode(uint8_t nodeaddr, uint8_t function, uint32_t data);
void N_MasterReadFirstRelevantNodeData(uint8_t function, uint32_t** data, uint8_t*** capabilities_pp, uint32_t*** node_data_pp);
void N_WriteEveryRelevantNode(uint8_t function, uint32_t data, uint8_t*** capabilities_pp, uint32_t*** node_data_pp);
void N_MasterRefreshAllNodeData(uint8_t*** capabilities_pp, uint32_t*** node_data_pp);

int N_SlaveHandleRxTxData(uint8_t* rxbuff, uint8_t rxlen, uint8_t* txbuff, uint32_t* node_data, const uint8_t* capabilitiesf, volatile uint8_t* datachangef);
int N_AssembleCapabilitiesTxFrame(uint8_t* txbufff, const uint8_t* capabilitiesf);

int N_AppendTxBuff(uint8_t* txbuff, uint8_t* offset, uint8_t* data, uint8_t datalen);
int N_GetNodeData(uint32_t* nd, uint32_t* data, uint8_t data_type, const uint8_t* capabilitiesf);
int N_StoreNodeData(uint32_t* nd, uint32_t data, uint8_t data_type, const uint8_t* capabilitiesf, volatile uint8_t* datachangef);

#endif /* INC_NODE_H_ */
