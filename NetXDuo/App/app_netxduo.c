/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @author  MCD Application Team
  * @brief   NetXDuo applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
#include "nxd_dhcp_client.h"
/* USER CODE BEGIN Includes */
#include "nx_stm32_eth_config.h"
#include "main.h"
#include <time.h>
#include "nxd_sntp_client.h"
#include "nxd_dns.h"
#include "nx_ip.h"
#include "nx_secure_tls_api.h"
#include "nx_web_http_client.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD      NxAppThread;
NX_PACKET_POOL NxAppPool;
NX_IP          NetXDuoEthIpInstance;
TX_SEMAPHORE   DHCPSemaphore;
NX_DHCP        DHCPClient;
/* USER CODE BEGIN PV */
TX_THREAD AppUsartThread;
TX_THREAD AppHTTPSclientThread;
TX_THREAD AppSNTPThread;
TX_THREAD AppUDPThread;
TX_THREAD AppLinkThread;

// Define NetX global data structures
NX_UDP_SOCKET UDPSocket;
ULONG IpAddress;
ULONG NetMask;
ULONG PublicIpAddress;
extern HAdata_S HAdata;

NX_DNS                  DnsClient;

NX_SNTP_CLIENT			SntpClient;
TX_EVENT_FLAGS_GROUP	SntpFlags;
CHAR					buffer[64];
struct 					tm timeInfos;
RTC_HandleTypeDef 		RtcHandle;
UINT  					iface_index = 0;//set the SNTP network interface to the primary interface.

extern volatile uint8_t** node_capabilities_pp;
extern volatile uint32_t** node_data_pp;

uint8_t time_update_after_boot_flag = 0;

TX_SEMAPHORE HTTPSSemaphore;

#define TLS_PACKET_BUFFER_SIZE     18000
UCHAR tls_packet_buffer[TLS_PACKET_BUFFER_SIZE];
NX_WEB_HTTP_CLIENT http_client;
NX_SECURE_TLS_SESSION tls_session;
#ifndef NX_WEB_HTTP_SERVER_SESSION_MAX
#define NX_WEB_HTTP_SERVER_SESSION_MAX          2
#endif
static CHAR crypto_metadata_client[20000 * NX_WEB_HTTP_SERVER_SESSION_MAX];
static NX_SECURE_X509_CERT trusted_certificate;
// Trusted CA certificate for https Client
#define ca_cert_der 		gts_root_r1_der
#define ca_cert_der_len 	gts_root_r1_der_len
extern const unsigned int gts_root_r1_der_len;
extern unsigned char gts_root_r1_der[1371];
static NX_SECURE_X509_CERT trusted_certificate;
static NX_SECURE_X509_CERT remote_certificate;
NX_SECURE_X509_CERT remote_issuer;
static UCHAR remote_cert_buffer[18000];
static UCHAR remote_issuer_buffer[18000];
extern NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static VOID App_Main_Thread_Entry (ULONG thread_input);
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr);
/* USER CODE BEGIN PFP */
static VOID App_HTTPS_Thread_Entry(ULONG thread_input);
static VOID App_UDP_Thread_Entry(ULONG thread_input);
static void App_SNTP_Thread_Entry(ULONG info);
static VOID App_Link_Thread_Entry(ULONG thread_input);

UINT tls_setup_callback(NX_WEB_HTTP_CLIENT *client_ptr, NX_SECURE_TLS_SESSION *tls_session);
VOID http_response_callback(NX_WEB_HTTP_CLIENT *client_ptr, CHAR *field_name, UINT field_name_length, CHAR *field_value, UINT field_value_length);
static UINT kiss_of_death_handler(NX_SNTP_CLIENT *client_ptr, UINT KOD_code);
static UINT dns_create(NX_DNS *dns_ptr);
static void rtc_time_update(NX_SNTP_TIME_MESSAGE *SNTP_msg, NX_SNTP_TIME *local_time);
static VOID time_update_callback(NX_SNTP_TIME_MESSAGE *time_update_ptr, NX_SNTP_TIME *local_time);

int32_t mypow10(int32_t exponent);
ULONG StrIp2Ulong(char* ipstr);
void UlongIp2Str(ULONG ipval, char* ipstr, ULONG maxlen);
UINT ipstr_cleanup(char* dst, char* src);

UINT createAndSendUDPPacket(ULONG ip_address, UINT port, char* data);
UINT Http_request(HTTP_RSC_STRUCT* resources);

//valamiért ez a fos nem látja, pedig ugyanúgy be van includolva mint a többi
extern UINT  _nxe_web_http_client_patch_secure_start(NX_WEB_HTTP_CLIENT *client_ptr, NXD_ADDRESS *server_ip, UINT server_port, CHAR *resource,
        CHAR *host, CHAR *username, CHAR *password, ULONG total_bytes,
        UINT (*tls_setup)(NX_WEB_HTTP_CLIENT *client_ptr, NX_SECURE_TLS_SESSION *),
        ULONG wait_option);
/* USER CODE END PFP */

