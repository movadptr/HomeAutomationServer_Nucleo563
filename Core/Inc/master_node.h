/*
 * node.h
 *
 *  Created on: Jul 13, 2025
 *  Author: asdman
 *
 *  message format:
 *  +-----------+-------+------------+--------------+--------------+--------------+--------------+-------+------------+--------------+     +--------------------------------+
 *  | node addr | CMD 0 | function 0 | data word 00 | data word 01 | data word 02 | data word 03 | CMD 1 | function 1 | data word 10 | ... | terminating 0x1ff (idle frame) |
 *  +-----------+-------+------------+--------------+--------------+--------------+--------------+-------+------------+--------------+     +--------------------------------+
 *
 *  0x100 and 0x1ff node addresses are forbidden to use
 *	0x100 is the address of the master //no address match configured, just to show
 *	0x1ff is the termination (idle) frame used to trigger rx interrupt in the node
 *
 *	-master initiates data transfer by addressing a slave
 *	-address word MSB is 1, data word MSB is 0
 *	-slave respond s with same kind of msg, for addr it writes 0 annd repeats every read command with the 4 byte data after it
 *	//not true anymore: -if cmd is get capabilities and capabilities are longer than 4 byte the have to repeat the same command and continue listing the capabilities in the next 4 byte
 *	-if cmd is get capabilities after master addr and get capabilities cmd all capabilities are listed then terminated with idle word
 *	-get capabilities cmd can't be mixed with other commands
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

void sec_timer_interrupt_callback(void);								

void N_MasterStoreCapabilities(uint8_t* rxbuff, uint8_t rxlen, volatile uint8_t*** capabilitiesf_ppp, uint8_t addr, volatile uint32_t*** node_data_ppp);
void N_MasterInitNodeNetwork(volatile uint8_t*** capabilitiesf_ppp, volatile uint32_t*** node_data_ppp);
int8_t N_MasterWriteNodeData(uint8_t nodeaddr, uint8_t function, uint32_t data);
int8_t N_MasterReadNodeData(uint8_t nodeaddr, uint8_t function, volatile uint32_t* data);
void N_MasterReadFirstRelevantNodeData(uint8_t function, volatile uint32_t** data, volatile uint8_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp);
void N_WriteEveryRelevantNode(uint8_t function, uint32_t data, volatile uint8_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp);
void N_MasterRefreshAllNodeData(volatile uint8_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp);

int N_AppendTxBuff(uint8_t* txbuff, uint8_t* offset, uint8_t* data, uint8_t datalen);
void N_MasterGetFirstRelevantNodeData(uint8_t function, uint32_t* data, volatile uint8_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp);
int N_GetNodeData(uint32_t* nd, uint32_t* data, uint8_t data_type, const uint8_t* capabilitiesf);
int N_StoreNodeData(uint32_t* nd, uint32_t data, uint8_t data_type, const uint8_t* capabilitiesf, volatile uint8_t* datachangef);

#endif /* INC_MASTER_NODE_H_ */
