/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_serial_qr.c
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
#include"tm_debug.h"
#include"tm_decoler.h"
#include"tm_serial_qr.h"
#include"tm_serial_base.h"
#include"tm_circle_buffer.h"

/*二维码处理流程*/
extern  int  Qrcode_Process_Fun  (unsigned  char *buf,int  len);

/*****************************************************************************
 * Function      : Qrcode_Process_Probe
 * Description   : 二维码设备枚举函数
 * Input         : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180626
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Qrcode_Process_Probe(PT_Serial_Dev Pdev)
{
	return   SUCESS;
}
/*****************************************************************************
 * Function      : Qrcode_Process_Reset
 * Description   : 二维码设备重启
 * Input         : struct  PT_Serial_Dev  Pdev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180626
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Qrcode_Process_Reset(PT_Serial_Dev  Pdev)
{
	return  SUCESS;
}
/*****************************************************************************
 * Function      : Qrcode_Process_Ymode
 * Description   : 二维码设备执行Ymodem
 * Input         : struct  PT_Serial_Dev  Pdev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180626
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Qrcode_Process_Ymode(PT_Serial_Dev  Pdev)
{
	return   SUCESS;
}
/*****************************************************************************
 * Function      : Qrcode_Process_Idle
 * Description   : 处理二维码读头空闲
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180626
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Qrcode_Process_Idle(PT_Serial_Dev  Pdev)
{
    pr_info("Qr code  idle %d",GetSysTimer32());
	return  SUCESS;
}
/*****************************************************************************
 * Function      : Qrcode_Process_Init
 * Description   : 设备初始化
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180626
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Qrcode_Process_Init(PT_Serial_Dev Pdev)
{
    unsigned char Prifix[]={0x02,0x00};
	Serial_Init (Pdev);
	Normal_Protocal_init(Pdev->Pst_decoler,Prifix,2,6,512);
}
/*****************************************************************************
 * Function      : Serial_Qr_Read
 * Description   : 二维码数据读取
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180612
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int  Qrcode_Process_Read  (PT_Serial_Dev Pdev)
{   

    uint16     Qrlen = 0;
    uint8      QrBuffer[1024]= {0};
	PT_Decoler QrDecoler = Pdev->Pst_decoler;

    Qrlen  =   read(Pdev->Serial_Fd,QrBuffer,1024);

    if(!Qrlen)
       return   ERROR;
    
    if(QrDecoler->Decoler_Push(QrDecoler,QrBuffer ,Qrlen) == ERROR )
       return   ERROR;


    for(;;)
    {
		if(QrDecoler->Decoler_POP(QrDecoler,QrBuffer,&Qrlen)==ERROR)
			break;
			
        Qrcode_Process_Fun(QrBuffer,Qrlen);
    }
    return     SUCESS;
}
/*****************************************************************************
 * Function      : Qrcode_Send_data
 * Description   : 二维码设备发送数据无发送队列
 * Input         : PT_Serial_Dev  Pdev  
                unsigned  char *buf  
                int  len             
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int  Qrcode_Send_data(PT_Serial_Dev  Pdev,unsigned  char *buf,int  len)
{
	return  Serial_Write(Pdev,buf,len);
}


/*二维码数据缓冲区*/
static  TCIRCLE_QUENE  st_Qr_CircleQue; 

/*二维码数据检包器*/
static  T_Decoler      St_Qr_Decoler  = 
{
   .Decoler_POP  = Normal_Protocal_POP ,
   .Decoler_Push = Normal_Protocal_Push,
   .decoler_circle_buffer = &st_Qr_CircleQue,
};


/*二维码数据函数操作结构*/
static  TSerial_Dev     t_Qr_Dev     =
{
	.Serial_Fd       =  -1, 
	.Serial_Tag      =  -1,
    .Serial_Name     =  "qrcode" ,     
    .Serial_Port     =  "/dev/ttymxc6",
    .Crc_Enable      =  'n',
    .DataBit         =  8,
    .StopBit         =  1,
    .BoardRate       =  B115200,
    .Serial_systime  =  0,
    .Serial_idletime =  5000,
    .Pst_decoler     =  &St_Qr_Decoler  ,
    .Serial_SendData =  Qrcode_Send_data,            
    .Serial_Init     =  Qrcode_Process_Init  ,            
    .Serial_Reset    =  Qrcode_Process_Reset ,             
    .Serial_Ymodem   =  Qrcode_Process_Ymode ,  
	.Serial_Probe    =  Qrcode_Process_Probe ,                          
	.Serial_Idle     =  Qrcode_Process_Idle  ,  
    .Serial_Read     =  Qrcode_Process_Read  ,                           
};
/*****************************************************************************
 * Function      : Serial_Qr_Reg
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
void  Serial_Qr_Reg()
{
    Serial_Dev_Reg(&t_Qr_Dev);
}