/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

   /* USER CODE BEGIN App_NetXDuo_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_NetXDuo_MEM_POOL */
  /* USER CODE BEGIN 0 */
  printf("Nx_application started..\n\r");
  /* USER CODE END 0 */

  /* Initialize the NetXDuo system. */
  CHAR *pointer;
  nx_system_initialize();

    /* Allocate the memory for packet_pool.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_PACKET_POOL_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the Packet pool to be used for packet allocation,
   * If extra NX_PACKET are to be used the NX_APP_PACKET_POOL_SIZE should be increased
   */
  ret = nx_packet_pool_create(&NxAppPool, "NetXDuo App Pool", DEFAULT_PAYLOAD_SIZE, pointer, NX_APP_PACKET_POOL_SIZE);

  if (ret != NX_SUCCESS)
  {
    return NX_POOL_ERROR;
  }

    /* Allocate the memory for Ip_Instance */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, Nx_IP_INSTANCE_THREAD_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

   /* Create the main NX_IP instance */
  ret = nx_ip_create(&NetXDuoEthIpInstance, "NetX Ip instance", NX_APP_DEFAULT_IP_ADDRESS, NX_APP_DEFAULT_NET_MASK, &NxAppPool, nx_stm32_eth_driver,
                     pointer, Nx_IP_INSTANCE_THREAD_SIZE, NX_APP_INSTANCE_PRIORITY);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

    /* Allocate the memory for ARP */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, DEFAULT_ARP_CACHE_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Enable the ARP protocol and provide the ARP cache size for the IP instance */

  /* USER CODE BEGIN ARP_Protocol_Initialization */

  /* USER CODE END ARP_Protocol_Initialization */

  ret = nx_arp_enable(&NetXDuoEthIpInstance, (VOID *)pointer, DEFAULT_ARP_CACHE_SIZE);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Enable the ICMP */

  /* USER CODE BEGIN ICMP_Protocol_Initialization */

  /* USER CODE END ICMP_Protocol_Initialization */

  ret = nx_icmp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Enable TCP Protocol */

  /* USER CODE BEGIN TCP_Protocol_Initialization */

  /* USER CODE END TCP_Protocol_Initialization */

  ret = nx_tcp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Enable the UDP protocol required for  DHCP communication */

  /* USER CODE BEGIN UDP_Protocol_Initialization */

  /* USER CODE END UDP_Protocol_Initialization */

  ret = nx_udp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

   /* Allocate the memory for main thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the main thread */
  ret = tx_thread_create(&NxAppThread, "NetXDuo App thread", App_Main_Thread_Entry , 0, pointer, NX_APP_THREAD_STACK_SIZE,
                         NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

  if (ret != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create the DHCP client */

  /* USER CODE BEGIN DHCP_Protocol_Initialization */

  /* USER CODE END DHCP_Protocol_Initialization */

  ret = nx_dhcp_create(&DHCPClient, &NetXDuoEthIpInstance, "DHCP Client");

  if (ret != NX_SUCCESS)
  {
    return NX_DHCP_ERROR;
  }

  /* set DHCP notification callback  */
  tx_semaphore_create(&DHCPSemaphore, "DHCP Semaphore", 0);

  /* USER CODE BEGIN MX_NetXDuo_Init */
  //Allocate the app UDP thread entry pool
    ret = tx_byte_allocate(byte_pool, (VOID **) &pointer, 4 * DEFAULT_MEMORY_SIZE, TX_NO_WAIT);

    if (ret != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }
    //create the UDP server thread
    ret = tx_thread_create(&AppUDPThread, "App UDP Thread", App_UDP_Thread_Entry, 0, pointer, 4 * DEFAULT_MEMORY_SIZE,
                          DEFAULT_PRIORITY, DEFAULT_PRIORITY, TX_NO_TIME_SLICE, TX_DONT_START);

    if (ret != TX_SUCCESS)
    {
      return TX_THREAD_ERROR;
    }

  	//Allocate the memory for SNTP client thread
  	if (tx_byte_allocate(byte_pool, (VOID **) &pointer, SNTP_CLIENT_THREAD_MEMORY, TX_NO_WAIT) != TX_SUCCESS)
  	{
  	  return TX_POOL_ERROR;
  	}

  	//create the SNTP client thread
  	ret = tx_thread_create(&AppSNTPThread, "App SNTP Thread", App_SNTP_Thread_Entry, 0, pointer, SNTP_CLIENT_THREAD_MEMORY,
  						   DEFAULT_PRIORITY, DEFAULT_PRIORITY, TX_NO_TIME_SLICE, TX_DONT_START);

  	if (ret != TX_SUCCESS)
  	{
  	  return NX_NOT_ENABLED;
  	}

    //Allocate the memory for Link thread
    if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 2 *  DEFAULT_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    //create the Link thread
    ret = tx_thread_create(&AppLinkThread, "App Link Thread", App_Link_Thread_Entry, 0, pointer, 2 * DEFAULT_MEMORY_SIZE,
                           LINK_PRIORITY, LINK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    if (ret != TX_SUCCESS)
    {
      return TX_THREAD_ERROR;
    }

  	ret = tx_event_flags_create(&SntpFlags, "SNTP event flags");

  	/* Check for errors */
  	if (ret != NX_SUCCESS)
  	{
  	  return NX_NOT_ENABLED;
  	}

  	// Allocate the memory for https thread
  	if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 6* DEFAULT_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  	{
  		return TX_POOL_ERROR;
  	}
  	// create the https thread
  	ret = tx_thread_create(&AppHTTPSclientThread, "App HTTPS Thread", App_HTTPS_Thread_Entry, 0, pointer, 6 * DEFAULT_MEMORY_SIZE,
  							DEFAULT_PRIORITY, DEFAULT_PRIORITY, TX_NO_TIME_SLICE, TX_DONT_START);
  	if (ret != TX_SUCCESS)
  	{
  		return TX_THREAD_ERROR;
  	}


  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/**
* @brief  ip address change callback.
* @param ip_instance: NX_IP instance
* @param ptr: user data
* @retval none
*/
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  /* USER CODE BEGIN ip_address_change_notify_callback */
	UNUSED(ip_instance);
	UNUSED(ptr);
	tx_semaphore_put(&DHCPSemaphore);
  /* USER CODE END ip_address_change_notify_callback */
}

/**
* @brief  Main thread entry.
* @param thread_input: ULONG user argument used by the thread entry
* @retval none
*/
static VOID App_Main_Thread_Entry (ULONG thread_input)
{
  /* USER CODE BEGIN Nx_App_Thread_Entry 0 */
  UNUSED(thread_input);
  /* USER CODE END Nx_App_Thread_Entry 0 */

  UINT ret = NX_SUCCESS;

  /* USER CODE BEGIN Nx_App_Thread_Entry 1 */

  /* USER CODE END Nx_App_Thread_Entry 1 */

  /* register the IP address change callback */
  ret = nx_ip_address_change_notify(&NetXDuoEthIpInstance, ip_address_change_notify_callback, NULL);
  if (ret != NX_SUCCESS)
  {
    /* USER CODE BEGIN IP address change callback error */

    /* USER CODE END IP address change callback error */
  }

  /* start the DHCP client */
  ret = nx_dhcp_start(&DHCPClient);
  if (ret != NX_SUCCESS)
  {
    /* USER CODE BEGIN DHCP client start error */

    /* USER CODE END DHCP client start error */
  }

  /* wait until an IP address is ready */
  if(tx_semaphore_get(&DHCPSemaphore, TX_WAIT_FOREVER) != TX_SUCCESS)
  {
    /* USER CODE BEGIN DHCPSemaphore get error */
	Error_Handler();
    /* USER CODE END DHCPSemaphore get error */
  }

  /* USER CODE BEGIN Nx_App_Thread_Entry 2 */
  //get IP address
    ret = nx_ip_address_get(&NetXDuoEthIpInstance, &IpAddress, &NetMask);

    //print the IP address
    PRINT_IP_ADDRESS(IpAddress);

    if (ret != TX_SUCCESS)
    {
      Error_Handler();
    }

    //Now the network is correctly initialized, start the UDP server thread
    tx_thread_resume(&AppUDPThread);

	//the network is correctly initialized, start the TCP server thread
	tx_thread_resume(&AppSNTPThread);

	//the network is correctly initialized, start the HTTPS client thread
	tx_thread_resume(&AppHTTPSclientThread);

	//start usart thread
	tx_thread_resume(&AppUsartThread);

    //this thread is not needed any more, we relinquish it
    tx_thread_relinquish();
  /* USER CODE END Nx_App_Thread_Entry 2 */

}
/* USER CODE BEGIN 1 */
//threads////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* @brief  SNTP thread entry.
* @param thread_input: ULONG user argument used by the thread entry
* @retval none
*/
static void App_SNTP_Thread_Entry(ULONG info)
{
  UINT ret;
  RtcHandle.Instance = RTC;
  ULONG  seconds;
  ULONG fraction;
  ULONG  events = 0;
  UINT   server_status;
  NXD_ADDRESS sntp_server_ip;
  NX_PARAMETER_NOT_USED(info);

  sntp_server_ip.nxd_ip_version = 4;

  //Create a DNS client
  ret = dns_create(&DnsClient);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  //Look up SNTP Server address
  ret = nx_dns_host_by_name_get(&DnsClient, (UCHAR *)SNTP_SERVER_NAME,
                                &sntp_server_ip.nxd_ip_address.v4, NX_APP_DEFAULT_TIMEOUT);
  //Check for error
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  //Create the SNTP Client
  ret =  nx_sntp_client_create(&SntpClient, &NetXDuoEthIpInstance, iface_index, &NxAppPool, NULL, kiss_of_death_handler, NULL);

  //Check for error
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  //Setup time update callback function
   nx_sntp_client_set_time_update_notify(&SntpClient, time_update_callback);

  //Use the IPv4 service to set up the Client and set the IPv4 SNTP server
  ret = nx_sntp_client_initialize_unicast(&SntpClient, sntp_server_ip.nxd_ip_address.v4);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  //Run whichever service the client is configured for
  ret = nx_sntp_client_run_unicast(&SntpClient);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }
  else
  {
    PRINT_CNX_SUCC();
  }

  //Wait for a server update event
  tx_event_flags_get(&SntpFlags, SNTP_UPDATE_EVENT, TX_OR_CLEAR, &events, PERIODIC_CHECK_INTERVAL);

  if (events == SNTP_UPDATE_EVENT)
  {
    //Check for valid SNTP server status
    while((ret != NX_SUCCESS) || (server_status == NX_FALSE))//loop until we do not have a valid update
    {
    	ret = nx_sntp_client_receiving_updates(&SntpClient, &server_status);
    }

    //We have a valid update.  Get the SNTP Client time
    ret = nx_sntp_client_get_local_time_extended(&SntpClient, &seconds, &fraction, NX_NULL, 0);

    ret = nx_sntp_client_utility_display_date_time(&SntpClient,buffer,64);

    if (ret != NX_SUCCESS)
    {
      printf("Internal error with getting local time 0x%x\n\r", ret);
      Error_Handler();
    }
    else
    {
      time_update_after_boot_flag = 1;
      printf("\nSNTP update :\n\r");
      printf("%s\n\n\r",buffer);

    }
  }
  else
  {
    Error_Handler();
  }

  //Set Current time from SNTP TO RTC
  rtc_time_update(&SntpClient.nx_sntp_current_server_time_message, NULL);


  //TODO set first closest alarm



  // Enable the Alarm B interrupt
  SET_BIT(RTC->CR, RTC_CR_ALRBE | RTC_CR_ALRBIE);

  /*
  // We can stop the SNTP service if for example we think the SNTP server has stopped sending updates
  ret = nx_sntp_client_stop(&SntpClient);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  // When done with the SNTP Client, we delete it
  ret = nx_sntp_client_delete(&SntpClient);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }
  */
  // time is printed in RTC interrupt

  // this thread is not needed any more, we relinquish it
  tx_thread_relinquish();

}

static VOID App_UDP_Thread_Entry(ULONG thread_input)
{
  UINT ret;
  ULONG bytes_read;
  UINT source_port;

  UCHAR data_buffer[UDP_DATA_LEN];
  ULONG source_ip_address;
  NX_PACKET *data_packet;

  RTC_DateTypeDef RTC_Date = {0};
  RTC_TimeTypeDef RTC_Time = {0};
  char tmps[30]={0};
  time_t tmp_ts;

  UNUSED(thread_input);

  HAdata.time_update_after_boot_timestamp = NULL;

  //create the UDP socket
  ret = nx_udp_socket_create(&NetXDuoEthIpInstance, &UDPSocket, "UDP Server Socket", NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, QUEUE_MAX_SIZE);

  if (ret != NX_SUCCESS)
  {
     Error_Handler();
  }

  //bind the socket indefinitely on the required port
  ret = nx_udp_socket_bind(&UDPSocket, DEFAULT_PORT, TX_WAIT_FOREVER);

  if (ret != NX_SUCCESS)
  {
     Error_Handler();
  }
  else
  {
    printf("UDP Server listening on PORT %d.. \n\r", DEFAULT_PORT);
  }

  while(1)
  {
    TX_MEMSET(data_buffer, '\0', sizeof(data_buffer));
    ret = nx_udp_socket_receive(&UDPSocket, &data_packet, 100);//wait for data for 1 sec

    //get time  date data from rtc
	HAL_RTC_GetTime(&RtcHandle,&RTC_Time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RtcHandle,&RTC_Date,RTC_FORMAT_BIN);
	//store time data in different ways
	tmp_ts =  get_local_rtc_time_date(&RTC_Date, &RTC_Time, tmps);

	if(time_update_after_boot_flag == 1)
	{
		time_update_after_boot_flag = 0;
		if(HAdata.time_update_after_boot_timestamp != NULL)//can't happen because after boot it should be empty but bab
		{
			free(HAdata.time_update_after_boot_timestamp);
			HAdata.time_update_after_boot_timestamp = NULL;
		}
		HAdata.time_update_after_boot_timestamp = StrAllocAndCpy(tmps);
	}

    if (ret == NX_SUCCESS)
    {
		nx_packet_data_retrieve(data_packet, data_buffer, &bytes_read);//data is available, read it into the data buffer
		nx_udp_source_extract(data_packet, &source_ip_address, &source_port);//get info about the client address and port
		PRINT_DATA(source_ip_address, source_port, data_buffer);//print the client address, the remote port and the received data

		HAdata.last_action_timestamp = tmp_ts;

		if(strcmp((char*)data_buffer, HA_SECR_STR1) == 0)
		{
			char txstr[150]={0};
			volatile uint32_t* outtemp_p = NULL;

			N_MasterReadFirstRelevantNodeData(N_OUTSIDE_TEMPERATURE_SENSOR, &outtemp_p, &node_capabilities_pp, &node_data_pp);
			/*union{float f; uint32_t u;}cnv;
			if(outtemp_p != NULL)	{ cnv.u = (*outtemp_p);}
								else{ cnv.u = 0;}//error*/
			sprintf(txstr,"INFO_Server datetime:\t\t%s\nServer temp:\t\t%.2f°C\nBoot datetime:\t\t%s",tmps, HAdata.temperature_server, HAdata.time_update_after_boot_timestamp!=0?HAdata.time_update_after_boot_timestamp:"no_ts");

			ret = createAndSendUDPPacket(source_ip_address, source_port, txstr);
		}

		if(strcmp((char*)data_buffer, HA_SECR_STR2) == 0)
		{
			N_WriteEveryRelevantNode(N_INDICATOR_LED, 1U, &node_capabilities_pp, &node_data_pp);

		}
		else if(strcmp((char*)data_buffer, HA_SECR_STR3) == 0)
		{
			N_WriteEveryRelevantNode(N_INDICATOR_LED, 0U, &node_capabilities_pp, &node_data_pp);
		}

		if(strncmp((char*)data_buffer, HA_SECR_STR4, HA_SECR_STR4_LEN) == 0)
		{
			union
			{
				uint32_t u;
				uint32_t i;
			}cnv;

			cnv.i = atoi((const char*)data_buffer+HA_SECR_STR4_LEN);
			N_WriteEveryRelevantNode(N_EAST_SHADER, cnv.u, &node_capabilities_pp, &node_data_pp);
		}

		//resend the same packet to the client
		//ret = nx_udp_socket_send(&UDPSocket, data_packet, source_ip_address, source_port);
		//if we are not resending the packet, we have to release it manually
		ret = nx_packet_release(data_packet);

		//set the yellow led to monitor visually the traffic, RTC interrupt will reset it
		HAL_GPIO_WritePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin, GPIO_PIN_SET);
    }
    else
    {
#ifdef DEBUG
        //the server is in idle state, toggle the green led
        HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
#endif
    }
  }
}

