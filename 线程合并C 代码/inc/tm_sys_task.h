#ifndef  TM_SYS_TASK_H_
#define  TM_SYS_TASK_H_
#include<pthread.h>
#include"tm_type.h"

/*ִ�е�������������ݳ���*/
#define  SYS_TASK_DATA_LEN  4046


typedef  void *(*pfSysTaskRoutine)(void *);		


/*ִ�е�������нṹ��*/
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

/*ִ�е������¼���*/
typedef struct SysTPool_st
{
	BOOL  bDestroy;										/* ������Ƿ����� */
	int32 iTaskMax;										/* ��������� */
	int32 iCount;										/* ��ǰ������ */
	SysListHead stHead;									/* SysTPoolTask����ͷ��� */
    pthread_t *pThreadID;								/* ������߳�ID */
	pthread_mutex_t	tpool_mtx;							/* ������ */
	pthread_cond_t tpool_cond;							/* ������������ */
} SysTPool;

#endif
