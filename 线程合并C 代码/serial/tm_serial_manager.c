/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_serial_manager.c
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
#include<unistd.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>
#include"tm_decoler.h"
#include"tm_serial_base.h"
#include"tm_serial_manager.h"
/*****************************************************************************
 * Function      : GetSysTimer32
 * Description   : ϵͳʱ����
 * Input          : None
 * Output        : None
 * Return        : unsigned
 * Others        : 
 * Record
 * 1.Date        : 20180625
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
unsigned  int   GetSysTimer32()
{

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    
}
/*****************************************************************************
 * Function      : Serial_TimeOut_Check
 * Description   : ��ʱʱ���жϷ�ֹʱ�����
 * Input         : struct  TSerial_Dev* Pdev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180625
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int   Serial_TimeOut_Check(PT_Serial_Dev  Pdev)
{

   if(GetSysTimer32() > Pdev->Serial_systime )
   	  return  (GetSysTimer32()-Pdev->Serial_systime \
   	                                     > Pdev->Serial_idletime);
   	  
   else  
   	  return  ((SERTIME_CLOCK_OUT + GetSysTimer32() \
   	                                    - Pdev->Serial_systime) \
   	                                     >Pdev->Serial_idletime);
   	                                     
}
/*****************************************************************************
 * Function      : Serial_Avaial_Check
 * Description   : ����״̬���
 * Input         : PT_Serial_Mng  Mng  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static int  Serial_Avaial_Check(PT_Serial_Mng  Mng)
{

   PT_Serial_Dev  pstSerialdev;
   SYS_LIST_FOR_EACH_ENTRY(pstSerialdev,&(Mng->stListSerial),stListSerial)
   {
	   if(pstSerialdev&&( pstSerialdev->Serial_Tag == -1))
	   {
       	   if(pstSerialdev->Serial_Init(pstSerialdev)==SUCESS)
       	   {
              pstSerialdev->Serial_Probe(pstSerialdev);
       	   }
       }
   }
   return  SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Fds_Add
 * Description   : ���þ�����
 * Input         : PT_Serial_Mng  Mng  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static int  Serial_Fds_Add(PT_Serial_Mng  Mng)
{

	int  iSerialMaxFd = 0;
    PT_Serial_Dev  pstSerialdev;
    SYS_LIST_FOR_EACH_ENTRY(pstSerialdev,&(Mng->stListSerial),stListSerial)
    {
     	if(pstSerialdev&&pstSerialdev->Serial_Tag&&!(pstSerialdev->Serial_Ymod))
     	{
     	
		   FD_SET (pstSerialdev->Serial_Fd,&(Mng->SerialFds));

           if(pstSerialdev->Serial_Fd > iSerialMaxFd)
              iSerialMaxFd =  pstSerialdev->Serial_Fd;
     	}
    }
    iSerialMaxFd++;
    return  iSerialMaxFd;
    
}
/*****************************************************************************
 * Function      : Serial_Fds_Read
 * Description   : ���ھ���Ķ�ȡ
 * Input         : PT_Serial_Mng  Mng  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Fds_Read(PT_Serial_Mng  Mng)
{
   PT_Serial_Dev  pstSerialdev;
   SYS_LIST_FOR_EACH_ENTRY(pstSerialdev,&(Mng->stListSerial),stListSerial)
   {
   
   	   if(FD_ISSET(pstSerialdev->Serial_Fd,&(Mng->SerialFds)))
   	   {
			pstSerialdev->Serial_Read(pstSerialdev);
			pstSerialdev->Serial_systime  = GetSysTimer32();
   	   }
   	   else
   	   {
			if( Serial_TimeOut_Check(pstSerialdev))
			{
                pstSerialdev->Serial_Idle(pstSerialdev);
                pstSerialdev->Serial_systime = GetSysTimer32 (); 
                if(pstSerialdev->Serial_heart_link > SERLINKMAX)
                {
         	        pstSerialdev->Serial_Reset(pstSerialdev);
                }
			}
   	   }
   }	
   return  SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Fds_Idle
 * Description   : �����豸���д���
 * Input         : PT_Serial_Mng  Mng  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int  Serial_Fds_Idle(PT_Serial_Mng  Mng)
{

   PT_Serial_Dev pstSerialdev;
   SYS_LIST_FOR_EACH_ENTRY(pstSerialdev,&(Mng->stListSerial),stListSerial)
   {
   
   	  if(Serial_TimeOut_Check(pstSerialdev))
   	  {
   	  
         pstSerialdev->Serial_Idle(pstSerialdev);
         pstSerialdev->Serial_systime  =  GetSysTimer32(); 
         pstSerialdev->Serial_heart_link++;
         
         if(pstSerialdev->Serial_heart_link > SERLINKMAX)
         {
         	pstSerialdev->Serial_Reset(pstSerialdev);
         }
         
   	  }
   }
   return  SUCESS;
   
}
/*****************************************************************************
 * Function      : Serial_Fds_Proc
 * Description   : ���ڹ���������
 * Input         : PT_Serial_Mng  Mng  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static int  Serial_Fds_Proc(PT_Serial_Mng  Mng)
{
	
     int     iret = 0;
     int     iSocketFd = 0;

     struct  timeval tv;
     
     tv.tv_sec  =  0;
     tv.tv_usec =  10000;
 
     do
     {
         FD_ZERO(&(Mng->SerialFds));
         Mng->Serial_IsAvail(Mng);
         iSocketFd =  Mng->Serial_FdsAdd(Mng);
         if(iSocketFd == 1)
         {
	    	sleep(1);
		    continue;    
		 }
		
     }while(0);
    
     iret= select(iSocketFd,&(Mng->SerialFds),NULL,NULL,&tv);
   
     if(iret> 0)
         Mng->Serial_FdsRead(Mng);
     
     else
         Mng->Serial_FdsIdle(Mng);
     
     return SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Mng
 * Description   : ���ڹ���ȫ�ֱ�������
 * Input          : None
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   T_Serial_Mng   Serial_Mng =
{
    .Serial_FdsAdd  =  Serial_Fds_Add ,
    .Serial_FdsRead =  Serial_Fds_Read,
    .Serial_FdsIdle =  Serial_Fds_Idle,
    .Serial_FdsProc =  Serial_Fds_Proc,
    .Serial_IsAvail =  Serial_Avaial_Check,
};
/*****************************************************************************
 * Function      : Serial_Dev_Reg
 * Description   : ��Ӵ��ڶ���
 * Input         : PT_Serial_Dev  Pdev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void  Serial_Dev_Reg(PT_Serial_Dev  Pdev)
{
    SYS_ListAdd_Tail(&(Pdev->stListSerial),&(Serial_Mng.stListSerial));
}
/*****************************************************************************
 * Function      : Serial_Device_Reg
 * Description   : ���еĴ����豸����ע��
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180705
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
void  Serial_Device_Reg()
{
    Serial_Qr_Reg();
	Serial_GPs_Reg();
	Serial_Card_Reg();
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
void   *Serial_Deal_Fun(void *arg)
{   
    /*ִ�д����豸ע��*/
    Serial_Device_Reg();

    while(1)
    {  
       /*��ѯ���еĴ����豸*/
       Serial_Mng.Serial_FdsProc(&Serial_Mng);
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
int  Serial_Mng_Proc()
{

    pthread_t   SerialMngt;

    SYS_LIST_HEAD_INIT(Serial_Mng.stListSerial);
    
	if(pthread_create (&SerialMngt,NULL,Serial_Deal_Fun,NULL)<0)
	{
	    return  ERROR; 
    } 
    
	return  SUCESS;
	
}