/*
* @brief  Link thread entry
* @param thread_input: ULONG thread parameter
* @retval none
*/
static VOID App_Link_Thread_Entry(ULONG thread_input)
{
  ULONG actual_status;
  UINT linkdown = 0;
  UINT status;

  UNUSED(thread_input);

  while(1)
  {
    //Get Physical Link status.
    status = nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0, NX_IP_LINK_ENABLED, &actual_status, 10);

    if(status == NX_SUCCESS)
    {
      if(linkdown == 1)
      {
        linkdown = 0;
        status = nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0, NX_IP_ADDRESS_RESOLVED, &actual_status, 10);
        if(status == NX_SUCCESS)
        {
          printf("The network cable is connected again.\n\r");
          printf("UDP Echo Server is available again.\n\r");
        }
        else
        {
          printf("The network cable is connected.\n\r");
          //Send command to Enable Nx driver
          nx_ip_driver_direct_command(&NetXDuoEthIpInstance, NX_LINK_ENABLE, &actual_status);
          //Restart DHCP Client
          nx_dhcp_stop(&DHCPClient);
          nx_dhcp_start(&DHCPClient);
        }
      }
    }
    else
    {
      if(0 == linkdown)
      {
        linkdown = 1;
        //The network cable is not connected
        printf("The network cable is not connected.\n\r");
      }
    }

    tx_thread_sleep(NX_ETH_CABLE_CONNECTION_CHECK_PERIOD);
  }
}

