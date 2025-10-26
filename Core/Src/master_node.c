/*
 * node.c
 *
 *  Created on: Jul 15, 2025
 *      Author: asdman
 */

///includes///////////////////////////////////////////////////

#include "master_node.h"
#include "LM71_SPI_temp_sensor.h"

///variables//////////////////////////////////////////////////

extern UART_HandleTypeDef huart2;
uint8_t** node_capabilities_pp = NULL;
uint32_t** node_data_pp = NULL;

///functions//////////////////////////////////////////////////

void N_WriteNode(uint8_t nodeaddr, uint8_t function, uint32_t data)
{
	uint8_t txdata[16] = {nodeaddr, N_ADDR_MSB, N_CMD_WRITE, N_DATA_MSB, function, N_DATA_MSB, (uint8_t)((data>>0)&0xff), N_DATA_MSB, (uint8_t)((data>>8)&0xff), N_DATA_MSB, (uint8_t)((data>>16)&0xff), N_DATA_MSB, (uint8_t)((data>>24)&0xff), N_DATA_MSB, 0xff, 0x01};
	HAL_UART_Transmit(&huart2, txdata, 8, 100);
}

void N_WriteEveryRelevantNode(uint8_t function, uint32_t data, uint8_t*** capabilities_ppp, uint32_t*** node_data_ppp)
{
	uint8_t txdata[16] = {0x00, N_ADDR_MSB, N_CMD_WRITE, N_DATA_MSB, function, N_DATA_MSB, (uint8_t)((data>>0)&0xff), N_DATA_MSB, (uint8_t)((data>>8)&0xff), N_DATA_MSB, (uint8_t)((data>>16)&0xff), N_DATA_MSB, (uint8_t)((data>>24)&0xff), N_DATA_MSB, 0xff, 0x01};

	for(uint8_t nodeindx=0; nodeindx<N_NODE_AMOUNT; nodeindx++)
	{
		for(uint8_t cindx=0; (*capabilities_ppp)[nodeindx][cindx]!=0; cindx++)
		{
			if((*capabilities_ppp)[nodeindx][cindx] == function)
			{
				txdata[0] = (nodeindx+1);
				HAL_UART_Transmit(&huart2, txdata, 8, 100);
				(*node_data_ppp)[nodeindx][cindx] = data;//also store it locally
			}
		}
	}
}

/*
 * Reads the first node in the node list which has the given function
 */
void N_MasterReadFirstRelevantNodeData(uint8_t function, uint32_t** data, uint8_t*** capabilities_ppp, uint32_t*** node_data_ppp)
{
	uint8_t txdata[16] = {0x00, N_ADDR_MSB, N_CMD_READ, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0xff, 0x01};
	uint8_t* rxdata = calloc(N_MAX_RX_BUFF, sizeof(uint8_t));
	uint16_t rxlen= 0;

	for(uint8_t nodeindx=0; nodeindx<N_NODE_AMOUNT; nodeindx++)
	{
		for(uint8_t cindx=0; (*capabilities_ppp)[nodeindx][cindx]!=0; cindx++)
		{
			if((*capabilities_ppp)[nodeindx][cindx] == function)
			{
				txdata[0] = (nodeindx+1);
				txdata[4] = function;
				HAL_UART_Transmit(&huart2, txdata, 8, 100);
				HAL_UARTEx_ReceiveToIdle(&huart2, rxdata, N_MAX_RX_BUFF, &rxlen, 1000);
				(*node_data_ppp)[nodeindx][cindx] = ((rxdata[6]<<0) | (rxdata[8]<<8) | (rxdata[10]<<16) | (rxdata[12]<<24));//save received data
				if(data!=NULL)
				{
					*data = &((*node_data_ppp)[nodeindx][cindx]);//copy received data location to use immediately
				}
				return;
			}
		}
	}
	free(rxdata);
}

void N_MasterRefreshAllNodeData(uint8_t*** capabilities_ppp, uint32_t*** node_data_ppp)
{
	//kell egy mutex
	uint8_t txdata[16] = {0x00, N_ADDR_MSB, N_CMD_READ, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0xff, 0x01};
	uint8_t* rxdata = calloc(N_MAX_RX_BUFF, sizeof(uint8_t));
	uint16_t rxlen= 0;

	for(uint8_t nodeindx=0; nodeindx<N_NODE_AMOUNT; nodeindx++)
	{
		for(uint8_t cindx=0; (*capabilities_ppp)[nodeindx][cindx]!=0; cindx++)
		{
			txdata[0] = (nodeindx+1);
			txdata[4] = (*capabilities_ppp)[nodeindx][cindx];
			HAL_UART_Transmit(&huart2, txdata, 8, 100);
			HAL_UARTEx_ReceiveToIdle(&huart2, rxdata, N_MAX_RX_BUFF, &rxlen, 1000);
			(*node_data_ppp)[nodeindx][cindx] = ((rxdata[6]<<0) | (rxdata[8]<<8) | (rxdata[10]<<16) | (rxdata[12]<<24));
		}
	}
	free(rxdata);
}

int N_MasterHandleRxData(uint8_t* rxbuff, uint8_t rxlen, uint32_t* node_data_p, uint8_t* capabilitiesf_p)
{
	uint8_t rxindx = 2;//set to after master addr
	uint32_t tmp_data = 0;

	while(rxindx < rxlen)
	{
		switch(rxbuff[rxindx])
		{

			case N_CMD_READ:	//master receives data from slave
								tmp_data = (rxbuff[rxindx+4] | (rxbuff[rxindx+6]<<8) | (rxbuff[rxindx+8]<<16) | (rxbuff[rxindx+10]<<24));//get data from rxbuff
								N_StoreNodeData(node_data_p,tmp_data, rxbuff[rxindx+2], capabilitiesf_p, NULL);//save data to node data block
								break;

			case N_CMD_WRITE:	//no such response is possible from slave
								break;

			case N_CMD_GET_CAPABILITIES:	//handled only at init phase
											break;

			case 0xff:	//idle word low byte, end of response
						break;

			default:	return -1;
						break;//bab
		}
		rxindx += N_BUFF_SECTION_SIZE;
	}

	return 0;
}

