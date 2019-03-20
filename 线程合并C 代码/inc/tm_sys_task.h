#ifndef  TM_SYS_TASK_H_
#define  TM_SYS_TASK_H_
#include<pthread.h>
#include"tm_type.h"

/*执行的任务输入最长数据长度*/
#define  SYS_TASK_DATA_LEN  4046


typedef  void *(*pfSysTaskRoutine)(void *);		


/*执行的任务队列结构体*/
typedef struct   SysTPoolTask_st
{
	BOOL   bContinue;                              
	BOOL   bStart;
	uint32 uiTaskID;
	pfSysTaskRoutine  pfRoutinue;
	SysListHead       tasklist;
	uint8             aucData[SYS_TASK_DATA_LEN];
    pthread_mutex_t   continue_mtx;
}

/*执行的任务事件池*/
typedef struct SysTPool_st
{
	BOOL  bDestroy;										/* 任务池是否销毁 */
	int32 iTaskMax;										/* 任务最大数 */
	int32 iCount;										/* 当前任务数 */
	SysListHead stHead;									/* SysTPoolTask队列头结点 */
    pthread_t *pThreadID;								/* 任务池线程ID */
	pthread_mutex_t	tpool_mtx;							/* 任务锁 */
	pthread_cond_t tpool_cond;							/* 任务条件变量 */
} SysTPool;

#endif
