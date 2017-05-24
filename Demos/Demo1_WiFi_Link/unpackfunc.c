#include "unpackfunc.h"
#include "math.h"

uint8_t* Databuffer;
CTLPK netpackctrl;

//网络初始化函数，包括生成
void unpackinit(void)
{
	int i =0;
	netpackctrl.pointer = 0;
	netpackctrl.recvbufpter = 0;
	Databuffer = (uint8_t*)malloc(ALLLENGHT); 
}
//将接收到的网络包进行解包，防止黏包的出现,当返回1时表明包未解完，当返回0时表示包解析完毕
int unpack(CTLPK* ctrpack,char* netbuf, int netbuflenght)//每个控制包都是固定大小，设计中按照该包的大小进行解包
{
	if(ctrpack->recvbufpter >= netbuflenght)//已将所有的包解出
		return 0;
	for(;ctrpack->recvbufpter < netbuflenght && ctrpack->pointer < RECBUFLENGHT;ctrpack->pointer++,ctrpack->recvbufpter++)
	{
			ctrpack->ctlbuffer[ctrpack->pointer] = netbuf[ctrpack->recvbufpter];
	} 
	if(ctrpack->pointer == RECBUFLENGHT)//表明解出了一个控制包
	{
		ctrpack->pointer = 0;
		return 1;
	}
	else //表明有一个控制包不全等待下次补全
		return 0;
}
//从FPGA中读取数据
void readbundledata(uint8_t* buffer,int recvlenght)//连续读数据
{	
	if(wait_fifo()==1)
	{
		DATA_GPIO_config(WR_FLAG);
		data_Send(FPGA_FIFO_RW_CTR_L,0x08);//关掉写使能
		DATA_GPIO_config(RD_FLAG);
		data_Receive(0xaa,buffer,recvlenght);
	}
	else //反馈硬件错误命令给android机
	{
		buffer[0] = 'e';
		buffer[1] = 'r';
		buffer[2] = 'r';
	}
}
//对接收到的数据命令进行解析
void explainnetcmd(char* cmdbuf,int recvdatalen,int listensock,int farsock)
{
	int tmp_index = 0;
	while(unpack(&netpackctrl,cmdbuf,recvdatalen)==1)
	{
		printf("uppack  ");
		if(netpackctrl.ctlbuffer[0] =='e' && netpackctrl.ctlbuffer[1] =='x')
		{						
					free(Databuffer);
					close(farsock);
					close(listensock);	
					wlan_disconnect();	
					SystemReset();
		}
		else if(netpackctrl.ctlbuffer[0] == 'r' && netpackctrl.ctlbuffer[1] == 'd')
		{		
				readbundledata(Databuffer,SENDBUFFERSIZE);//读取波形数据
					send(farsock,Databuffer,ALLLENGHT,0);
					for(tmp_index =0 ; tmp_index< RECBUFLENGHT;tmp_index++)//将要返回的状态信息进行清零
					Databuffer[SENDBUFFERSIZE+tmp_index] = 0;
		}
		else if(netpackctrl.ctlbuffer[0] =='w' && netpackctrl.ctlbuffer[1] =='c')
		{					
				if(explainCommand(netpackctrl.ctlbuffer, recvdatalen) == 1) //如果命令被成功执行的话explainCommand()返回1
				{
					for(tmp_index =0 ; tmp_index< RECBUFLENGHT;tmp_index++)//将数据进行打包,即将控制信息赋值在波形数据的尾部
							Databuffer[SENDBUFFERSIZE+tmp_index] = netpackctrl.ctlbuffer[tmp_index];			
					hw_ExecuteCommand();	//执行硬件操作
				}					
		}
		DATA_GPIO_config(WR_FLAG);//改变总线数据传输方向
		dpx_InitFifo(0x8804);
	}
	netpackctrl.recvbufpter = 0;
}