static VOID App_HTTPS_Thread_Entry(ULONG thread_input)
{
	UNUSED(thread_input);
	UINT status = NX_SUCCESS;
	ULONG StoredIpAddress = 0;
	char* public_ip_str;
	RTC_DateTypeDef RTC_Date = {0};
	RTC_TimeTypeDef RTC_Time = {0};
	char tmps[30]={0};

	tx_semaphore_create(&HTTPSSemaphore, "HTTPS_semaphore", 0);

	while(1)
	{
		//suspend thread and wait for semaphore put
		tx_semaphore_get(&HTTPSSemaphore, NX_WAIT_FOREVER);


		//get time  date data from rtc
		HAL_RTC_GetTime(&RtcHandle,&RTC_Time,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&RtcHandle,&RTC_Date,RTC_FORMAT_BIN);
		get_local_rtc_time_date(&RTC_Date, &RTC_Time, tmps);


		//get public IP
		{
			//init
			HTTP_RSC_STRUCT input_data;
			input_data.server_ip_address.nxd_ip_version = NX_IP_VERSION_V4;
			input_data.hostURL = StrAllocAndCpy("ipv4.icanhazip.com");
			if(input_data.hostURL == NULL)	{ Error_Handler();}
			input_data.resource = StrAllocAndCpy(" ");
			if(input_data.resource == NULL)	{ Error_Handler();}
			input_data.method = NX_WEB_HTTP_METHOD_GET;

			//get
			status = Http_request(&input_data);
			if(status != NX_SUCCESS)	{ printf("icanhazip.com request not successful, status:%i\n\r", status);}

			//process
			if(input_data.return_data_p != NULL)
			{
				public_ip_str = calloc(30, 1);
				if(public_ip_str == NULL)
				{ Error_Handler();}

				ipstr_cleanup(public_ip_str, input_data.return_data_p);
				PublicIpAddress = StrIp2Ulong(public_ip_str);
				free(public_ip_str);
				public_ip_str = NULL;
			}

			//cleanup
			free(input_data.hostURL);
			input_data.hostURL = NULL;
			free(input_data.resource);
			input_data.resource = NULL;
			if(input_data.return_data_p != NULL)
			{
				free(input_data.return_data_p);
				input_data.return_data_p = NULL;
			}
		}

		//get stored IP from firebase
		{
			//init
			HTTP_RSC_STRUCT input_data;
			input_data.server_ip_address.nxd_ip_version = NX_IP_VERSION_V4;
			input_data.hostURL = StrAllocAndCpy("firestore.googleapis.com");
			if(input_data.hostURL == NULL)	{ Error_Handler();}
			input_data.resource = StrAllocAndCpy(HA_SECR_STR5);
			if(input_data.resource == NULL)	{ Error_Handler();}
			input_data.method = NX_WEB_HTTP_METHOD_GET;

			//get
			status = Http_request(&input_data);
			if(status != NX_SUCCESS)
			{
				printf("firebase get request not successful, status:%i\n\r", status);
			}

			//process
			if(input_data.return_data_p != NULL)
			{
				//cut out the ip string from the response
				char* ip_part_location = strstr(input_data.return_data_p, "stringValue");
				if( (ip_part_location != NULL) && (ip_part_location != input_data.return_data_p) )
				{
					char* stored_ip_str = calloc(30, 1);
					char* tmps = calloc(20, 1);

					if(stored_ip_str == NULL)	{ Error_Handler();}
					if(tmps == NULL)	{ Error_Handler();}

					strncpy(tmps, ip_part_location+15, 16);
					ipstr_cleanup(stored_ip_str, tmps);
					StoredIpAddress = StrIp2Ulong(stored_ip_str);

					free(stored_ip_str);
					stored_ip_str = NULL;
					free(tmps);
					tmps = NULL;
				}
				else
				{
					printf("cannot find ip in response\n\r");
					StoredIpAddress = 0;
				}
			}
			else
			{
				StoredIpAddress = 0;
			}

			//cleanup
			free(input_data.hostURL);
			input_data.hostURL = NULL;
			free(input_data.resource);
			input_data.resource = NULL;
			if(input_data.return_data_p != NULL)
			{
				free(input_data.return_data_p);
				input_data.return_data_p = NULL;
			}
		}

		//compare public ip and stored ip and if they don't match, call a patch req to firebase
		if(((StoredIpAddress^IP_OBFUSCATION_VAL) != PublicIpAddress) && (StoredIpAddress != 0) )
		{
			//init
			char* patch_ip_str = calloc(21,1);
			if(patch_ip_str == NULL)	{ Error_Handler();}
			UlongIp2Str(PublicIpAddress^IP_OBFUSCATION_VAL, patch_ip_str, 20);

			HTTP_RSC_STRUCT input_data;
			input_data.server_ip_address.nxd_ip_version = NX_IP_VERSION_V4;
			input_data.hostURL = StrAllocAndCpy("firestore.googleapis.com");
			if(input_data.hostURL == NULL)	{ Error_Handler();}
			input_data.resource = StrAllocAndCpy(HA_SECR_STR6);
			if(input_data.resource == NULL)	{ Error_Handler();}
			ULONG part1 = strlen(HA_SECR_STR7);
			ULONG part2 = strlen(patch_ip_str);
			ULONG part3 = (strlen(HA_SECR_STR8) + 1);
			input_data.resource_content = calloc(part1+part2+part3, 1);
			if(input_data.resource_content == NULL)	{ Error_Handler();}
			strcpy(input_data.resource_content, HA_SECR_STR7);
			strcpy(input_data.resource_content+part1, patch_ip_str);
			strcpy(input_data.resource_content+part1+part2, HA_SECR_STR8);
			input_data.method = NX_WEB_HTTP_METHOD_PATCH;

			//patch
			status = Http_request(&input_data);
			if(status != NX_SUCCESS)	{ printf("firebase patch request not successful, status:%i\n\r", status);}
			else
			{
				printf("patch content:%s\n\r", input_data.resource_content);
				printf("patch status:%i\n\r", status);
			}

			//process
			//no response expected from patch request

			//cleanup
			free(patch_ip_str);
			patch_ip_str = NULL;
			free(input_data.hostURL);
			input_data.hostURL = NULL;
			free(input_data.resource);
			input_data.resource = NULL;
			free(input_data.resource_content);
			input_data.resource_content = NULL;
			if(input_data.return_data_p != NULL)
			{
				free(input_data.return_data_p);
				input_data.return_data_p = NULL;
			}
		}

		{//get fdr info if changed //see comment is secr
			uint32_t oldval = 0;
			volatile uint32_t* newval = NULL;

			N_MasterGetFirstRelevantNodeData(N_SECR_FDPS, &oldval, &node_capabilities_pp, &node_data_pp);
			N_MasterReadFirstRelevantNodeData(N_SECR_FDPS, &newval, &node_capabilities_pp, &node_data_pp);

			if(oldval != (*newval))
			{
				char state_str[8] = {0};
				if((*newval) == 0)	{ strcpy(state_str, HA_SECR_STR13);}
				else{ strcpy(state_str, HA_SECR_STR14);}

				//init
				HTTP_RSC_STRUCT input_data;
				input_data.server_ip_address.nxd_ip_version = NX_IP_VERSION_V4;
				input_data.hostURL = StrAllocAndCpy("firestore.googleapis.com");
				if(input_data.hostURL == NULL)	{ Error_Handler();}
				input_data.resource = StrAllocAndCpy(HA_SECR_STR10);
				if(input_data.resource == NULL)	{ Error_Handler();}
				ULONG part1 = strlen(HA_SECR_STR11);
				ULONG part2 = strlen(tmps);
				ULONG part3 = strlen(HA_SECR_STR12);
				ULONG part4 = strlen(state_str);
				ULONG part5 = (strlen(HA_SECR_STR8) + 1);

				input_data.resource_content = calloc(part1+part2+part3+part4+part5, 1);
				if(input_data.resource_content == NULL)	{ Error_Handler();}
				strcpy(input_data.resource_content, HA_SECR_STR11);
				strcpy(input_data.resource_content+part1, tmps);
				strcpy(input_data.resource_content+part1+part2, HA_SECR_STR12);
				strcpy(input_data.resource_content+part1+part2+part3, state_str);
				strcpy(input_data.resource_content+part1+part2+part3+part4, HA_SECR_STR8);

				input_data.method = NX_WEB_HTTP_METHOD_PATCH;

				//patch
				status = Http_request(&input_data);
				if(status != NX_SUCCESS)	{ printf("firebase patch request not successful, status:%i\n\r", status);}
				else
				{
					printf("patch content:%s\n\r", input_data.resource_content);
					printf("patch status:%i\n\r", status);
				}

				//process
				//no response expected from patch request

				//cleanup
				free(input_data.hostURL);
				input_data.hostURL = NULL;
				free(input_data.resource);
				input_data.resource = NULL;
				free(input_data.resource_content);
				input_data.resource_content = NULL;
				if(input_data.return_data_p != NULL)
				{
					free(input_data.return_data_p);
					input_data.return_data_p = NULL;
				}
			}
		}


	}
}

