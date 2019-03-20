/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_circle_bufffer.c
 * Author        : xundanqing
 * Date          : 2018-06-06
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-06-06
 *   Author      : xundanqing
 *   Modification: Created file

*************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "tm_decoler.h"
#include "tm_circle_buffer.h"
/*****************************************************************************
 * Function      : TM_CirQue_Init
 * Description   : ѭ�����еĳ�ʼ��
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : unsigned
 * Others        : 
 * Record
 * 1.Date        : 20171016
 *   Author      : xundanqing
 *   Modification: Created function
*****************************************************************************/
void   TM_CirQue_Init (PTCIRCLE_QUENE Q)
{
    Q->front   =  0;
    Q->tail    =  0;
    Q->check   =  0;
    Q->total   =  CIRCLE_BUFFER_SIZE;
    memset(Q->buffer,0X00,CIRCLE_BUFFER_SIZE);
}
/*****************************************************************************
 * Function      : TM_CirQue_Empty
 * Description   : ѭ�����п��ж�
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : unsigned
 * Others        : 
 * Record
 * 1.Date        : 20171016
 *   Author      : xundanqing
 *   Modification: Created function
*****************************************************************************/
int   TM_CirQue_IsEmpty (PTCIRCLE_QUENE Q)
{
    return  (Q->tail == Q->front) ? CIRCLE_TRUE : CIRCLE_FALSE;
}
/*****************************************************************************
 * Function      : TM_CirQue_Full
 * Description   : ѭ�����е����ж�
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : unsigned
 * Others        : 
 * Record
 * 1.Date        : 20171016
 *   Author      : xundanqing
 *   Modification: Created function
*****************************************************************************/
int TM_CirQue_IsFull (PTCIRCLE_QUENE Q)
{
    return  ((Q->tail + 1) % Q->total)  == Q->front ? CIRCLE_TRUE : CIRCLE_FALSE;
}
/*****************************************************************************
 * Function      : TM_Move_Check
 * Description   : �ƶ����ָ�뵽����ͷ��
 * Input         : PTCIRCLE_QUENE  Q  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180606
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int TM_Move_Check(PTCIRCLE_QUENE  Q)
{
   Q->check = Q->front;
}
/*****************************************************************************
 * Function      : TM_CirQue_len
 * Description   : ѭ�����еĳ��Ȼ��
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : unsigned
 * Others        : 
 * Record
 * 1.Date        : 20171016
 *   Author      : xundanqing
 *   Modification: Created function
*****************************************************************************/
int TM_CirQue_Len (PTCIRCLE_QUENE Q)
{
    return (Q->tail+Q->total-Q->front)%Q->total;
}
/*****************************************************************************
 * Function      : TM_CirQue_Left
 * Description   : ��������������
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180606
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int TM_CirQue_Left(PTCIRCLE_QUENE Q)
{
   return  (Q->total - TM_CirQue_Len(Q));
}
/*****************************************************************************
 * Function      : TM_CirQue_Insert
 * Description   : ����ѭ������
 * Input         : PTCIRCLE_QUENE  Q     
                unsigned  char *data  
 * Output        : None
 * Return        : unsigned
 * Others        : 
 * Record
 * 1.Date        : 20171016
 *   Author      : xundanqing
 *   Modification: Created function
*****************************************************************************/
int TM_CirQue_Insert (PTCIRCLE_QUENE Q, unsigned char* data)
{
    if (TM_CirQue_IsFull(Q))
        return   ERROR;
        
    else
    {
        Q->buffer[Q->tail]  = *data;
        Q->tail             = (Q->tail+1)%Q->total;
    }
    return   SUCESS;
}
/*****************************************************************************
 * Function      : TM_CirQue_Out
 * Description   : ѭ�����еĶ�������
 * Input         : PTCIRCLE_QUENE      Q  
                   unsigned  char *data   
 * Output        : None
 * Return        : unsigned
 * Others        : 
 * Record
 * 1.Date        : 20171016
 *   Author      : xundanqing
 *   Modification: Created function
*****************************************************************************/
int TM_CirQue_Out (PTCIRCLE_QUENE Q, unsigned char* data)
{
    if (TM_CirQue_IsEmpty (Q))
        return  ERROR;
        
    else
    {
        *data    =  Q->buffer[Q->front];
         Q->front= (Q->front + 1)%Q->total;
    }
    return     SUCESS;
}
/*****************************************************************************
 * Function      : TM_CirQue_POP
 * Description   : �������λ�õ�һ���ֽ�
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180606
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  TM_CirQue_POP(PTCIRCLE_QUENE Q,unsigned  char* data)
{
    if (TM_CirQue_IsEmpty (Q))
        return   ERROR;
        
    else
    {
        *data    =  Q->buffer[Q->check];
         Q->check= (Q->check + 1)%Q->total;
    }
    return      SUCESS;
}
/*****************************************************************************
 * Function      : TM_CirQue_Shift
 * Description   : ƽ�Ƽ��λ����Ϣ
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180607
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  TM_CirQue_ShiftCheck(PTCIRCLE_QUENE Q)
{
	  Q->check = Q->check+1;
}
/*****************************************************************************
 * Function      : TM_CirQue_ChecktoFront
 * Description   : ���ö���front ��check
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180607
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  TM_CirQue_ChecktoFront(PTCIRCLE_QUENE Q)
{
	Q->front = Q->check;
}
/*****************************************************************************
 * Function      : TM_CirQue_Shift_PoP
 * Description   : ���λ�����ѭ����ȡ����
 * Input         : PTCIRCLE_QUENE Q      
                unsigned  char* data  
                int  len              
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180607
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  TM_CirQue_Check(PTCIRCLE_QUENE Q,unsigned  char* data,int  len)
{ 
	int  Shift_Addr;
	
	Shift_Addr =  Q->check;
    while(len--)
    {
      *(data++) = Q->buffer[Shift_Addr];
       Shift_Addr= (Shift_Addr+1)%Q->total;
    }
    
    return  SUCESS;
}
/*****************************************************************************
 * Function      : TM_CirQue_Insertbuf
 * Description   : ���λ�������������
 * Input         : PTCIRCLE_QUENE Q     
                unsigned char* data  
                int  len             
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180607
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  TM_CirQue_Insert_buffer(PTCIRCLE_QUENE Q, unsigned char* data,int  len)
{   
	if(TM_CirQue_Left(Q) < len)
		return ERROR;
		
    while(len--)
    {
		TM_CirQue_Insert(Q,data++);
    }
	return    SUCESS;
}
/*****************************************************************************
 * Function      : TM_CirQue_Out_buffer
 * Description   : ��û������е���ֵ
 * Input         : PTCIRCLE_QUENE Q     
                unsigned char* data  
                int  len             
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180607
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  TM_CirQue_Out_buffer(PTCIRCLE_QUENE Q, unsigned char* data,int  len)
{
	if(TM_CirQue_Len(Q) < len)
	   return  ERROR;
	   
    while(len--)
    {
       TM_CirQue_Out(Q,data++);
    }
    return     SUCESS;
}
/*****************************************************************************
 * Function      : TM_CirQue_Debug
 * Description   : ���λ����������
 * Input         : PTCIRCLE_QUENE Q  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180606
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int TM_CirQue_Debug(PTCIRCLE_QUENE Q)
{
	printf("CircleQue  info:  \n");
	printf("front  %d\n",Q->front);
	printf("tail   %d\n",Q->tail );
	printf("check  %d\n",Q->check);
}
