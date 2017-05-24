#include "xControl.h"
//x系统
struct _XScale _oPicoSecPerDiv[]=
{    
	//插值档
	{10000,			10*(6-5),					1,              1},  //10ns
	{20000,			10*(6-5),					2,              1},  //20ns
	{50000,			10*(6-5),			       	5,             1},  //50ns
	{100000,			10*(6-50000000),				10,              1},  //100ns
	{200000,			10*(6-25000000),				20,              2},  //200ns
	///
	{500000,			10*(6-10000000),				1,              1},  //500ns
	{1000000,			10*(6-5000000),				2,             1},  //us
	{2000000,			10*(6-2500000),				4,             1},
	{5000000,			  10*(6-1000000),				1,             1},
	{10000000,			10*(6-1000000),				2,             1},
	{20000000,			10*(6-500000),				4,             1},
	{50000000,			10*(6-200000),				5,             1},//50us
	{100000000,			10*(6-100000),				5,             2},
	{200000000,			10*(6-50000),				10,             4},
	{500000000,			10*(6-20000),				10,             10},
	{1000000000,		10*(6-10000),				10,             20},	//1ms
	{2000000000,		10*(6-5000),				10,             40},//2ms
	{5000000000,		10*(6-2000),				10,             100},  //2000
	{10000000000,		10*(6-1000),				10,             200},
	{20000000000,		10*(6-500),					10,             400},
	{50000000000,		10*(6-200),					10,             1000},
	//SCAN档	
	{100000000000,		10*(6-100),					10,             2000},
	{200000000000,		10*(6-50),					10,             4000},
	{500000000000,		10*(6-20),					10,             10000},
	{1000000000000,		10*(6-10),					10,             20000},	//s
	{2000000000000,		10*(6-5),					10,             40000},
	{5000000000000,		10*(6-2),					10,             100000},
	{10000000000000,		10*(6-1),					10,             200000},
	{20000000000000,		10*6-10/2,					10,             400000},
	{50000000000000,		10*6-10/5,					10,             1000000},
};

//////////////////////////////设置调整主时基水平系统/////////////////////////////////////
void _AdjMasterXSensitivity(struct ZoomSystem *zsystem, short Delta)
{
//	if ( IsZoom() )
//	{
//		_AdjZoomSensitivity(this, pContainer, Delta);

		//SEND HARDWARE COMMAND
//		HW_PushCommand(HWCMDID_XSYS_SAMPLE_SLAVE); 
//	}
//	else	
		_AdjMasterSensitivity(zsystem->pXModule, Delta);
}

void _AdjMasterXPosition(struct ZoomSystem *zsystem, short Delta)
{
//	if ( IsZoom() )
//	{
//		_AdjZoomPosition(this, pContainer, Delta);

		//SEND HARDWARE COMMAND
//		HW_PushCommand(HWCMDID_XSYS_DEEP_SLAVE); 
//		if (Delta)
//			DPX_SuspendMap();
//	}
//	else
		_AdjMasterPosition(zsystem->pXModule, Delta);
}

void _GetMasterTimePerDiv(const struct ZoomSystem *zsystem, struct UnitLogicData *pLogicData)
{
//	if ( IsZoom() )
//		gGetXLogicLevel(this->Sensitivity, pLogicData);
//	else
		gGetXLogicLevel(zsystem->pXModule->Sensitivity, pLogicData); 
}

long long _GetMasterDepth(const struct ZoomSystem *zsystem)
{
//	if ( IsZoom() )
//		return this->Depth;
//	else
		return zsystem->pXModule->pDepth->Depth;
}

struct ZoomOperations _MasterOps=
{
	_GetMasterTimePerDiv,
	_GetMasterDepth,

	_AdjMasterXSensitivity,
	_AdjMasterXPosition,
};

//////////////////////////////设置调整从时基水平系统/////////////////////////////////////
struct ZoomOperations _SlaveOps=
{
	NULL,
	NULL,
	NULL,
	NULL,
};
///////////////////////////////////定义水平变量//////////////////////////////////////////
struct XDepth _oXDepth[2]=
{
	{0.0, TIMEBASE_ZERO_POS,},

	{0.0, TIMEBASE_ZERO_POS,}
};

struct XSystem gXSystem[2]=
{
	{//从采集时基，（在物理通道使用独立时基时使用）
			LEV1US,
			25,
			1,
			MAXDISPDOTS,
			_oXDepth+0
	},
	{//默认主采集时基
			LEV1US,
			25,
			1,
			MAXDISPDOTS,
			_oXDepth+1
	},

};
//扩展时基定义、初始化
struct ZoomSystem gZoomXSystem[2]=
{
	{//从时基扩展时基（在物理通道使用独立时基时使用）

			LEV500NS,//Sensitivity 
			TIMEBASE_ZERO_POS,//Depth
			TIMEBASE_ZERO_POS,//Center	
			MAXDISPDOTS,//Width
			gXSystem+0,//pXModule
			&_SlaveOps,//pOps

	},
	{//默认主扩展时基
		
			LEV500NS,//Sensitivity
			TIMEBASE_ZERO_POS,//Depth
			TIMEBASE_ZERO_POS,//Center
			MAXDISPDOTS,//Width
			gXSystem+1,//pXModule
			&_MasterOps,//pOps		
	},
	
};
///////////////////////////////////水平系统的各个操作函数//////////////////////////////////////////
/************************************/
/*函数功能：硬件函数			*/
/************************************/
void XCtrl_Config(void)
{
	//物理X系统参数合法性检测
	_AdjMasterPosition(&gXSystem[CHANNEL_ID_PRIMARY], 0);
	_AdjMasterSensitivity(&gXSystem[CHANNEL_ID_PRIMARY], 0);
}
/************************************/
/*函数功能：结构体构造函数		*/
/************************************/
void XCtrl_CreateDefDepth(void)
{
	_oXDepth[0].Time	= 0.0;
	_oXDepth[0].Depth	= TIMEBASE_ZERO_POS;

	_oXDepth[1].Time	= 0.0;
	_oXDepth[1].Depth	= TIMEBASE_ZERO_POS;
}

