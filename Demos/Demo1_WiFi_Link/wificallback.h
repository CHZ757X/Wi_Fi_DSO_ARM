#ifndef WIFICALLBACK_H
#define WIFICALLBSCK_H

#include "stdio.h"
#include "stdlib.h"
#include "stm32f2xx.h"
#include "platform.h"
#include "mxchipWNET.h"

#define LowPowerMode
#define AP_NAME           "EMW_CELL"
#define AP_PASSWORD       ""
#define APP_INFO          "Wi-Fi Link"

extern volatile short configSuccess;
extern volatile short linkSuccess;
extern volatile short ap_mode_flag;
extern volatile short easylink ;
extern network_InitTypeDef_st wNetConfig;
extern lib_config_t libConfig;
extern net_para_st para; 
extern char UDPsendbuf[20];


void system_version(char *str, int len);

void easylink_user_data_result(int datalen, char* data);

//callback while easylink is opened successly
void RptConfigmodeRslt(network_InitTypeDef_st *nwkpara);

//callback function of mxchipTick, it will be called by mxchipTick() when the status of Net change.
void WifiStatusHandler(int event);

// call back function of mxchipTick, 
// to back aplist around device,when mxchipStartScan() is called ,
// michipTick will callback this function to list all ap;
void ApListCallback(ScanResult *pApList);

// callback this function by mxchipTick() when DHCP success.
// param pnet is ip addr alocated by DHCP server
void NetCallback(net_para_st *pnet);

//Net config
void InetConfig(void);

//start AP model
void softAPModeStart(void);

//start station model
void stationModeStart(void);

#endif
