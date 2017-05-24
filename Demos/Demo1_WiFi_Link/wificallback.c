#include "wificallback.h"

/* ========================================
	User provide callback functions 
    ======================================== */
void system_version(char *str, int len)
{
  snprintf( str, len, "%s", APP_INFO);
}
		
void userWatchDog(void)
{
} 

void easylink_user_data_result(int datalen, char* data)
{
   net_para_st para;
   getNetPara(&para, Station);
   if(!datalen){
    printf("No user input. %s\r\n", data);
   }
   else{
    printf("User input is %s\r\n", data);
   }
} 
//callback while easylink is opened successly
void RptConfigmodeRslt(network_InitTypeDef_st *nwkpara)
{
	if(nwkpara == NULL){
		printf("Configuration failed\r\n");
	}
	else{
		configSuccess = 1;
		easylink = 0;
		memcpy(&wNetConfig, nwkpara, sizeof(network_InitTypeDef_st));
		printf("Configuration is successful, SSID:%s, Key:%s\r\n", \
																		wNetConfig.wifi_ssid,\
																		wNetConfig.wifi_key);
	}
}
//callback function of mxchipTick, it will be called by mxchipTick() when the status of Net change.
void WifiStatusHandler(int event)
{
  switch (event) {
    case MXCHIP_WIFI_UP:
      printf("Station up \r\n");
	  linkSuccess = 1;
      break;
    case MXCHIP_WIFI_DOWN:
      printf("Station down \r\n");
      break;
    case MXCHIP_UAP_UP:
	   linkSuccess = 1;
      printf("uAP up \r\n");
      getNetPara(&para, Soft_AP);
      printf("Soft AP mode: IP address: %s \r\n", para.ip);
      printf("Soft AP mode: NetMask address: %s \r\n", para.mask);
      printf("Soft AP mode: MAC address: %s \r\n", para.mac);
      break;
    case MXCHIP_UAP_DOWN:
      printf("uAP down \r\n");
      break;
    default:
      break;
  }
  return;
}
// call back function of mxchipTick, 
// to back aplist around device,when mxchipStartScan() is called ,
// michipTick will callback this function to list all ap;
void ApListCallback(ScanResult *pApList)
{
	int i;
	printf("Find %d APs: \r\n", pApList->ApNum);
	for (i=0;i<pApList->ApNum;i++)
	printf("    SSID: %s, Signal: %d%%\r\n", pApList->ApList[i].ssid, pApList->ApList[i].ApPower);
}

// callback this function by mxchipTick() when DHCP success.
// param pnet is ip addr alocated by DHCP server
void NetCallback(net_para_st *pnet)
{
	strcpy((char *)UDPsendbuf,pnet->ip);
	printf("Station mode: IP address: %s \r\n", pnet->ip);
	printf("Station mode: NetMask address: %s \r\n", pnet->mask);
	printf("Station mode: Gateway address: %s \r\n", pnet->gate);
	printf("Station mode: DNS server address: %s \r\n", pnet->dns);
	printf("Station mode: MAC address: %s \r\n", pnet->mac);
}

//Net config
void InetConfig()
{
	libConfig.tcp_buf_dynamic = mxEnable;
	libConfig.tcp_max_connection_num = 1;
	libConfig.tcp_rx_size = 1024;
	libConfig.tcp_tx_size = 17408;//17K bytes
	libConfig.hw_watchdog = 0;
	libConfig.wifi_channel = WIFI_CHANNEL_1_13;
	lib_config(&libConfig);
}
//start AP model
void softAPModeStart()
{
	int ret;
	memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));	
	wNetConfig.wifi_mode = Soft_AP;
	strcpy((char*)wNetConfig.wifi_ssid, AP_NAME );
	strcpy((char*)wNetConfig.wifi_key, AP_PASSWORD);
	strcpy((char*)wNetConfig.local_ip_addr, "192.168.2.1");
	strcpy((char*)wNetConfig.net_mask, "255.255.255.0");
	strcpy((char*)wNetConfig.address_pool_start, "192.168.2.10");
	strcpy((char*)wNetConfig.address_pool_end, "192.168.2.20");
	wNetConfig.dhcpMode = DHCP_Server;
	ret = StartNetwork(&wNetConfig);
	printf("Setup soft AP: %s, return %d\r\n", wNetConfig.wifi_ssid, ret);
}
//start station model
void stationModeStart()
{
	int ret;
	wNetConfig.wifi_mode = Station;
	wNetConfig.dhcpMode = DHCP_Client;
	ret = StartNetwork(&wNetConfig);
	printf("Connect to %s..... return %d\r\n", wNetConfig.wifi_ssid,ret);
}
