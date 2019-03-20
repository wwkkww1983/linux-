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
 * Function      : GPS�豸��ʼ��
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
 * Function      : GPS���ݶ�ȡ����
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
 * Function      : GPS�豸���д���
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
 * Function      : GPS�豸ö��
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
 * Description   : GPS Ymodemͨ��Э��
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
 * Function      : GPS �豸��λ����
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
 * Description   : GPS�豸��ѯ
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
 * Description   : GPS���ݴ���
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
 * Description   : GPS�豸���ʹ���
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

/*GPS ģ���豸�������ӿ�*/
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
 * Description   : GPS�豸ע�ắ��
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
