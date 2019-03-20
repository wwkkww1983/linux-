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


/* 超时节点轮询时间间隔 */
#define   WAITQUE_TIME (100)

/*发送队列检索值*/
typedef struct   sthashkey
{ 
   int16   cmd;
   int16   seq;
   
}tWaitkey;


/*发送队列检索值*/
typedef  union  
{
  int       iKey;
  tWaitkey  stKey;
  
}WaitQueKey;


/*发送数据包结构*/
typedef struct   stPackinfo
{
   int16  iCmd;
   int16  iSeq;
   int16  Bufferlen;
   int8*  aucBuffer;
   
}__attribute__ ((__packed__))tPackInfo, *PtPackInfo;



/* 超时发送节点结构定义 */
typedef struct    stQuenode
{  
   int  iNodeKey;                         /*超时发送的数据查找编号*/
   int  iTransval;                        /*节点的发送剩余次数缓存*/
   int  iWaitTime;                        /*发送节点已经发送的次数*/
   int  iBufferlen;                       /*发送数据的缓冲长度值*/
   unsigned char  *aucBuffer;             /*发送缓冲中数据指针*/
   SysListHead     stList; 
   
}__attribute__ ((__packed__))TQueNode,*PTQueNode;




/* 超时发送队列的属性结构 */
typedef struct  stQueattribute
{
   int  iQueSendVal;                       /*队列中节点发送的次数*/
   int  iQueWaitVal;                       /*队列单节点等待时间*/
   int  iQueWarnVal;                       /*队列节点阈值提示值*/
   int  (*QueWarnCallBack)();              /*队列节点阈值处理*/
   int  (*QueSendCallBack)(unsigned  char *aucbuf ,int ilen);  /*数据处理发送回调函数*/
   
}__attribute__ ((__packed__))TQueAtrribute, *PTQueAttribute;



/*超时发送缓冲队列*/
typedef struct  stWaitque
{
    int             QueTimeId;              /*定时器分配的ID*/
    TQueAtrribute   QueAtribute;            /*超时队列的属性*/
    int           (*QueTimerFun)(void *arg);/*超时默认的回调函数*/
    int             iQueNodeVal;            /*当前队列中的节点数*/
    SysListHead     stList;    
    pthread_mutex_t QueMutex;
    
}__attribute__ ((__packed__))TWaitQue ,*PTWaitQue;

#endif