//functions////////////////////////////////////////////////////////////////////////////////////////////

UINT ipstr_cleanup(char* dst, char* src)
{
	ULONG indx_d = 0;
	ULONG indx_s = 0;

	if( (dst != NULL) && (src != NULL) )
	{
		while(1)
		{
			if( (src[indx_s]<'0') || (src[indx_s]>'9') )//we have decimal numbers in the string separated with '.' so it stops when finds a number
			{
				indx_s++;
			}
			else
			{
				break;
			}
		}
		while(1)
		{
			if( ((src[indx_s]<'0') || (src[indx_s]>'9')) && (src[indx_s]!='.') )
			{
				return 0;
			}
			else
			{
				dst[indx_d] = src[indx_s];
				indx_d++;
				indx_s++;
			}
		}
	}
	else
	{
		return -1;
	}
}

void UlongIp2Str(ULONG ipval, char* ipstr, ULONG maxlen)
{
	uint8_t b1 = ((ipval>>24)&0xff);
	uint8_t b2 = ((ipval>>16)&0xff);
	uint8_t b3 = ((ipval>>8)&0xff);
	uint8_t b4 = ((ipval>>0)&0xff);

	snprintf(ipstr, maxlen, "%u.%u.%u.%u", b1, b2, b3, b4);
}

