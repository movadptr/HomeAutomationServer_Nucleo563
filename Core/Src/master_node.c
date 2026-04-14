/*
 * node.c
 *
 *  Created on: Jul 15, 2025
 *      Author: asdman
 */

///includes///////////////////////////////////////////////////

#include "master_node.h"
#include "LM71_SPI_temp_sensor.h"
#include "tx_api.h"

///variables//////////////////////////////////////////////////

volatile uint16_t** node_capabilities_pp = NULL;
volatile uint32_t** node_data_pp = NULL;

extern volatile uint8_t uart_state;
extern volatile uint16_t uart_rx_buff[N_MAX_RX_BUFF];
extern volatile uint16_t uart_rx_buff_indx;

TX_MUTEX uart2_mutex;

///functions//////////////////////////////////////////////////

/*
 * shitty blocking delay
 * only valid at 550MHz clock
 * input: how many times 100us
 * TODO build a not shitty one
 */
void delay100us_b(uint32_t us100)
{
	uint32_t cnt = 0;
	while(us100)
	{
		cnt = 0;
		while(cnt<55000)//100us
		{
			cnt++;
		}
		us100--;
	}
}

/*
 * desc: writes to the node in the given address
 */
int8_t N_MasterWriteNodeData(uint8_t nodeaddr, uint8_t function, uint32_t data)
{
	tx_mutex_get(&uart2_mutex, TX_WAIT_FOREVER);

	if((nodeaddr == 0) || (nodeaddr > N_NODE_AMOUNT))//bad addr
	{
		return -1;
	}

	uint16_t txdata[10] = {N_ADDR|nodeaddr, 10, N_CMD_WRITE_NACK, function, (uint16_t)((data>>0)&0xff), (uint16_t)((data>>8)&0xff), (uint16_t)((data>>16)&0xff), (uint16_t)((data>>24)&0xff), 0x0000, N_IDLE_CHAR};
	N_UART_Transmit(txdata, 10);

	//todo receive with ack cmd
	tx_mutex_put(&uart2_mutex);

	return 0;
}

/*
 * desc: Reads from the first node in the given address
 */
int8_t N_MasterReadNodeData(uint8_t nodeaddr, uint8_t function, volatile uint32_t* data)
{
	tx_mutex_get(&uart2_mutex, TX_WAIT_FOREVER);

	if((nodeaddr == 0) || (nodeaddr > N_NODE_AMOUNT))//bad addr
	{
		return -1;
	}

	uint16_t txdata[6] = {N_ADDR|nodeaddr, 6, N_CMD_READ, function, 0x0000, N_IDLE_CHAR};
	uint8_t rxlen = 0;

	N_UART_Transmit(txdata, 6);
	if(N_UART_receive(&rxlen)==0)
	{
		if(data != NULL)
		{
			*data = ((uart_rx_buff[4]<<0) | (uart_rx_buff[5]<<8) | (uart_rx_buff[6]<<16) | (uart_rx_buff[7]<<24));//save received data
		}
	}


	tx_mutex_put(&uart2_mutex);

	return 0;
}

/*
 * desc: Writes to the first node in the node list which has the given function
 */
void N_WriteEveryRelevantNode(uint8_t function, uint32_t data, volatile uint16_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp)
{
	tx_mutex_get(&uart2_mutex, TX_WAIT_FOREVER);

	uint16_t txdata[10] = {N_ADDR, 10, N_CMD_WRITE_NACK, function, (uint16_t)((data>>0)&0xff), (uint16_t)((data>>8)&0xff), (uint16_t)((data>>16)&0xff), (uint16_t)((data>>24)&0xff), 0x0000, N_IDLE_CHAR};
	for(uint8_t nodeindx=0; nodeindx<N_NODE_AMOUNT; nodeindx++)
	{
		for(uint8_t cindx=0; (*capabilities_ppp)[nodeindx][cindx]!=0; cindx++)
		{
			if((*capabilities_ppp)[nodeindx][cindx] == function)
			{
				txdata[0] = (N_ADDR|(nodeindx+1));
				delay100us_b(5);
				N_UART_Transmit(txdata, 10);
				//todo receive with ack cmd
				(*node_data_ppp)[nodeindx][cindx] = data;//also store it locally
			}else{}
		}
	}

	tx_mutex_put(&uart2_mutex);
}

/*
 * desc: Reads from the first node in the node list which has the given function
 */
