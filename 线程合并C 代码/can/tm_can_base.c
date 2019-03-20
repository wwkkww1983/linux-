/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_can_base.c
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
#include<net/if.h>
#include<pthread.h>
#include<termios.h>
#include<sys/time.h>
#include<linux/can.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<linux/can/raw.h>
#include"tm_decoler.h"
#include"tm_can_base.h"
/*****************************************************************************
 * Function      : Can_Device_Block
 * Description   : 阻塞CAN设备
 * Input         : struct  can_dev*  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_Device_Block(struct  can_dev* Pdev)
{
   int ret;
   int val = fcntl(Pdev->Device_fd, F_GETFL,0);
   val  &= ~O_NONBLOCK;
   ret=fcntl(Pdev->Device_fd, F_SETFL, val); 	
   return  ret;
}
/*****************************************************************************
 * Function      : Can_Device_Unblock
 * Description   : 设置CAN设备非阻塞
 * Input         : struct  can_dev * Pdev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_Device_Unblock(struct  can_dev * Pdev)
{
   int ret;
   int val = fcntl(Pdev->Device_fd, F_GETFL, 0);
   val  |=   O_NONBLOCK;
   ret  =    fcntl(Pdev->Device_fd, F_SETFL, val); 	
   return  ret;
}
/*****************************************************************************
 * Function      : Can_Device_OPen
 * Description   : 打开CAN设备
 * Input         : struct can_dev *Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int   Can_OPen(struct can_dev *Pdev)
{
      struct ifreq ifr;
	  struct sockaddr_can can_addr;
	  Pdev->Device_fd =  socket(PF_CAN, SOCK_RAW, CAN_RAW);
	  if(Pdev->Device_fd < 0)
		   return  ERROR;

	  can_addr.can_family = PF_CAN;
	  strncpy(ifr.ifr_name, Pdev->Device_Can_Name, sizeof(ifr.ifr_name));
	  if (ioctl( Pdev->Device_fd, SIOCGIFINDEX, &ifr)) 
		 return  ERROR;

	  can_addr.can_ifindex = ifr.ifr_ifindex;
	  if (bind(Pdev->Device_fd, (struct sockaddr *)&can_addr, sizeof(can_addr)) < 0)
		 return  ERROR;  

	  if (Pdev->Device_filter_table)
	  {
		   if (setsockopt(Pdev->Device_fd, SOL_CAN_RAW, CAN_RAW_FILTER, Pdev->Device_filter_table,
					      Pdev->Device_filter_num * sizeof(struct can_filter)) != 0)
			   return  ERROR;
	  }
	  return  SUCESS;
}
/*****************************************************************************
 * Function      : Can_Device_Init
 * Description   : 设备初始化
 * Input         : struct can_dev *Pdev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_Init(struct can_dev *Pdev)
{
	if(Can_OPen(Pdev)==ERROR)	
       return  ERROR;
       
    Pdev->Device_Tag =1;
    
    pthread_mutex_init(&Pdev->Device_mutex,NULL);
    return    SUCESS;
}
/*****************************************************************************
 * Function      : Can_Device_Write
 * Description   : can 设备发送can 数据
 * Input         : struct can_dev *Pdev         
                struct can_frame *Pack_info  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_Write(struct can_dev *Pdev, struct can_frame *Pack_info)
{
	 pthread_mutex_lock (&Pdev->Device_mutex);
     if(Pdev->Device_fd)
     {
         if(write(Pdev->Device_fd,Pack_info,sizeof(struct can_frame)) < 0)
         {
            pthread_mutex_unlock (&Pdev->Device_mutex);
            return   ERROR;
         }
     }
	 pthread_mutex_unlock (&Pdev->Device_mutex);
	 return  SUCESS;
}