ULONG StrIp2Ulong(char* ipstr)
{
	uint8_t byte = 0;//which byte is processed currently
	uint8_t indx1 = 0;//start index of represented byte
	uint8_t indx2 = 0;//end index of represented byte
	uint8_t numlen = 0;//number of digits in the number
	uint8_t num = 0;//digit*place value
    uint8_t num2 = 0;//result byte value
    ULONG ip = 0;//assembled return value

	while(byte<4)
	{
		while(1)
		{
		    if((ipstr[indx2] == '.') || (ipstr[indx2] == 0) || (ipstr[indx2] == '\n'))    {break;}
			indx2++;
		}
		numlen = (indx2-indx1);

        num2 = 0;
		for(uint8_t tmp=0; tmp<numlen; tmp++)
		{
			num = (ipstr[indx1+tmp]-'0')*mypow10(numlen-1-tmp);
			num2+=num;
		}

		ip |= (num2 << ((3-byte)*8));

		byte++;
		indx2++;
		indx1 = indx2;
	}
	return ip;
}

int32_t mypow10(int32_t exponent)
{
	uint32_t num = 10;

	switch(exponent)
	{
		case 0: num = 1;
				break;

		case 1: //num = 10;
				break;

		default:	exponent--;
					while(exponent)
					{
						num *= 10;
						exponent--;
					}
					break;
	}
	return num;
}

/*
 * Before call fill the input struct with paramteres: hostURL, resource, method
 * After return, free the return_data_p in the struct
 */