void XCtrl_CreateDefPhyXSys(void)
{
	gXSystem[0].Sensitivity	= LEV10US;
	gXSystem[0].Lenght= MAXDISPDOTS;
	gXSystem[0].pDepth= _oXDepth+0;

	gXSystem[1].Sensitivity	= LEV10US;
	gXSystem[1].Lenght= MAXDISPDOTS;
	gXSystem[1].pDepth= _oXDepth+1;
}

void XCtrl_CreateDefZoomSys(void)
{
	short i;

	for (i=0; i<CHANNEL_MAX_COUNT; i++)
	{
		gZoomXSystem[i].Sensitivity	= LEV1MS;
		gZoomXSystem[i].Depth		= TIMEBASE_ZERO_POS;
		gZoomXSystem[i].Center		= TIMEBASE_ZERO_POS;
		gZoomXSystem[i].Width		= MAXDISPDOTS;
	}

	gZoomXSystem[CHANNEL_ID_SLAVE].pXModule		= gXSystem + 0;
	gZoomXSystem[CHANNEL_ID_SLAVE].pOps			= &_SlaveOps;

	gZoomXSystem[CHANNEL_ID_PRIMARY].pXModule	= gXSystem + 1;
	gZoomXSystem[CHANNEL_ID_PRIMARY].pOps		= &_MasterOps;
}

void XCtrl_CreateDefXSys(void)
{
	XCtrl_CreateDefDepth();

	XCtrl_CreateDefPhyXSys();

	XCtrl_CreateDefZoomSys();
}	
/************************************/
/*函数功能：调整主时基档位			*/
/************************************/
void _AdjMasterSensitivity(struct XSystem *xsystem, short Delta)
{    
	xsystem->Sensitivity = Delta;
	if (xsystem->Sensitivity > TIMEBASE_MAX_SCALE)
	{
		xsystem->Sensitivity = TIMEBASE_MAX_SCALE;
	}
	else if (xsystem->Sensitivity < TIMEBASE_MIN_SCALE)
	{
		xsystem->Sensitivity = TIMEBASE_MIN_SCALE;
	}

	xsystem->H_samples = _oPicoSecPerDiv[xsystem->Sensitivity].H_Factor;
	//根据触发时间，改变触发深度
	//	_UpdateMasterPosition(xsystem);

	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_XSYS_SAMPLE_MASTER); 

//	if ( IS_SCANAUTO() )
//	{
//		gAcqStatusFlag=PROC_SCAN;
//	}

}
/************************************/
/*函数功能：根据触发时间调整触发深度		*/
/************************************/
void _UpdateMasterPosition(struct XSystem *xsystem)
{
	long double ldTriggerTime;

	ldTriggerTime = xsystem->pDepth->Time;
	xsystem->pDepth->Depth = (long long)( ldTriggerTime /_oPicoSecPerDiv[xsystem->Sensitivity].Scale / 1000000.0f  * XDOTS_PER_DIV );
	xsystem->pDepth->Depth = TIMEBASE_ZERO_POS - xsystem->pDepth->Depth;

	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_XSYS_DEEP_MASTER); 
}
/************************************/
/*函数功能：调整触发位置			*/
/************************************/
void _AdjMasterPosition(struct XSystem *xsystem, short Delta)
{
	xsystem->pDepth->Depth += Delta;

	//更新触发时间
	xsystem->pDepth->Time = gGetXDelay(xsystem->Sensitivity, xsystem->pDepth->Depth);

	//SEND HARDWARE COMMAND
	hw_PushCommand(HWCMDID_XSYS_DEEP_MASTER); 
}
/************************************/
/*函数功能：更新触发时间			*/
/************************************/
float gGetXDelay(short Sensitivity, short Depth)
{
	float TimePerDiv;

	TimePerDiv = (_oPicoSecPerDiv[Sensitivity].Scale / 1000000.0f);
	TimePerDiv = (TIMEBASE_ZERO_POS - Depth) * TimePerDiv / XDOTS_PER_DIV;
	return TimePerDiv;
}

/************************************/
/*根据档位获取对应的ms/DIV		    */
/************************************/
void gGetXLogicLevel(short Sensitivity, struct UnitLogicData *pLogicData)
{   
	if (pLogicData)
	{
		pLogicData->Value = (_oPicoSecPerDiv[Sensitivity].Scale / 1000000.0f);
		pLogicData->oUnit.Unit = UNT_SECOND;
		pLogicData->oUnit.Multiple = UNT_MICRO;	
	}
}
