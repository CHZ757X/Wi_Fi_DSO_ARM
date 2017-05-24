#include "unpackfunc.h"
#include "math.h"

uint8_t* Databuffer;
CTLPK netpackctrl;

//�����ʼ����������������
void unpackinit(void)
{
	int i =0;
	netpackctrl.pointer = 0;
	netpackctrl.recvbufpter = 0;
	Databuffer = (uint8_t*)malloc(ALLLENGHT); 
}
//�����յ�����������н������ֹ���ĳ���,������1ʱ������δ���꣬������0ʱ��ʾ���������
int unpack(CTLPK* ctrpack,char* netbuf, int netbuflenght)//ÿ�����ư����ǹ̶���С������а��ոð��Ĵ�С���н��
{
	if(ctrpack->recvbufpter >= netbuflenght)//�ѽ����еİ����
		return 0;
	for(;ctrpack->recvbufpter < netbuflenght && ctrpack->pointer < RECBUFLENGHT;ctrpack->pointer++,ctrpack->recvbufpter++)
	{
			ctrpack->ctlbuffer[ctrpack->pointer] = netbuf[ctrpack->recvbufpter];
	} 
	if(ctrpack->pointer == RECBUFLENGHT)//���������һ�����ư�
	{
		ctrpack->pointer = 0;
		return 1;
	}
	else //������һ�����ư���ȫ�ȴ��´β�ȫ
		return 0;
}
//��FPGA�ж�ȡ����
void readbundledata(uint8_t* buffer,int recvlenght)//����������
{	
	if(wait_fifo()==1)
	{
		DATA_GPIO_config(WR_FLAG);
		data_Send(FPGA_FIFO_RW_CTR_L,0x08);//�ص�дʹ��
		DATA_GPIO_config(RD_FLAG);
		data_Receive(0xaa,buffer,recvlenght);
	}
	else //����Ӳ�����������android��
	{
		buffer[0] = 'e';
		buffer[1] = 'r';
		buffer[2] = 'r';
	}
}
//�Խ��յ�������������н���
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
				readbundledata(Databuffer,SENDBUFFERSIZE);//��ȡ��������
					send(farsock,Databuffer,ALLLENGHT,0);
					for(tmp_index =0 ; tmp_index< RECBUFLENGHT;tmp_index++)//��Ҫ���ص�״̬��Ϣ��������
					Databuffer[SENDBUFFERSIZE+tmp_index] = 0;
		}
		else if(netpackctrl.ctlbuffer[0] =='w' && netpackctrl.ctlbuffer[1] =='c')
		{					
				if(explainCommand(netpackctrl.ctlbuffer, recvdatalen) == 1) //�������ɹ�ִ�еĻ�explainCommand()����1
				{
					for(tmp_index =0 ; tmp_index< RECBUFLENGHT;tmp_index++)//�����ݽ��д��,����������Ϣ��ֵ�ڲ������ݵ�β��
							Databuffer[SENDBUFFERSIZE+tmp_index] = netpackctrl.ctlbuffer[tmp_index];			
					hw_ExecuteCommand();	//ִ��Ӳ������
				}					
		}
		DATA_GPIO_config(WR_FLAG);//�ı��������ݴ��䷽��
		dpx_InitFifo(0x8804);
	}
	netpackctrl.recvbufpter = 0;
}
