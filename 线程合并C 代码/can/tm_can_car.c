/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_can_car.c
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
#include"tm_can_base.h"
#include"tm_decoler.h"

/*CAN 接口调用函数指针*/
static  int    Can_Car_Init(struct can_dev *Pdev);
static  int    Can_Car_Read(struct can_dev *Pdev);
static  int    Can_Car_Inqury(struct can_dev *Pdev);
static  int    Can_Car_Send(struct can_dev *Pdev, unsigned  char *buf,int  len);	 

/*CAN 过滤器设置*/
static  struct can_filter  car_can_filter[] =
{
    {(0xAAA10000>>3),(0XFFFF0000>>3)}
};

/*对接外部车载机结构*/
T_CAN_DEV     Can_car_dev = 
{
    .Device_fd           =-1,
    .Device_Tag          =-1,
    .Device_module_name  = "car",
	.Device_Can_Name     = "can0",
    .Device_filter_num   = 1,
    .Device_filter_table = car_can_filter,
    .Device_Init         = Can_Car_Init,
    .Device_Read         = Can_Car_Read,
    .Device_Send         = Can_Car_Send,
    .Device_Inqury       = Can_Car_Inqury,
};
/*****************************************************************************
 * Function      : Can_Car_Reg
 * Description   : 车载机can对接
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Can_Car_Reg()
{
	return     Can_Devive_Reg(&Can_car_dev);
}
/*****************************************************************************
 * Function      : Can_Car_Init
 * Description   : CAN设备初始化
 * Input         : struct can_dev *Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int   Can_Car_Init(struct can_dev *Pdev)
{
    return    Can_Init(&Can_car_dev);
}
/*****************************************************************************
 * Function      : Can_Car_Inqury
 * Description   : 当前查询数据空
 * Input         : struct can_dev *Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int  Can_Car_Inqury(struct can_dev *Pdev)
{

}
/*****************************************************************************
 * Function      : Can_Car_Read
 * Description   : Can数据读取
 * Input         : struct can_dev *Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int   Can_Car_Read(struct can_dev *Pdev)
{


}
/*****************************************************************************
 * Function      : Can_Car_Send
 * Description   : can 设备发送数据
 * Input         : struct can_dev *Pdev  
                unsigned  char *buf   
                int  len              
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int   Can_Car_Send(struct can_dev *Pdev, unsigned  char *buf,int  len)
{


}


