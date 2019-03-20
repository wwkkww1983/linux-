/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_sys_timer.c
 * Author        : xundanqing
 * Date          : 2018-07-06
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-07-06
 *   Author      : xundanqing
 *   Modification: Created file

*************************************************************************************************************/
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>
#include"tm_debug.h"
#include"tm_type.h"
#include"tm_decoler.h"
#include"tm_sys_list.h"
#include"tm_sys_timer.h"

/* 全局的定时器链表 */
static  SysTimer   g_stSysTimer;
/*****************************************************************************
 * Function      : TM_Sleep
 * Description   : 定时器延时睡眠函数
 * Input         : int nSec   
                int nUSec  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180706
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void    TM_Sleep(int nSec, int nUSec)
{
	struct timeval tv;
	fd_set s;
	FD_ZERO(&s);
	tv.tv_sec = nSec;
	tv.tv_usec = nUSec;
	select(0, NULL, NULL, NULL, &tv);
}
/*****************************************************************************
 * Function      : SysTimer_ListEntry_Proc
 * Description   : Systimer  节点处理
 * Input         : SysTimerEntry *pstPos  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180706
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int  SysTimer_ListEntry_Proc(SysTimerEntry *pstPos)
{   
    int32       iInterVal;
    iInterVal = pstPos->iInterval;

    if( !pstPos->iRemain)
    {
        pstPos->pfHandle((void *)NULL);

        if(!pstPos->iRoundFlag)
            pstPos->iRepeat-- ;

        if(pstPos->iRepeat)
           pstPos->iRemain = iInterVal;
    }
    
    else
        pstPos->iRemain--;

    return    0;
}
/*****************************************************************************
 * Function      : timer_unique_id
 * Description   : 分配定时器标示ID
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180706
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
uint32 timer_unique_id()
{

	uint32 uiTimerID = 0;
	SysTimerEntry *pstPos = NULL;

    Mutex_Lock(&g_stSysTimer.timer_mtx);

    static uint32 uiSeq = 1;
    while (1)
    {
    	SYS_LIST_FOR_EACH_ENTRY(pstPos, &g_stSysTimer.stHead, stList)
	    {
			if (pstPos->uiTimerID == uiSeq)
	        {
	        	break;
	        }
	    }

        if (&pstPos->stList != &g_stSysTimer.stHead)
        {
        	uiSeq++;
        }
        else
        {
        	break;
        }
    }

	uiTimerID = uiSeq;
	
    Mutex_UnLock(&g_stSysTimer.timer_mtx);
    
    return uiTimerID;
}
/*****************************************************************************
 * Function      : SysTimerRoutine
 * Description   : 定时器线程处理函数
 * Input         : void *arg  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180706
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void*   SysTimerRoutine(void *arg)
{

    while(1)
    {
    
       SysTimerEntry *pstPos = NULL;
       
	   Mutex_Lock(&g_stSysTimer.timer_mtx);

       SYS_LIST_FOR_EACH_ENTRY(pstPos, &g_stSysTimer.stHead, stList)
       {
       
          SysTimer_ListEntry_Proc(pstPos);
       }

       SYS_LIST_FOR_EACH_ENTRY(pstPos, &g_stSysTimer.stHead, stList)
	   {
	   
 		  if((pstPos->iRepeat <= 0) && !(pstPos->iRoundFlag))
	      {
	          
        	  SYS_ListDel(&(pstPos->stList));
              free(pstPos);
          }
       }
       
       Mutex_UnLock(&g_stSysTimer.timer_mtx);
       
       TM_Sleep(0,SYS_TIMER_PRECISION * 1000);
    }
}
/*****************************************************************************
 * Function      : SYS_Timer_Delete
 * Description   : 删除一个定时器删除定时器
 * Input         : uint32  TimerId  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180706
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  SYS_Timer_Delete(uint32  uiTimerID)
{
	BOOL bRet = SYS_FALSE;
	SysTimerEntry *pstPos = NULL;

	Mutex_Lock(&g_stSysTimer.timer_mtx);
	SYS_LIST_FOR_EACH_ENTRY(pstPos, &g_stSysTimer.stHead, stList)
    {
		if (pstPos->uiTimerID == uiTimerID)
        {
        	break;
        }
    }

    if (&pstPos->stList != &g_stSysTimer.stHead)
    {
		pstPos->iRepeat   =  0;
		pstPos->iRoundFlag = 0;
        g_stSysTimer.iCount--;
        bRet = SYS_TRUE;
    }
    else
    {
        bRet = SYS_FALSE;
    }
    Mutex_UnLock(&g_stSysTimer.timer_mtx);

    return bRet;
}
/*****************************************************************************
 * Function      : SYS_TimerAdd
 * Description   : 添加定时器
 * Input         : int32 iInterval            
                int32 iRepeat              
                pfSysTimerHandle pfHandle  
                uint32 *puiTimerID         
                int  iFlag                 
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180706
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int   SYS_TimerAdd(int32 iInterval, int32 iRepeat, pfSysTimerHandle pfHandle, uint32 *puiTimerID,int  iFlag)
{

	SysTimerEntry *pstTimer = NULL;

    Mutex_Lock(&g_stSysTimer.timer_mtx);
    if (g_stSysTimer.iCount >= g_stSysTimer.iTimerMax)
    {
        Mutex_UnLock(&g_stSysTimer.timer_mtx);
        return   ERROR;
    }
    Mutex_UnLock(&g_stSysTimer.timer_mtx);

	pstTimer = (SysTimerEntry *)malloc(sizeof(SysTimerEntry));
	
	if (!pstTimer)
	{
		return   ERROR;
	}

    pstTimer->iInterval =  iInterval / SYS_TIMER_PRECISION;
    pstTimer->iRemain   =  iInterval / SYS_TIMER_PRECISION;
	pstTimer->iRepeat   =  iRepeat ;
    pstTimer->pfHandle  =  pfHandle;
    pstTimer->iRoundFlag=  iFlag;
    pstTimer->uiTimerID =  timer_unique_id();

   *puiTimerID = pstTimer->uiTimerID;

    Mutex_Lock(&g_stSysTimer.timer_mtx);
    
    g_stSysTimer.iCount++;
    
    SYS_ListAdd_Tail(&(pstTimer->stList), &(g_stSysTimer.stHead));
    
	Mutex_UnLock(&g_stSysTimer.timer_mtx);

    return    SUCESS;
    
}
/*****************************************************************************
 * Function      : SYS_TimerInit
 * Description   : sys timer 初始化
 * Input         : int32 iTimerMax  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180706
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  SYS_TimerInit(int32 iTimerMax)
{

	g_stSysTimer.iTimerMax = iTimerMax;
	
    SYS_LIST_HEAD_INIT(g_stSysTimer.stHead);
    
    if(pthread_mutex_init (&g_stSysTimer.timer_mtx, NULL) < 0)
    {
        return  ERROR;
    }
    
	if(pthread_create(&g_stSysTimer.timer_t,NULL,SysTimerRoutine,NULL) <0)
	{
        return  ERROR;
    }

	return     SUCESS;
	
}

