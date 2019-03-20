/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_socket.h
 * Author        : xundanqing
 * Date          : 2018-06-26
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-06-26
 *   Author      : xundanqing
 *   Modification: Created file

*************************************************************************************************************/

#ifndef  _TM_SOCKET_BASE_H
#define  _TM_SOCKET_BASE_H

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/un.h>
#include<netinet/in.h>
#include"tm_type.h"
#include"tm_sys_list.h"

/**
 *@网络地址定义
 *錶inux  域服务器地址 IP地址
 */
typedef struct timeval      TTimeVal;
typedef struct linger       TSoLinger;
typedef struct sockaddr     TSockAddr;
typedef struct sockaddr_un  TSockAddrUn;
typedef struct sockaddr_in  TSockAddrIn;

/**
 * @网络客户端地址类型
 * 客户端地址类型IP数据文件目录数据
 */
typedef struct  stSocketAddr
{
    int  iSockType;
    TSockAddrIn tSockAddr;   
    TSockAddrUn tSockAddrUn;  
    
} TAddrt,*PTAddrt;
/**
 * @数据客户端数据定义
 *  增加网络连接通路判断来确定建立网络连接
 */
typedef  struct  st_socket_client
{
   int     Socket_fd;        
   int     Socket_Tag;         
   TAddrt  SocketAddr;      
   int     (*Socket_init)   (struct  st_socket_client *stSocket);
   int     (*Socket_Idle)   (struct  st_socket_client *stSocket);
   int     (*Socket_Recv)   (struct  st_socket_client *stSocket);
   int     (*Socket_Connect)(struct  st_socket_client *stSocket);
   int     (*Socket_iSLink )(struct  st_socket_client *stSocket);
   int     (*Socket_Send   )(struct  st_socket_client *stSocket,unsigned   char *aucdata,int ilen);
   SysListHead  SocketList;

}T_SOCKET_CLIENT,*PT_SOCKET_CLIENT;

/**
 * @网络连接管理数据定义
 *  网络连接数据定义网络句柄的select
 */
typedef  struct  st_socket_manager
{
   int     (*SocketFds_Add)    (struct  st_socket_manager*  stSockManager);
   int     (*SocketFds_Connect)(struct  st_socket_manager*  stSockManager);
   int     (*SocketFds_Process)(struct  st_socket_manager*  stSockManager);
   int     (*SocketFds_Read  ) (struct  st_socket_manager*  stSockManager);
   int     (*SocketFds_Idle  ) (struct  st_socket_manager*  stSockManager);
   int     (*SockeFds_ListAdd) (struct  st_socket_client*   stClient,struct  st_socket_manager*  stSockManager);
   int     (*SockeFds_ListDel) (struct  st_socket_client*   stClient,struct  st_socket_manager*  stSockManager);
   SysListHead      SocketList;
   fd_set           Socket_fdsets;
   pthread_mutex_t  Socketfds_Mutex_t;
   
}T_Socket_Mng,*PT_Socket_Mng;

#endif