void N_MasterReadFirstRelevantNodeData(uint8_t function, volatile uint32_t** data, volatile uint16_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp)
{
	tx_mutex_get(&uart2_mutex, TX_WAIT_FOREVER);

	uint16_t txdata[6] = {N_ADDR, 6, N_CMD_READ, function, 0x0000, N_IDLE_CHAR};
	uint8_t rxlen = 0;

	for(uint8_t nodeindx=0; nodeindx<N_NODE_AMOUNT; nodeindx++)
	{
		for(uint8_t cindx=0; (*capabilities_ppp)[nodeindx][cindx]!=0; cindx++)
		{
			if((*capabilities_ppp)[nodeindx][cindx] == function)
			{
				txdata[0] = (N_ADDR|(nodeindx+1));
				delay100us_b(5);
				N_UART_Transmit(txdata, 6);
				if(N_UART_receive(&rxlen)==0)
				{
					(*node_data_ppp)[nodeindx][cindx] = ((uart_rx_buff[4]<<0) | (uart_rx_buff[5]<<8) | (uart_rx_buff[6]<<16) | (uart_rx_buff[7]<<24));//save received data
					if(data!=NULL)
					{
						*data = &((*node_data_ppp)[nodeindx][cindx]);//copy received data location to use immediately
					}
					else
					{
						*data = NULL;
					}
				}
				return;
			}
		}
	}
	tx_mutex_put(&uart2_mutex);
}

/*
 * desc: read all data from every node
 */
void N_MasterRefreshAllNodeData(volatile uint16_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp)
{
	tx_mutex_get(&uart2_mutex, TX_WAIT_FOREVER);

	uint16_t txdata[6] = {N_ADDR, 6, N_CMD_READ, 0x0000, 0x0000, N_IDLE_CHAR};
	uint8_t rxlen = 0;

	for(uint8_t nodeindx=0; nodeindx<N_NODE_AMOUNT; nodeindx++)
	{
		for(uint8_t cindx=0; (*capabilities_ppp)[nodeindx][cindx]!=0; cindx++)
		{
			txdata[0] = (N_ADDR|(nodeindx+1));
			txdata[3] = (*capabilities_ppp)[nodeindx][cindx];
			N_UART_Transmit(txdata, 6);
			if(N_UART_receive(&rxlen)==0)
			{
				(*node_data_ppp)[nodeindx][cindx] = ((uart_rx_buff[4]<<0) | (uart_rx_buff[5]<<8) | (uart_rx_buff[6]<<16) | (uart_rx_buff[7]<<24));
			}
#ifdef DEBUG
			printf("a_%i__c_", txdata[0]);
			uint8_t asd = 0;
			while(asd<rxlen)
			{
				printf("%i_",uart_rx_buff[asd]);
				asd++;
			}
			printf(".\n\n\r");
#else
			LL_mDelay(3);//TODO debug, why is it working with this?????
#endif
		}
	}

	tx_mutex_put(&uart2_mutex);
}

void N_MasterInitNodeNetwork(volatile uint16_t*** capabilitiesf_ppp, volatile uint32_t*** node_data_ppp)
{
	//no mutex, called before rtos init

	uint16_t addr = 0;

	uint16_t tx[5] = {N_ADDR, 5, N_CMD_GET_CAPABILITIES, 0x0000/*dummy tmp CRC val*/, N_IDLE_CHAR};
	uint8_t rxlen = 0;

	//allocate mem for capabilities
	*capabilitiesf_ppp = (volatile uint16_t**)calloc(N_NODE_AMOUNT, sizeof(volatile uint16_t*));
	if(*capabilitiesf_ppp == NULL)	{ Error_Handler();}
	//allocate mem for data
	*node_data_ppp = (volatile uint32_t**)calloc(N_NODE_AMOUNT, sizeof(volatile uint32_t*));
	if(*node_data_ppp == NULL)	{ Error_Handler();}


	while(addr < N_NODE_AMOUNT)
	{
		tx[0] = (N_ADDR|(addr+1));
		delay100us_b(5);
		N_UART_Transmit(tx, 5);
		if(N_UART_receive(&rxlen)==0)
		{
			N_MasterStoreCapabilities(uart_rx_buff, rxlen, capabilitiesf_ppp, addr, node_data_ppp);
		}
		addr++;
	}
}

