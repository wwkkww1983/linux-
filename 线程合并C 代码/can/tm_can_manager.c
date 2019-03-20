/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_can_manager.c
 * Author        : xundanqing
 * Date          : 2018-06-13
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-06-13
 *   Author      : xundanqing
 *   Modification: Created file

*************************************************************************************************************/
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<termios.h>
#include<net/if.h>
#include<pthread.h>
#include<linux/can.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include"tm_decoler.h"
#include"tm_can_base.h"
#include"tm_sys_list.h"

/*CAN ����ͷ*/
SYS_LIST_HEAD(g_CanHead);

/*****************************************************************************
 * Function      : Can_Device_Init
 * Description   : CAN �豸ע��ӿ�
 * Input         : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_Device_Init()
{
	 Can_Car_Reg();
}
/*****************************************************************************
 * Function      : Can_Devive_Reg
 * Description   : CAN �豸ע��ȫ������
 * Input         : struct  can_dev * Pdev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_Devive_Reg(struct  can_dev * Pdev)
{
    SYS_ListAdd_Tail(&(Pdev->stCanlist),&g_CanHead);
}
/*****************************************************************************
 * Function      : Serial_Dev_Poll_init
 * Description   : �豸��ѯ��ʼ����ʼ�����ɹ��������г�ʼ��
 * Input          : None
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int  Can_Dev_Poll_init()
{
    struct  can_dev * stCanDev;
    SYS_LIST_FOR_EACH_ENTRY(stCanDev, &g_CanHead, stCanlist)
    {
		if(stCanDev&&(stCanDev->Device_Tag==-1))
           stCanDev->Device_Init(stCanDev);
    }
    return  SUCESS;
}
/*****************************************************************************
 * Function      : Can_FdSets
 * Description   : ����Can  ��ȡFd ����
 * Input         : fd_set *rfdset  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180622
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  _Can_FdSets(fd_set *rfdset)
{
    int  iFdmax;
    
    struct  can_dev *  stCanDev;
    
    SYS_LIST_FOR_EACH_ENTRY(stCanDev, &g_CanHead, stCanlist)
    {
		if(stCanDev&&stCanDev->Device_Tag)
		{
			 FD_SET (stCanDev->Device_fd,rfdset);
			 
	         if(stCanDev->Device_fd  > iFdmax)
     		    iFdmax = stCanDev->Device_fd;
		}
    }

    iFdmax++;
    return  iFdmax;
}
/*****************************************************************************
 * Function      : _Can_FdIdle
 * Description   : Can �豸���д���
 * Input         : fd_set *rfdset  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180622
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   void  _Can_FdIdle(fd_set *rfdset)
{
    struct  can_dev *  stCanDev;
    
    SYS_LIST_FOR_EACH_ENTRY(stCanDev, &g_CanHead, stCanlist)
    {
    	if(stCanDev->Device_Inqury)
    	   stCanDev->Device_Inqury(stCanDev);
    }
}
/*****************************************************************************
 * Function      : _Can_FdRead
 * Description   : can �豸��ȡ����
 * Input         : fd_set *rfdset  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180622
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  void   _Can_FdRead(fd_set *rfdset)
{
    struct  can_dev *  stCanDev;
    
    SYS_LIST_FOR_EACH_ENTRY(stCanDev, &g_CanHead, stCanlist)
    {
		 if(FD_ISSET(stCanDev->Device_fd,rfdset))
		    stCanDev->Device_Read(stCanDev);

          else
            stCanDev->Device_Inqury(stCanDev);
    }
}
/*****************************************************************************
 * Function      : Can_dev_Select
 * Description   : can �豸��ȡSELECT
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_dev_Select(int  can_time)
{
	int     ret;   
 	int     iSer   = 0;
 	int     iFdmax = 0;
    fd_set  rfdset;
    struct  timeval tv;

    tv.tv_sec  =  0;
    tv.tv_usec =  can_time*1000;
    
    FD_ZERO(&rfdset);
    iFdmax = _Can_FdSets(&rfdset);
    
    ret= select((iFdmax+1),&rfdset,NULL,NULL,&tv);

    if(ret > 0)
      _Can_FdRead(&rfdset);  

    if(ret ==0)
      _Can_FdIdle(&rfdset);
     
    return  SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Dev_Poll_Read
 * Description   : �豸��ѯ��ȡ
 * Input         : None
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int  Can_Dev_Poll_Read()
{
	return  Can_dev_Select (CAN_SELCT_TIME);
}
/*****************************************************************************
 * Function      : Serial_Deal_Fun
 * Description   : ͨ�õĴ��ڴ�����
 * Input         : void *arg  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void   *Can_Deal_Fun(void *arg)
{
    while(1)
    {  
       Can_Dev_Poll_init();
       Can_Dev_Poll_Read();
	}
}
/*****************************************************************************
 * Function      : Serial_Dev_Start
 * Description   : �����豸����
 * Input         : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_Dev_Start()
{
    static  pthread_t can_thread_fd;
	return  pthread_create(&can_thread_fd,NULL,Can_Deal_Fun,NULL);
}