UINT Http_request(HTTP_RSC_STRUCT* resources)
{
	UINT status = NX_SUCCESS;
	ULONG bytes;

	resources->return_data_p = NULL;
	resources->return_data_len = 0;

	//Look up Server address
	status = nx_dns_host_by_name_get(&DnsClient, (UCHAR*)(resources->hostURL), &(resources->server_ip_address.nxd_ip_address.v4), NX_IP_PERIODIC_RATE);
	//Check status.
	if(status != NX_SUCCESS)
	{ return status;}

	//Create an HTTP client instance.
	status = _nxe_web_http_client_create(&http_client, "HTTP Client", &NetXDuoEthIpInstance, &NxAppPool, 1536, sizeof(http_client));

#ifdef DEBUG
	//Set the header callback routine.
	//_nxe_web_http_client_response_header_callback_set(&http_client, http_response_callback);
#endif

	NX_PACKET* resource_content_packet;

	switch(resources->method)
	{
		case NX_WEB_HTTP_METHOD_GET:	status =  _nxe_web_http_client_get_secure_start(&http_client, &(resources->server_ip_address), NX_WEB_HTTPS_SERVER_PORT,
																						resources->resource, resources->hostURL,
																						NULL, NULL, tls_setup_callback, NX_WAIT_FOREVER);
										break;

		case NX_WEB_HTTP_METHOD_PATCH:	status =  _nxe_web_http_client_patch_secure_start(&http_client, &(resources->server_ip_address), NX_WEB_HTTPS_SERVER_PORT,
																						resources->resource, resources->hostURL,
																						NULL, NULL, (ULONG)strlen(resources->resource_content),
																						tls_setup_callback, NX_WAIT_FOREVER);
										status = _nxe_web_http_client_request_packet_allocate(&http_client, &resource_content_packet, NX_WAIT_FOREVER);
										status = nx_packet_data_append(resource_content_packet, resources->resource_content, strlen(resources->resource_content), &NxAppPool, NX_WAIT_FOREVER);
										status = _nxe_web_http_client_put_packet(&http_client, resource_content_packet, NX_WAIT_FOREVER);
										break;

		case NX_WEB_HTTP_METHOD_PUT:	break;
		case NX_WEB_HTTP_METHOD_POST:	break;
		case NX_WEB_HTTP_METHOD_DELETE:	break;
		case NX_WEB_HTTP_METHOD_HEAD:	break;
		case NX_WEB_HTTP_METHOD_NONE:	break;
		default:						break;
	}

	//Check status.
	if(status != NX_SUCCESS)
	{
		return status;
	}

	//put and patch normally does not give response
	if(resources->method == NX_WEB_HTTP_METHOD_GET)
	{
		UINT get_status = NX_SUCCESS;
		NX_PACKET *receive_packet = NULL;
		CHAR *tmp_p = NULL;
		UCHAR receive_buffer[NX_MAX_STRING_LENGTH];

		while(get_status != NX_WEB_HTTP_GET_DONE)
		{
			(void)memset(receive_buffer, 0x00, NX_MAX_STRING_LENGTH);
			get_status = _nxe_web_http_client_response_body_get(&http_client, &receive_packet, NX_WAIT_FOREVER);

			if((get_status == NX_SUCCESS) || (get_status == NX_WEB_HTTP_GET_DONE))
			{
				status = nx_packet_data_extract_offset(receive_packet, 0, receive_buffer, NX_MAX_STRING_LENGTH, &bytes);
				if(status)	{ Error_Handler();}

				//make sure we have a null terminator
				if(bytes < NX_MAX_STRING_LENGTH)	{ receive_buffer[bytes] = 0;}//redundant because of the previous memset()
				else{ receive_buffer[NX_MAX_STRING_LENGTH-1] = 0;}//theoretically can't happen but bab

				if(bytes>0)//if packet data part was somehow empty then do nothing
				{
					//mem juggling
					if(resources->return_data_p == NULL)//first entry
					{
						resources->return_data_p = (CHAR*)calloc((size_t)bytes, 1);
						//now resources->return_data_len is still 0, so the memcpy() will be ok
					}
					else
					{
						tmp_p = resources->return_data_p;//transfer pointer
						resources->return_data_p = (CHAR*)realloc(tmp_p, (size_t)resources->return_data_len + bytes);//get bigger buff, also frees tmp_p
					}
					if(resources->return_data_p == NULL)	{ Error_Handler();}//alloc was not successful
					(void)memcpy(resources->return_data_p + resources->return_data_len, receive_buffer, bytes);//copy the new data
					resources->return_data_len += bytes;//set new len
				}
			}
			else{ Error_Handler();}

			if(receive_packet != NULL)
			{
				status = nx_packet_release(receive_packet);
				receive_packet = NULL;
			}
		}
	}

	(void)_nxe_web_http_client_delete(&http_client);// Clear out the HTTP client when we are done.

	return NX_SUCCESS;
}

/*
 * Callback invoked when HTTP(S) client processes a response header.
 * This example shows how the HTTP headers are extracted.
 */
VOID http_response_callback(NX_WEB_HTTP_CLIENT *client_ptr, CHAR *field_name, UINT field_name_length,
                            CHAR *field_value, UINT field_value_length)
{
	CHAR name[100];
	CHAR value[100];

    NX_PARAMETER_NOT_USED(client_ptr);

    if(field_name_length >= sizeof(name) || field_value_length >= sizeof(value))
    {
        return;
    }

    memset(name, 0, sizeof(name));
    memset(value, 0, sizeof(value));

    memcpy(name, field_name, field_name_length);//Use case of memcpy is verified.
    memcpy(value, field_value, field_value_length);//Use case of memcpy is verified.

    printf("Received https header: \n\rField name: %s (%d bytes)\n\rValue: %s (%d bytes)\n\r--------------\n\r", name, field_name_length, value, field_value_length);
}

/*
 * Callback to setup TLS parameters for secure HTTPS.
 */
UINT tls_setup_callback(NX_WEB_HTTP_CLIENT *client_ptr, NX_SECURE_TLS_SESSION *tls_session)
{
	UINT status;

    NX_PARAMETER_NOT_USED(client_ptr);

    //Initialize and create TLS session
    status = nx_secure_tls_session_create(tls_session, &nx_crypto_tls_ciphers, crypto_metadata_client, sizeof(crypto_metadata_client));

    //Check status
    if(status != NX_SUCCESS)
    {
        return(status);
    }

    //Allocate space for packet reassembly
    status = nx_secure_tls_session_packet_buffer_set(tls_session, tls_packet_buffer, sizeof(tls_packet_buffer));

    //Check status
    if(status != NX_SUCCESS)
    {
        return(status);
    }

    //Add a CA Certificate to our trusted store for verifying incoming server certificates
    nx_secure_x509_certificate_initialize(&trusted_certificate, ca_cert_der, (USHORT)ca_cert_der_len, NX_NULL, 0, NULL, 0, NX_SECURE_X509_KEY_TYPE_NONE);
    nx_secure_tls_trusted_certificate_add(tls_session, &trusted_certificate);

    //Need to allocate space for the certificate coming in from the remote host
    nx_secure_tls_remote_certificate_allocate(tls_session, &remote_certificate, remote_cert_buffer, sizeof(remote_cert_buffer));
    nx_secure_tls_remote_certificate_allocate(tls_session, &remote_issuer, remote_issuer_buffer, sizeof(remote_issuer_buffer));

    return(NX_SUCCESS);
}

UINT createAndSendUDPPacket(ULONG ip_address, UINT port, char* data)
{
	UINT ret = 0;
	char* data_buffer = calloc(UDP_DATA_LEN, sizeof(char));

	NX_PACKET *data_packet;

	if(strlen(data) > (UDP_DATA_LEN-1))
	{
		ret = NX_SIZE_ERROR;
		return ret;
	}
	else
	{
		strcpy(data_buffer, data);
	}

	//create the packet to send over the UDP socket
	ret = nx_packet_allocate(&NxAppPool, &data_packet, NX_UDP_PACKET, TX_WAIT_FOREVER);
	if (ret != NX_SUCCESS)	{ Error_Handler();}

	ret = nx_packet_data_append(data_packet, data_buffer, UDP_DATA_LEN, &NxAppPool, TX_WAIT_FOREVER);
	if (ret != NX_SUCCESS)	{ Error_Handler();}

	//send the message
	ret = nx_udp_socket_send(&UDPSocket, data_packet, ip_address, port);

	free(data_buffer);
	return ret;
}


