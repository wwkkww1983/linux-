#ifndef  TM_SYS_TIMER_H
#define  TM_SYS_TIMER_H

#include <pthread.h>
#include "tm_type.h"
#include "tm_sys_list.h"


#define SYS_TIMER_PRECISION	     (10)

/**
 *@定时器处理函数指针定义
 */
typedef void (*pfSysTimerHandle)(void *);			  /* 定时器处理函数类型 */

/**
 *@定时器链表节点结构
 */
typedef struct SysTimerEntry_st
{
	uint32 uiTimerID;									/* 定时器ID */
	uint32 iRoundFlag;                                  /* 定时器轮询标志*/
	int32  iRepeat;										/* 重复次数 */
	int32  iInterval;									/* 间隔，单位100ms */
	int32  iRemain;										/* 剩余时间， 单位100ms */
	SysListHead stList;									/* 定时器队列 */
	pfSysTimerHandle pfHandle;							/* 响应处理 */
    pthread_mutex_t repeat_mtx;
} SysTimerEntry;

/**
 *定时器链表结构 
 */
typedef struct SysTimer_st
{
    int32             iCount;							/* 当前定时器队列条目数 */
    int32          iTimerMax;						    /* 定时器队列最大条目数 */
    uint32          uiTaskID;						    /* 任务池中任务ID */
    SysListHead       stHead;							/* SysTimerEntry队列头结点 */
	pthread_t       timer_t;                            /* 线程创建标示符*/
	pthread_mutex_t	timer_mtx;							/* 定时器队列锁 */
} SysTimer;
#endif
