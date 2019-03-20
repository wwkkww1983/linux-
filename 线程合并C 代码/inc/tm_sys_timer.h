#ifndef  TM_SYS_TIMER_H
#define  TM_SYS_TIMER_H

#include <pthread.h>
#include "tm_type.h"
#include "tm_sys_list.h"


#define SYS_TIMER_PRECISION	     (10)

/**
 *@��ʱ��������ָ�붨��
 */
typedef void (*pfSysTimerHandle)(void *);			  /* ��ʱ������������ */

/**
 *@��ʱ������ڵ�ṹ
 */
typedef struct SysTimerEntry_st
{
	uint32 uiTimerID;									/* ��ʱ��ID */
	uint32 iRoundFlag;                                  /* ��ʱ����ѯ��־*/
	int32  iRepeat;										/* �ظ����� */
	int32  iInterval;									/* �������λ100ms */
	int32  iRemain;										/* ʣ��ʱ�䣬 ��λ100ms */
	SysListHead stList;									/* ��ʱ������ */
	pfSysTimerHandle pfHandle;							/* ��Ӧ���� */
    pthread_mutex_t repeat_mtx;
} SysTimerEntry;

/**
 *��ʱ������ṹ 
 */
typedef struct SysTimer_st
{
    int32             iCount;							/* ��ǰ��ʱ��������Ŀ�� */
    int32          iTimerMax;						    /* ��ʱ�����������Ŀ�� */
    uint32          uiTaskID;						    /* �����������ID */
    SysListHead       stHead;							/* SysTimerEntry����ͷ��� */
	pthread_t       timer_t;                            /* �̴߳�����ʾ��*/
	pthread_mutex_t	timer_mtx;							/* ��ʱ�������� */
} SysTimer;
#endif
