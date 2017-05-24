#ifndef UNPACKFUNC_H
#define UNPACKFUNC_H

#include <stdio.h>
#include "instructionSet.h"
#include "stdint.h"

#define RECBUFLENGHT 20
#define SENDBUFFERSIZE 		16384
#define ALLLENGHT (SENDBUFFERSIZE+RECBUFLENGHT) //返回包的总长度

typedef struct contrlPack
{
	int recvbufpter;
	int pointer;
	char ctlbuffer[20];
}CTLPK;

void unpackinit(void);
void explainnetcmd(char* cmdbuf,int recvdatalen,int listensock,int farsock);
#endif