int8_t N_UART_receive(uint8_t* len)
{
	uint32_t cnt = 0;
	LL_GPIO_ResetOutputPin(RS485_DIR_GPIO_Port, RS485_DIR_Pin);//set RS485 transceiver to rx mode //bab

	while((uart_state != UART_WAIT_FOR_PROCESS) && (cnt<10000000))
	{
		__NOP();
		//TODO timeout
		//ha a timeout lejárt return -1
		cnt++;
	}

	if(cnt >= 10000000)
	{
		uart_rx_buff[1] = 10;
		uart_rx_buff[2] = N_CMD_READ;
		uart_rx_buff[3] = N_FUNCTION_NONE;
		uart_rx_buff[4] = 0;
		uart_rx_buff[5] = 0;
		uart_rx_buff[6] = 0;
		uart_rx_buff[7] = 0;
		uart_rx_buff[8] = 0;
		uart_rx_buff[9] = 0x1ff;
		uart_rx_buff_indx=10;
		HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
	}


	if(len != NULL)
	{
		(*len) = uart_rx_buff_indx;
	}

	return 0;
}

/*
 * txbuff -
 * len - length of buffer (num of uint16_t elements)
 */
void N_UART_Transmit(uint16_t* txbuff, uint8_t len)
{
	uint8_t indx = 0;

	uart_state = UART_TRANSMITTING;
	LL_GPIO_SetOutputPin(RS485_DIR_GPIO_Port, RS485_DIR_Pin);//set RS485 transceiver to tx mode
	while(indx<len)
	{
		LL_USART_TransmitData9(USART2, txbuff[indx]);
		indx ++;
		while(LL_USART_IsActiveFlag_TC(USART2) == 0)
		{
			__NOP();
		}
	}
	LL_GPIO_ResetOutputPin(RS485_DIR_GPIO_Port, RS485_DIR_Pin);//set RS485 transceiver to rx mode

	uart_state = UART_READY;
}

void N_MasterStoreCapabilities(volatile uint16_t* rxbuff, uint8_t rxlen, volatile uint16_t*** capabilitiesf_ppp, uint8_t addr, volatile uint32_t*** node_data_ppp)
{
	//ADDR DLC CMD DATA1 DATA2 ... DATAN CRC IDLE
	//0    1   2   3     4     ...
	uint8_t clen = 0;

	if(rxlen > 5)
	{
		clen = (rxlen-5);//ADDR+DLC+CMD+CRC+IDLE
	}
	else
	{
		Error_Handler();
	}

	//allocate mem for this node capabilities
	(*capabilitiesf_ppp)[addr] = (uint16_t*)calloc(clen+1, sizeof(uint16_t));//+1 to have a fix zero byte at the end //calloc ensures termination zero is there
	if((*capabilitiesf_ppp)[addr] == NULL)	{ Error_Handler();}
	//allocate mem for this node data
	(*node_data_ppp)[addr] = (uint32_t*)calloc(clen, sizeof(uint32_t));
	if((*node_data_ppp)[addr] == NULL)	{ Error_Handler();}

	//copy capabilities to allocated mem
	uint8_t rxindx = 3;
	uint8_t capindx = 0;
	while(capindx <= clen)// "<=" to incorporate terminating 0
	{
		(*capabilitiesf_ppp)[addr][capindx] = rxbuff[rxindx];
		capindx++;
		rxindx++;
	}
}


/*
 * desc: Gets the first node data in the local variable structure which has the given function
 */
void N_MasterGetFirstRelevantNodeData(uint8_t function, volatile uint32_t* data, volatile uint16_t*** capabilities_ppp, volatile uint32_t*** node_data_ppp)
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
 * desc: read local stored data
 * nd - data block for node
 * data - pointer to variable where to store the value
 * function - what kind of data to get (values are in node.h at section: node functions)
 * capabilitiesf - block which stores what node functions are available in the current node
 */
int8_t N_GetNodeData(volatile uint32_t* nd, uint32_t* data, uint8_t function, volatile uint16_t* capabilitiesf)
{
	uint8_t i = 0;
	while(capabilitiesf[i])
	{
		if(capabilitiesf[i] == function)
		{
			*data = nd[i];
			return 0;
		}
		i++;
	}
	return -1;
}

/*
 * desc: overwrite local stored data
 * nd - data block for node
 * data - data to store
 * function - what kind of data to store (values are in node.h at section: node functions)
 * capabilitiesf - block which stores what node functions are vailable in the current node
 * datachangef - variable to notify caller where the data was stored
 */
int8_t N_StoreNodeData(volatile uint32_t* nd, uint32_t data, uint8_t function, volatile uint16_t* capabilitiesf, volatile uint8_t* datachangef)
{
	uint8_t i = 0;
	while(capabilitiesf[i])
	{
		if(capabilitiesf[i] == function)
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
