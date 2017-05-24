#include"main.h"
extern delay50ns(void);

int UDPfd = -1;
char UDPsendbuf[20] = {0};
int wifi_up = 0;
volatile short configSuccess=0;
volatile short linkSuccess=0;
volatile short ap_mode_flag=0;
volatile short easylink =1;
network_InitTypeDef_st wNetConfig;
lib_config_t libConfig;
net_para_st para; 
const char *securityString[8] = {"Open system", "WEP", "WPA TKIP", "WPA AES", "WPA2 TKIP",
                           "WPA2 AES",  "WPA2 MIXED", "Auto"};


/**
**link device to smartphone
**/
void linkdevice2cell(struct sockaddr_t addrt)
{
	int i = 3;
	configSuccess = 0;
	linkSuccess = 0;
	ap_mode_flag = 0;
	easylink = 1;
	while(1){
		mxchipTick();
		if(ap_mode_flag )
		{			
			softAPModeStart();
			ap_mode_flag	= 0;		
		}
		if(easylink){
			OpenEasylink(60);
		}
		if(configSuccess){
			stationModeStart();
			configSuccess = 0;
		}
		if(linkSuccess){
			linkSuccess = 0;
			UDPfd = socket(AF_INET,SOCK_DGRM,IPPROTO_UDP);
			addrt.s_port = 8090;
			if(UDPfd !=-1){
				bind(UDPfd,&addrt,sizeof (addrt));
				printf("Open UDP port %d \r\n",addrt.s_port);
				addrt.s_ip = INADDR_BROADCAST;
				addrt.s_port = 5353;
				while(i--)
				{
					sendto(UDPfd,UDPsendbuf,20,0,&addrt,sizeof (addrt));
					delay_us(100);
				}
				close(UDPfd);
			break;
			}
		}
	}
}

#define BUFFER_SIZE1             (sizeof(Tx1_Buffer)-1)
#define sEE_WRITE_ADDRESS1        0x50
#define sEE_READ_ADDRESS1         0x50

uint8_t Tx1_Buffer[] = "/* STM32F20xx I2C Firmware Library EEPROM driver example: \
                        buffer 1 transfer into address sEE_WRITE_ADDRESS1 */ \
                        Example Descriptio";
uint8_t Rx1_Buffer[BUFFER_SIZE1];
volatile uint16_t NumDataRead = 0;

int main(void)
{
	
	int fd_listen,far_sock;
	int con = 0,len = 0,tmp_index = 0;
	char rdbuffer[1024],ipaddr[16];
	struct sockaddr_t addr;	
	struct timeval_t t;
	uint8_t retvalue = 0; 
	fd_set readfs,exception;
//寄存器状态初始化
	fd_listen = -1;
	far_sock = -1;

	TimeConfig();	
	InetConfig();	
	Button_Init();	
	Control_GPIO_config();
	ADCLED_GPIO_config();
	mxchipInit();
	unpackinit();
	swi2c_init();//初始化i2c管脚
  swi2c_blkwrite(0xA0, sEE_WRITE_ADDRESS1, Tx1_Buffer, BUFFER_SIZE1);
	delay_us(10000);
	swi2c_blkread(0xA0,sEE_WRITE_ADDRESS1,Rx1_Buffer,BUFFER_SIZE1);
	
// 程序主体1
	while(1){
		linkdevice2cell(addr);//设备与手机配对函数
		//硬件初始化
		DATA_GPIO_config(WR_FLAG);		
		hw_SmartStart();	
		initSystem();		
		dpx_InitFifo(0x8804);//初始化fifo
		while(1)
		{	
			mxchipTick();			
			if(fd_listen == -1)
			{
				linkSuccess = 0;
				fd_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				addr.s_port = 5000;
				bind(fd_listen,&addr,sizeof(addr));
				listen(fd_listen,0);
				printf("TCP server establised at port:%d\r\n",addr.s_port);
			}
			FD_ZERO(&readfs);
			FD_SET(fd_listen,&readfs);
			if(far_sock != -1)
			{
				FD_SET(far_sock,&readfs);
			}   
			t.tv_sec = 0;
			t.tv_usec = 100;
			select(2,&readfs,NULL,&exception,&t);
			if(FD_ISSET(fd_listen,&readfs))
			{	
				len = sizeof(addr);
				far_sock = accept(fd_listen,&addr,&len);
				printf("get far_sock");
				if(far_sock != -1)
				{
					inet_ntoa(ipaddr,addr.s_ip);
					printf("TCP client establised \nip:%s \nport%d\r\n",ipaddr,addr.s_port);
				}
			}				
			if(far_sock != -1)
			{
				if(FD_ISSET(far_sock,&readfs))
				{	
					con = recv(far_sock,rdbuffer,sizeof(rdbuffer),0);
					if(con >0 )
					{
						 explainnetcmd(rdbuffer,con,fd_listen,far_sock);
					}
					else 
					{				
						printf("con<0\n");
						close(far_sock);
						far_sock = -1;
					}
				}							
				else if(FD_ISSET(far_sock,&exception))
				{
					printf("fd_listen exception\n");
					far_sock = -1;
				}		
			}
				
		}
	}
}

void ap_set_function(void)
{
	//irs function
	ap_mode_flag = 1;
	easylink = 0;
	printf("中断产生\n");
}
