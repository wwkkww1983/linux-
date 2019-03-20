#include "tm_decoler.h"
#include "tm_sys_timer.h"
#include "tm_sys_waitquene.h"
/*****************************************************************************
 * Function      : GernerQueNode
 * Description   : 根据命令生成一个超时节点
 * Input         : tPackInfo   Packinfo  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180709
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
PTQueNode   GernerQueNode(tPackInfo   Packinfo)
{ 

    WaitQueKey waitkey;
	PTQueNode  Pwaitnode = (PTQueNode)malloc(sizeof(TQueNode)+Packinfo.Bufferlen);
	
	if(Pwaitnode == NULL)
	{
       return  NULL;
	}
	
	waitkey.stKey.cmd =  Packinfo.iCmd;
	waitkey.stKey.seq =  Packinfo.iSeq;
	Pwaitnode->iNodeKey  =  waitkey.iKey;
	Pwaitnode->iBufferlen=  Packinfo.Bufferlen;
    Pwaitnode->aucBuffer = ((int8*)Pwaitnode)+sizeof(TQueNode);
    memcpy(Pwaitnode->aucBuffer,Packinfo.aucBuffer,Packinfo.Bufferlen);
    
    return   Pwaitnode;
    
}
/*****************************************************************************
 * Function      : GetWaitQueHead
 * Description   : 获得队列的头节点主意此时已经加锁
 * Input         : PTWaitQue pstQue  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180707
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
PTQueNode  GetWaitQueHead(PTWaitQue pstQue)
{
    TQueNode *Pnode  = NULL;
    
    Pnode =  SYS_LIST_ENTRY(pstQue->stList.pstNext,TQueNode, stList);
    
    return   Pnode;
}
/*****************************************************************************
 * Function      : QueNodeProc
 * Description   : 节点超时判断并处理
 * Input         : PTQueNode pstNode  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180707
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int    QueNodeProc(PTQueNode pstNode , PTWaitQue pstQue)
{

	  pstNode->iWaitTime -= WAITQUE_TIME;
	  
	  if(pstNode->iWaitTime  <=  0)
      {
            pstQue->QueAtribute.QueSendCallBack \
                         (pstNode->aucBuffer,pstNode->iBufferlen);
            pstNode->iTransval--;
        
         if(pstNode->iTransval)
            pstNode->iWaitTime =  pstQue->QueAtribute.iQueWarnVal ;

         else
         {   
            Mutex_Lock (&(pstQue->QueMutex));
            SYS_ListDel(&(pstNode->stList) );
            pstQue->iQueNodeVal--;
            Mutex_UnLock(&(pstQue->QueMutex));
         }
     }
     return   SUCESS;
}
/*****************************************************************************
 * Function      : QueWaitTimerFun
 * Description   : 
 * Input         : void *arg  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180707
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int   QueWaitTimerFun(void *arg)
{

    PTQueNode pstQueNode =  NULL;
    PTWaitQue pStWaitQue = (PTWaitQue)arg;
    Mutex_Lock(&(pStWaitQue->QueMutex));
    if(SYS_ListEmpty (pStWaitQue->stList ) == SYS_TRUE)	 
    {
       Mutex_UnLock(&(pStWaitQue->QueMutex));
       return  0;
    }
    pstQueNode =  GetWaitQueHead(pStWaitQue);
    if(pstQueNode == NULL)
    {
	   Mutex_UnLock(&(pStWaitQue->QueMutex));
       return  0;	
    }
    QueNodeProc(pstQueNode,pStWaitQue);
    Mutex_UnLock(&(pStWaitQue->QueMutex));
    return     0;
    
}
/*****************************************************************************
 * Function      : QueWaitInser
 * Description   : 插入超时的缓冲队列插入的同时进行阈值判断进行阈值提醒
 * Input         : PTQueNode  PstNode     
                PTWaitQue  PstWaitQue  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180709
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int    QueWaitAddTail(PTQueNode  PstNode,PTWaitQue  PstWaitQue)
{

	Mutex_Lock(&(PstWaitQue->QueMutex));
	SYS_ListAdd_Tail(&(PstNode->stList),&(PstWaitQue->stList));
    PstWaitQue->iQueNodeVal++;
    Mutex_UnLock(&(PstWaitQue->QueMutex));
    
    if(PstWaitQue->iQueNodeVal > PstWaitQue->QueAtribute.iQueWarnVal)
    {
       PstWaitQue->QueAtribute.QueWarnCallBack();
    }
    return   SUCESS;
    
}
/*****************************************************************************
 * Function      : QueWaitAlloc
 * Description   : 超时发送队列申请
 * Input         : PTQueAttribute Queattribute  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180707
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
PTWaitQue    QueWaitAlloc(TQueAtrribute * Queattribute)
{
    PTWaitQue    pStWaitQue  = NULL;
    pStWaitQue = (PTWaitQue)malloc(sizeof(TWaitQue));
   
    if(pStWaitQue == NULL)
    {
        return  ERROR;
    }

    if(pthread_mutex_init (&(pStWaitQue->QueMutex),NULL) < 0)
    {
        return  ERROR;
    }

    SYS_LIST_HEAD_INIT (pStWaitQue->stList);
    pStWaitQue->QueAtribute.iQueSendVal =  Queattribute->iQueSendVal;
    pStWaitQue->QueAtribute.iQueWaitVal =  Queattribute->iQueWaitVal;
    pStWaitQue->QueAtribute.iQueWarnVal =  Queattribute->iQueWarnVal;
    pStWaitQue->QueAtribute.QueSendCallBack = Queattribute->QueSendCallBack;
    pStWaitQue->QueAtribute.QueWarnCallBack = Queattribute->QueWarnCallBack;
    SYS_TimerAdd(WAITQUE_TIME,1,QueWaitTimerFun,&(pStWaitQue->QueTimeId),TIMER_ROUTINE);
   
    return   pStWaitQue;
}