void N_MasterInitNodeNetwork(uint8_t*** capabilitiesf_ppp, uint32_t*** node_data_ppp)
{
	uint8_t addr = 0;

	uint8_t tx[16] = {0x00, N_ADDR_MSB, N_CMD_GET_CAPABILITIES, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00, N_DATA_MSB, 0x00,N_DATA_MSB, 0xff, 0x01};
	uint8_t* rx = calloc(N_MAX_RX_BUFF, sizeof(uint8_t));
	uint8_t rxlen = 0;

	//allocate mem for capabilities
	*capabilitiesf_ppp = (uint8_t**)calloc(N_NODE_AMOUNT, sizeof(uint8_t*));
	if(*capabilitiesf_ppp == NULL)	{ Error_Handler();}
	//allocate mem for data
	*node_data_ppp = (uint32_t**)calloc(N_NODE_AMOUNT, sizeof(uint32_t*));
	if(*node_data_ppp == NULL)	{ Error_Handler();}


	while(addr < N_NODE_AMOUNT)
	{
		tx[0] = addr+1;
		HAL_UART_Transmit(&huart2, tx, 8, 100);
		HAL_UARTEx_ReceiveToIdle(&huart2, rx, N_MAX_RX_BUFF, (uint16_t*)&rxlen, 1000);
		N_MasterStoreCapabilities(rx, rxlen*2, capabilitiesf_ppp, addr, node_data_ppp);
		addr++;
	}
	free(rx);
}

void N_MasterStoreCapabilities(uint8_t* rxbuff, uint8_t rxlen, uint8_t*** capabilitiesf_ppp, uint8_t addr, uint32_t*** node_data_ppp)
{
	uint8_t clen = ((rxlen-4)/2);//(((rxlen-6)/2)+1);//-2 master addr, -2 cmd, -2 terminating idle word, /2 because every word is 2 byte, +1 termination zero in capabilities

	//allocate mem for this node capabilities
	(*capabilitiesf_ppp)[addr] = (uint8_t*)calloc(clen, sizeof(uint8_t));//calloc ensures termination zero is there
	if((*capabilitiesf_ppp)[addr] == NULL)	{ Error_Handler();}
	//allocate mem for this node data
	(*node_data_ppp)[addr] = (uint32_t*)calloc(clen-1, sizeof(uint32_t));//clen-1 -> don't need the terminating zero
	if((*node_data_ppp)[addr] == NULL)	{ Error_Handler();}

	uint8_t rxindx = 4;

	for(clen=0; rxindx<(rxlen-2); rxindx+=2, clen++)
	{
		(*capabilitiesf_ppp)[addr][clen] = rxbuff[rxindx];
	}
}


/*
 * txbuff - tranmit buffer to work on (designed to work with 9 bit uart, so the stride is 2byte)
 * offset - at which index to start putting data in, after function finished it points to the next free address
 * data - normal data string containing data to append
 * datalen - length of data to append
 */
int N_AppendTxBuff(uint8_t* txbuff, uint8_t* offset, uint8_t* data, uint8_t datalen)
{
	uint8_t i = 0;

	while(i<datalen)
	{
		if(*offset < N_MAX_TX_BUFF)
		{
			txbuff[*offset] = data[i];
			i++;
			*offset+=2;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

/*
 * Reads the first node data in the local variable structure which has the given function
 */
void N_MasterGetFirstRelevantNodeData(uint8_t function, uint32_t* data, uint8_t*** capabilities_ppp, uint32_t*** node_data_ppp)
{
	for(uint8_t nodeindx=0; nodeindx<N_NODE_AMOUNT; nodeindx++)
	{
		for(uint8_t cindx=0; (*capabilities_ppp)[nodeindx][cindx]!=0; cindx++)
		{
			if((*capabilities_ppp)[nodeindx][cindx] == function)
			{
				if(data!=NULL)
				{
					*data = (*node_data_ppp)[nodeindx][cindx];
				}
				return;
			}
		}
	}
}

/*
 * nd - data block for node
 * data - pointer to variable where to store the value
 * data type - what kind of data to get (values are in node.h at section: node functions)
 * capabilitiesf - block which stores what node functions are available in the current node
 */
int N_GetNodeData(uint32_t* nd, uint32_t* data, uint8_t data_type, const uint8_t* capabilitiesf)
{
	uint8_t i = 0;
	while(capabilitiesf[i])
	{
		if(capabilitiesf[i] == data_type)
		{
			*data = nd[i];
			return 0;
		}
		i++;
	}
	return -1;
}

/*
 * nd - data block for node
 * data - data to store
 * data type - what kind of data to store (values are in node.h at section: node functions)
 * capabilitiesf - block which stores what node functions are vailable in the current node
 * datachangef - variable to notify caller where the data was stored
 */
int N_StoreNodeData(uint32_t* nd, uint32_t data, uint8_t data_type, const uint8_t* capabilitiesf, volatile uint8_t* datachangef)
{
	uint8_t i = 0;
	while(capabilitiesf[i])
	{
		if(capabilitiesf[i] == data_type)
		{
			if(nd[i] != data)
						  
			{
				nd[i] = data;
				if(datachangef != NULL)
				{
					*datachangef |= (1<<i);
				}
			}

			return 0;
		}
		i++;
	}
	return -1;
}
