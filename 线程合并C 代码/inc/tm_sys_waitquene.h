/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_sys_waitquene.h
 * Author        : xundanqing
 * Date          : 2018-07-07
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-07-07
 *   Author      : xundanqing
 *   Modification: Created file

*************************************************************************************************************/
#ifndef TM_SYS_WAITQUE_H
#define TM_SYS_WAITQUE_H

#include<stdio.h>
#include<pthread.h>
#include"tm_debug.h"
#include"tm_sys_timer.h"
#include"tm_serial_base.h"
#include"tm_socket_base.h"


/* ��ʱ�ڵ���ѯʱ���� */
#define   WAITQUE_TIME (100)

/*���Ͷ��м���ֵ*/
typedef struct   sthashkey
{ 
   int16   cmd;
   int16   seq;
   
}tWaitkey;


/*���Ͷ��м���ֵ*/
typedef  union  
{
  int       iKey;
  tWaitkey  stKey;
  
}WaitQueKey;


/*�������ݰ��ṹ*/
typedef struct   stPackinfo
{
   int16  iCmd;
   int16  iSeq;
   int16  Bufferlen;
   int8*  aucBuffer;
   
}__attribute__ ((__packed__))tPackInfo, *PtPackInfo;



/* ��ʱ���ͽڵ�ṹ���� */
typedef struct    stQuenode
{  
   int  iNodeKey;                         /*��ʱ���͵����ݲ��ұ��*/
   int  iTransval;                        /*�ڵ�ķ���ʣ���������*/
   int  iWaitTime;                        /*���ͽڵ��Ѿ����͵Ĵ���*/
   int  iBufferlen;                       /*�������ݵĻ��峤��ֵ*/
   unsigned char  *aucBuffer;             /*���ͻ���������ָ��*/
   SysListHead     stList; 
   
}__attribute__ ((__packed__))TQueNode,*PTQueNode;




/* ��ʱ���Ͷ��е����Խṹ */
typedef struct  stQueattribute
{
   int  iQueSendVal;                       /*�����нڵ㷢�͵Ĵ���*/
   int  iQueWaitVal;                       /*���е��ڵ�ȴ�ʱ��*/
   int  iQueWarnVal;                       /*���нڵ���ֵ��ʾֵ*/
   int  (*QueWarnCallBack)();              /*���нڵ���ֵ����*/
   int  (*QueSendCallBack)(unsigned  char *aucbuf ,int ilen);  /*���ݴ����ͻص�����*/
   
}__attribute__ ((__packed__))TQueAtrribute, *PTQueAttribute;



/*��ʱ���ͻ������*/
typedef struct  stWaitque
{
    int             QueTimeId;              /*��ʱ�������ID*/
    TQueAtrribute   QueAtribute;            /*��ʱ���е�����*/
    int           (*QueTimerFun)(void *arg);/*��ʱĬ�ϵĻص�����*/
    int             iQueNodeVal;            /*��ǰ�����еĽڵ���*/
    SysListHead     stList;    
    pthread_mutex_t QueMutex;
    
}__attribute__ ((__packed__))TWaitQue ,*PTWaitQue;

#endif

