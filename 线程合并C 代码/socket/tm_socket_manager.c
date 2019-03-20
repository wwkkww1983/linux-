/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_socket_manager.c
 * Author        : xundanqing
 * Date          : 2018-07-03
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-07-03
 *   Author      : xundanqing
 *   Modification: Created file

********************************************************************** ***************************************/
#include<pthread.h>
#include"tm_type.h"
#include"tm_decoler.h"
#include"tm_socket_base.h"
/*****************************************************************************
 * Function      : Socket_Client_Add
 * Description   : socket客户端管理添加客户端
 * Input         : T_SOCKET_CLIENT Client  
                PT_Socket_Mng Mng       
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void  Socket_Client_Add(PT_SOCKET_CLIENT Client ,PT_Socket_Mng Mng)
{
	 Mutex_Lock(&(Mng->Socketfds_Mutex_t));
	 SYS_ListAdd_Tail(&(Client->SocketList),&(Mng->SocketList));
     Mutex_UnLock(&(Mng->Socketfds_Mutex_t));
}
/*****************************************************************************
 * Function      : Socket_Client_Del
 * Description   : socket客户端删除
 * Input         : T_SOCKET_CLIENT Client  
                PT_Socket_Mng Mng       
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void   Socket_Client_Del(PT_SOCKET_CLIENT Client ,PT_Socket_Mng Mng)
{
	 Mutex_Lock(&(Mng->Socketfds_Mutex_t));
	 SYS_ListDel(&(Client->SocketList));
     Mutex_UnLock(&(Mng->Socketfds_Mutex_t));
} 
/*****************************************************************************
 * Function      : Socket_Client_IsConnect
 * Description   : 判断客户端是不是已经建立连接
 * Input         : PT_Socket_Mng   Mng  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int   Socket_Client_IsConnect(PT_Socket_Mng   Mng)
{
	 PT_SOCKET_CLIENT pstClient;
	 SYS_LIST_FOR_EACH_ENTRY(pstClient,&(Mng->SocketList),SocketList)
	 {
      	if(!(pstClient->Socket_Tag))
      	    pstClient->Socket_Connect(pstClient);
	 }
	 return  SUCESS;
}
/*****************************************************************************
 * Function      : Socket_ClientFds_Set
 * Description   : 添加网络连接可用句柄
 * Input         : PT_Socket_Mng   Mng  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function
*****************************************************************************/
int   Socket_ClientFds_Set(PT_Socket_Mng   Mng)
{
     int  iSocketFd = 0;
	 PT_SOCKET_CLIENT pstClient = NULL;
	 SYS_LIST_FOR_EACH_ENTRY(pstClient,&(Mng->SocketList),SocketList)
	 {
	 
        if(pstClient->Socket_Tag && pstClient && pstClient->Socket_fd)
            FD_SET (pstClient->Socket_fd,&(Mng->Socket_fdsets));


        if(pstClient->Socket_fd >  iSocketFd)
           iSocketFd =  pstClient->Socket_fd;
           
	 }
	 iSocketFd++;
	 return  iSocketFd;
}  
/*****************************************************************************
 * Function      : Socket_Client_Read
 * Description   : 网络客户端读取数据
 * Input         : PT_Socket_Mng   Mng  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Socket_Client_Read (PT_Socket_Mng   Mng)
{
     PT_SOCKET_CLIENT pstClient = NULL;
	 SYS_LIST_FOR_EACH_ENTRY(pstClient,&(Mng->SocketList),SocketList)
	 {
  	     if(FD_ISSET(pstClient->Socket_fd,&(Mng->Socket_fdsets)))
   	        pstClient->Socket_Recv(pstClient);
     }
     return  SUCESS;
}
/*****************************************************************************
 * Function      : Socket_Client_Read
 * Description   : 网络客户端空闲处理
 * Input         : PT_Socket_Mng   Mng  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int   Socket_Client_Idle(PT_Socket_Mng   Mng)
{
	 PT_SOCKET_CLIENT  pstClient = NULL;
	 SYS_LIST_FOR_EACH_ENTRY(pstClient,&(Mng->SocketList),SocketList)
	 {
   	    pstClient->Socket_Idle(pstClient);
     }
     return  SUCESS;
}
/*****************************************************************************
 * Function      : Socket_Client_Read
 * Description   : 网络客户端读取函数
 * Input         : PT_Socket_Mng   Mng  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int   Socket_Client_Process (PT_Socket_Mng   Mng)
{
     int     iret = 0;
     int     iSocketFd = 0;

     struct  timeval tv;
 
     tv.tv_sec  =  0;
     tv.tv_usec =  10000;
 
     FD_ZERO(&(Mng->Socket_fdsets));
     Mng->SocketFds_Connect(Mng);
   
     iSocketFd =  Mng->SocketFds_Add(Mng);

     if(iSocketFd == 1)
     {
		sleep(1);
		return  ERROR;
     }
     
     iret= select(iSocketFd,&(Mng->Socket_fdsets),NULL,NULL,&tv);
   
     if(iret> 0)
       Mng->SocketFds_Read(Mng);

     else
       Mng->SocketFds_Idle(Mng);
     
     return SUCESS;
}
/*****************************************************************************
 * Function      : Socket_Client_Read
 * Description   : 网络客户端管理类初始化
 * Input         : PT_Socket_Mng   Mng  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   T_Socket_Mng   stClient_Mng =
{
	.SockeFds_ListAdd  = Socket_Client_Add,
	.SockeFds_ListDel  = Socket_Client_Del,
	.SocketFds_Read    = Socket_Client_Read,
	.SocketFds_Idle    = Socket_Client_Idle,
    .SocketFds_Add     = Socket_ClientFds_Set,
    .SocketFds_Connect = Socket_Client_IsConnect,
    .SocketFds_Process = Socket_Client_Process,
};   
/*****************************************************************************
 * Function      : Mng_Client_Add
 * Description   : 添加管理客户端外部接口
 * Input         : PT_SOCKET_CLIENT Client  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180704
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void   Client_Mng_Add (PT_SOCKET_CLIENT Client)
{
	  Socket_Client_Add(Client,&stClient_Mng);
}
/*****************************************************************************
 * Function      : Mng_Client_Del
 * Description   : 网络管理客户端删除外部接口
 * Input         : PT_SOCKET_CLIENT Client  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180704
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void   Client_Mng_Del(PT_SOCKET_CLIENT Client)
{
     Socket_Client_Del(Client,&stClient_Mng);
}
/*****************************************************************************
 * Function      : Client_Thread_Fun
 * Description   : 网络客户端管理线程
 * Input         : void *arg  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void   *Client_Thread_Fun(void *arg)
{
   while(1)
   {
      stClient_Mng.SocketFds_Process(&stClient_Mng);
   }
}
/*****************************************************************************
 * Function      : Client_Mng_Start
 * Description   : 开启网络客户端管理
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180703
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Client_Mng_Process()
{

    pthread_t  stMng_t;
    
	SYS_LIST_HEAD_INIT(stClient_Mng.SocketList);

    if(pthread_mutex_init(&stClient_Mng.Socketfds_Mutex_t,NULL)< 0)
    {
       return  ERROR;
    }
    
	if(pthread_create (&stMng_t,NULL,Client_Thread_Fun,NULL)  < 0)
	{
	    return  ERROR;
    }
    
	return  SUCESS;
}


