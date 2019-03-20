/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_serial_gps.c
 * Author        : xundanqing
 * Date          : 2018-06-12
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-06-12
 *   Author      : xundanqing
 *   Modification: Created file

*************************************************************************************************************/
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<termios.h>
#include"tm_decoler.h"
#include"tm_serial_base.h"
#include"tm_circle_buffer.h"
/*****************************************************************************
 * Function      : GPS设备初始化
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static	int  Serial_GPs_Init (PT_Serial_Dev Pdev)
{
	return   Serial_Init(Pdev);
}
/*****************************************************************************
 * Function      : GPS数据读取函数
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_GPs_Read  (PT_Serial_Dev Pdev)  
{
   int  i;
   int  GPs_len;
   unsigned  char GPs_Buffer[1024] = {0};

   GPs_len = read(Pdev->Serial_Fd,GPs_Buffer,1024);
   for(i = 0;i < GPs_len;i++)
       printf("%c",GPs_Buffer[i]);
}
/*****************************************************************************
 * Function      : GPS设备空闲处理
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_GPs_Idl   (PT_Serial_Dev Pdev)
{
	return  SUCESS;
}
/*****************************************************************************
 * Function      : GPS设备枚举
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_GPs_Prob  (PT_Serial_Dev Pdev)
{

	return  SUCESS;
}
/*****************************************************************************
 * Function      : Serial_GPs_Ymode
 * Description   : GPS Ymodem通信协议
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_GPs_Ymode (PT_Serial_Dev Pdev)
{
	return  SUCESS;
}
/*****************************************************************************
 * Function      : GPS 设备复位操作
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_GPs_Reset (PT_Serial_Dev Pdev)
{
	return  SUCESS;
}
/*****************************************************************************
 * Function      : Serial_GPs_Inqury
 * Description   : GPS设备查询
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_GPs_Inqury(PT_Serial_Dev Pdev)
{
	return  SUCESS;
}
/*****************************************************************************
 * Function      : Serial_GPs_Proc
 * Description   : GPS数据处理
 * Input         : PT_Serial_Dev Pdev   
                unsigned  char *buf  
                int len              
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_GPs_Proc  (PT_Serial_Dev Pdev,unsigned  char *buf,int len)
{
	return  SUCESS;
}
/*****************************************************************************
 * Function      : Serial_GPs_Send
 * Description   : GPS设备发送处理
 * Input         : PT_Serial_Dev Pdev   
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
static  int  Serial_GPs_Send  (PT_Serial_Dev Pdev,unsigned  char *buf,int  len)
{
	return  SUCESS;
}

/*GPS 模块设备处理函数接口*/
static  TSerial_Dev  t_GPs_Dev =
{
	.Serial_Fd       =  -1, 
	.Serial_Tag      =  -1,
    .Serial_Name     =  "GPs" ,     
    .Serial_Port     =  "/dev/ttymxc4",
    .Crc_Enable      =  'n',
    .DataBit         =  8,
    .StopBit         =  1,
    .BoardRate       =  B9600,
    .Serial_Init     =  Serial_GPs_Init  ,            
    .Serial_Reset    =  Serial_GPs_Reset ,             
    .Serial_Inqury   =  Serial_GPs_Inqury, 
    .Serial_Ymodem   =  Serial_GPs_Ymode ,  
    .Serial_SendData =  Serial_GPs_Send  ,            
	.Serial_Probe    =  Serial_GPs_Prob  ,                          
	.Serial_Read     =  Serial_GPs_Read  ,                           
	.Serial_Idle     =  Serial_GPs_Idl   ,  
};
/*****************************************************************************
 * Function      : Serial_GPs_Reg
 * Description   : GPS设备注册函数
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void  Serial_GPs_Reg()
{
	  Serial_Dev_Reg(&t_GPs_Dev);
}