/*
* @brief  DNS Create Function.
* @param dns_ptr
* @retval ret
*/
UINT dns_create(NX_DNS *dns_ptr)
{
	UINT ret = NX_SUCCESS;

	// Create a DNS instance for the Client
	ret = nx_dns_create(dns_ptr, &NetXDuoEthIpInstance, (UCHAR *)"DNS Client");
	if (ret != NX_SUCCESS)	{ Error_Handler();}

	// Initialize DNS instance with the DNS server Address
	ret = nx_dns_server_add(dns_ptr, USER_DNS_ADDRESS);
	if (ret != NX_SUCCESS)	{ Error_Handler();}

	ret = nx_dns_server_add(dns_ptr, IP_ADDRESS(1,0,0,1));//cloudflare
	if (ret != NX_SUCCESS)	{ Error_Handler();}

	ret = nx_dns_server_add(dns_ptr, IP_ADDRESS(8,8,8,8));//google
	if (ret != NX_SUCCESS)	{ Error_Handler();}

	ret = nx_dns_server_add(dns_ptr, IP_ADDRESS(192,168,100,1));//my router
	if (ret != NX_SUCCESS)	{ Error_Handler();}

	return ret;
}

/*This application defined handler for handling a Kiss of Death packet is not
 * required by the SNTP Client. A KOD handler should determine
 * if the Client task should continue vs. abort sending/receiving time data
 * from its current time server, and if aborting if it should remove
 * the server from its active server list.
 *
 * Note that the KOD list of codes is subject to change. The list
 * below is current at the time of this software release.
*/
static UINT kiss_of_death_handler(NX_SNTP_CLIENT *client_ptr, UINT KOD_code)
{
  UINT    remove_server_from_list = NX_FALSE;
  UINT    status = NX_SUCCESS;

  NX_PARAMETER_NOT_USED(client_ptr);

  //Handle kiss of death by code group
  switch (KOD_code)
  {
	  case NX_SNTP_KOD_RATE:
	  case NX_SNTP_KOD_NOT_INIT:
	  case NX_SNTP_KOD_STEP:		//Find another server while this one is temporarily out of service
									status =  NX_SNTP_KOD_SERVER_NOT_AVAILABLE;
									break;

	  case NX_SNTP_KOD_AUTH_FAIL:
	  case NX_SNTP_KOD_NO_KEY:
	  case NX_SNTP_KOD_CRYP_FAIL:	//These indicate the server will not service client with time updates without successful authentication
									remove_server_from_list =  NX_TRUE;
									break;

	  default:	//All other codes. Remove server before resuming time updates
				remove_server_from_list =  NX_TRUE;
				break;
  }

  //Removing the server from the active server list?
  if (remove_server_from_list)
  {

    //Let the caller know it has to bail on this server before resuming service
    status = NX_SNTP_KOD_REMOVE_SERVER;
  }

  return status;
}

/*
 * This application defined handler for notifying SNTP time update event.
 */
static VOID time_update_callback(NX_SNTP_TIME_MESSAGE *time_update_ptr, NX_SNTP_TIME *local_time)
{
  rtc_time_update(time_update_ptr, local_time);

  printf("Local time updated from SNTP server\n\r");

  tx_event_flags_set(&SntpFlags, SNTP_UPDATE_EVENT, TX_OR);
}

/*
 * This application updates Time from SNTP to STM32 RTC
 */
static void rtc_time_update(NX_SNTP_TIME_MESSAGE *SNTP_msg, NX_SNTP_TIME *local_time)
{
	RTC_DateTypeDef sdatestructure = {0};
	RTC_TimeTypeDef stimestructure = {0};
	struct tm ts;
	CHAR  temp[32] = {0};
	time_t timestamp = 0;

	if(local_time == NULL)//not calling from time update callback
	{
		/* convert SNTP time (seconds since 01-01-1900 to 01-01-1970)
		EPOCH_TIME_DIFF is equivalent to 70 years in sec
		calculated with www.epochconverter.com/date-difference
		This constant is used to delete difference between :
		Epoch converter (referenced to 1970) and SNTP (referenced to 1900) */
		timestamp = SNTP_msg->receive_time.seconds - EPOCH_TIME_DIFF;
	}
	else //calling from time update callback
	{
		//Time difference between Device A(client) and Device B(server): Offset = ((T2 – T1) + (T3 – T4)) / 2
		// T1 originate_time_stamp
		// T2 receive_time_stamp
		// T3 transmit_time_stamp
		// T4 local_time
		//time_t offset = ((SNTP_msg->receive_time_stamp[0] - SNTP_msg->originate_time_stamp[0]) + (SNTP_msg->transmit_time_stamp[0] - local_time->seconds)) / 2;
		timestamp = local_time->seconds;// - offset;
		timestamp -= EPOCH_TIME_DIFF;
	}


	//convert time in yy/mm/dd hh:mm:sec
	ts = *localtime(&timestamp);

	//convert date composants to hex format
	sprintf(temp, "%d", (ts.tm_year - 100));
	sdatestructure.Year = strtol(temp, NULL, 16);
	sprintf(temp, "%d", ts.tm_mon + 1);
	sdatestructure.Month = strtol(temp, NULL, 16);
	sprintf(temp, "%d", ts.tm_mday);
	sdatestructure.Date = strtol(temp, NULL, 16);
	/* dummy weekday */
	sdatestructure.WeekDay =0x00;


	//convert time composants to hex format
	sprintf(temp,"%d", ts.tm_hour);
	stimestructure.Hours = strtol(temp, NULL, 16);
	sprintf(temp,"%d", ts.tm_min);
	stimestructure.Minutes = strtol(temp, NULL, 16);
	sprintf(temp, "%d", ts.tm_sec);
	stimestructure.Seconds = strtol(temp, NULL, 16);

	if(sdatestructure.Year != 0)
	{
		if (HAL_RTC_SetDate(&RtcHandle, &sdatestructure, RTC_FORMAT_BCD) != HAL_OK)
		{
			Error_Handler();
		}

		if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
		{
			Error_Handler();
		}

		nx_sntp_client_set_local_time(&SntpClient, SNTP_msg->receive_time.seconds, SNTP_msg->receive_time.fraction);
	}
}
/* USER CODE END 1 */
