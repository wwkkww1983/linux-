/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_serial_card.c
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
#include"tm_serial_base.h"
#include"tm_circle_buffer.h"
/*****************************************************************************
 * Function      : 卡机数据初始化
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static	int  Serial_Card_Init  (PT_Serial_Dev Pdev)
{
    int8   Prifix[] = {0x40,0x40};
    if(Serial_Init(Pdev) ==ERROR)
    {
       return  ERROR;
    }
    if(Normal_Protocal_init(Pdev->Pst_decoler,Prifix,2,0,1024)==ERROR)
    {
      return  ERROR;
    }
    return     SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Card_Read
 * Description   : 卡机读取函数
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Card_Read  (PT_Serial_Dev Pdev) 
{  
    int8  i;
    int8  iCardLen;
    int8  iCardBuflen       = 0;
    int8  aucCardBuf[1024]  = {0};
    int8  aucCardData[1024] = {0};
    PT_Decoler PstDecoler   =  Pdev->Pst_decoler;
    
	iCardLen = read(Pdev->Serial_Fd,aucCardData,1024);
	
	if(PstDecoler->Decoler_Push(PstDecoler,aucCardData,iCardLen)==ERROR)
	      return  ERROR;

	for(;;) 
	{
       if(PstDecoler->Decoler_POP(PstDecoler,aucCardBuf,&iCardBuflen)== ERROR)
          break;

       for(i = 0;i < iCardBuflen ;i++)
    		printf("%02x ",aucCardBuf[i]);
    		
    	printf("\n");
	}
	return   SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Serial_Idl
 * Description   : 卡机空闲处理函数
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Card_Idl (PT_Serial_Dev Pdev) 
{
     return   SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Card_Prob
 * Description   : 卡机枚举函数
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Card_Prob  (PT_Serial_Dev Pdev)
{
    return   SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Card_Ymode
 * Description   : YMODEM 系诶处理
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Card_Ymode (PT_Serial_Dev Pdev)
{
    return   SUCESS;
}
/*****************************************************************************
 * Function      : 卡机设备复位操作
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Card_Reset (PT_Serial_Dev Pdev)
{
   	return   SUCESS;
}
/*****************************************************************************
 * Function      : 卡机设备查询操作
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Card_Inqury(PT_Serial_Dev Pdev)
{
    return   SUCESS;
}
/*****************************************************************************
 * Function      : 卡机数据发送函数
 * Description   : 
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Card_Send  (PT_Serial_Dev Pdev,unsigned  char *buf,int  len)
{
   return   SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Card_Proc
 * Description   : 卡机处理函数
 * Input         : PT_Serial_Dev Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Card_Proc   (PT_Serial_Dev Pdev,unsigned  char *buf,int len)
{
   	return   SUCESS;
}

/*卡机读取缓冲区*/
static  TCIRCLE_QUENE  Card_Circle_Buf;


/*卡机数据检包器*/
static  T_Decoler      Card_decoler = 
{
	.Decoler_POP  = Normal_Protocal_POP,
    .Decoler_Push = Normal_Protocal_Push,
    .decoler_circle_buffer = &Card_Circle_Buf,
};

/*读卡器函数接口数据读取定义*/
TSerial_Dev  t_Card_Dev =
{
	.Serial_Fd       = -1, 
	.Serial_Tag      = -1,
    .Serial_Name     =  "card" ,     
    .Serial_Port     =  "/dev/ttymxc1",
    .Crc_Enable      =  'n',
    .BoardRate       =  B115200,
    .DataBit         =  8,
    .StopBit         =  1,
    .Serial_systime  =  0,  
    .Serial_idletime =  5000,   
    .Serial_Init     =  Serial_Card_Init  ,            
    .Serial_Reset    =  Serial_Card_Reset ,             
    .Serial_Inqury   =  Serial_Card_Inqury, 
    .Serial_Ymodem   =  Serial_Card_Ymode ,  
    .Serial_SendData =  Serial_Card_Send  ,            
	.Serial_Probe    =  Serial_Card_Prob  ,                          
	.Serial_Read     =  Serial_Card_Read  ,                           
	.Serial_Idle     =  Serial_Card_Idl   ,  
	.Pst_decoler     =  &Card_decoler,
};
/*****************************************************************************
 * Function      : Serial_Card_Reg
 * Description   : 读卡设备注册
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180611
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void   Serial_Card_Reg()
{
	 Serial_Dev_Reg(&t_Card_Dev);
}
