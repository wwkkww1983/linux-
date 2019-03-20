/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_socket_base.c
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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include "tm_decoler.h"
#include "tm_socket_base.h"
/*****************************************************************************
 * Function      : Connect_SocketUn_Server
 * Description   : 连接UNIX域服务器
 * Input         : TSockAddrUn tSvrAddr  
                int *piClientFd       
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180626
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int Connect_SocketUn_Server(TSockAddrUn tSvrAddr, int *piClientFd)
{
    int iFd;
    int iRet;
    int iLen;
    int iFlag;
    
    TTimeVal  tTimeVal;
    TSoLinger tSoLinger;

    iFd = socket(AF_UNIX, SOCK_STREAM, 0);

    tTimeVal.tv_sec  = 2;
    tTimeVal.tv_usec = 0;
    setsockopt(iFd, SOL_SOCKET, SO_SNDTIMEO, (char *)(&tTimeVal), sizeof(tTimeVal));

    tTimeVal.tv_sec  = 2;
    tTimeVal.tv_usec = 0;
    setsockopt(iFd, SOL_SOCKET, SO_RCVTIMEO, (char *)(&tTimeVal), sizeof(tTimeVal));

    iFlag = 1;
    setsockopt(iFd, IPPROTO_TCP, TCP_NODELAY, (char *)(&iFlag), sizeof(iFlag));

    tSoLinger.l_onoff = 1;
    tSoLinger.l_linger = 0;
    setsockopt(iFd, SOL_SOCKET, SO_LINGER, (char*)(&tSoLinger),sizeof(tSoLinger));

    iLen = sizeof(tSvrAddr.sun_family) + strlen(tSvrAddr.sun_path);

    iRet = connect(iFd, (struct sockaddr *)&tSvrAddr, iLen);

    if(0 == iRet)
    {
        *piClientFd = iFd;
         return  SUCESS;
    }
    else
    {
        close(iFd);
        return   ERROR;
    }
}
/*****************************************************************************
 * Function      : Connect_SocketiP_Server
 * Description   : 连接IP服务器
 * Input         : TSockAddrIn tSvrAddr  
                int *piCilentFd       
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180626
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int Connect_SocketiP_Server(TSockAddrIn tSvrAddr, int *piCilentFd)
{
    int iRet;
    int iFd;
    int iFlag = 1;

    TSoLinger tSoLinger;
    TTimeVal  tTimeVal;

    iFd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(iFd, IPPROTO_TCP, TCP_NODELAY, (char *)&iFlag, sizeof(iFlag));

    tTimeVal.tv_sec  = 2;
    tTimeVal.tv_usec = 0;
    setsockopt(iFd, SOL_SOCKET, SO_SNDTIMEO, (char *)(&tTimeVal), sizeof(tTimeVal));

    tTimeVal.tv_sec  = 2;
    tTimeVal.tv_usec = 0;
    setsockopt(iFd, SOL_SOCKET, SO_RCVTIMEO, (char *)(&tTimeVal), sizeof(tTimeVal));

    tSoLinger.l_onoff  = 1;
    tSoLinger.l_linger = 0;
    setsockopt(iFd, SOL_SOCKET, SO_LINGER, (char*)(&tSoLinger), sizeof(tSoLinger));

    iRet = connect(iFd, (TSockAddr*)(&tSvrAddr), sizeof(tSvrAddr));
    
    if(0 == iRet)
    {
        *piCilentFd = iFd;
         return  SUCESS;
    }
    else
    {
        close(iFd);
        return   ERROR;
    }
}
/*****************************************************************************
 * Function      : Create_SocketUn_Server
 * Description   : 创建UNSOCKET  server
 * Input         : TSockAddrUn tSvrAddr  
                int *piSvrFd          
                int iLstnNum          
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180626
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int Create_SocketUn_Server(TSockAddrUn tSvrAddr, int *piSvrFd, int iLstnNum)
{
    int iFd;
    int iRet;
    int iFlag = 1;

    iFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (iFd < 0)
        return  ERROR;

    setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, (char *)&iFlag, sizeof(iFlag));

    unlink(tSvrAddr.sun_path);

    iRet = bind(iFd, (TSockAddr *)&tSvrAddr, sizeof(tSvrAddr));
    
    if(-1 == iRet)
        return  ERROR;

    iRet = listen(iFd, iLstnNum);
    
    if (iRet < 0)
        return  ERROR;

    *piSvrFd = iFd;
    return 	 SUCESS;
}


