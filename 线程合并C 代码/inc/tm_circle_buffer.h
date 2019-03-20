/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_circle_buffer.h
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

************************************************************************************************************/
#ifndef   _CIRCLE_H_
#define   _CIRCLE_H_

/**
 * @�����������ķ���ֵ
 *
 */
#define   CIRCLE_FALSE       (0 )
#define   CIRCLE_TRUE        (1 )
#define   CIRCLE_BUFFER_SIZE (4096)

/**
 * @���λ���������
 * ���λ������Ĵ�СΪ�궨��
 */
 
typedef struct Circle
{
    unsigned int   front;    
    unsigned int   tail;     
    unsigned int   check; 
    unsigned int   total;
    unsigned char  buffer[CIRCLE_BUFFER_SIZE];  
    
}TCIRCLE_QUENE,* PTCIRCLE_QUENE;


 /**
  * @���λ�������������
  * ��Ҫ�ĺ�����������Ƕ���
  */
  
 void  TM_CirQue_Init   (PTCIRCLE_QUENE Q);
 int   TM_CirQue_IsEmpty(PTCIRCLE_QUENE Q);
 int   TM_CirQue_IsFull (PTCIRCLE_QUENE Q);
 int   TM_CirQue_Len    (PTCIRCLE_QUENE Q);
 int   TM_CirQue_Insert (PTCIRCLE_QUENE Q,unsigned char *data);
 int   TM_CirQue_Out    (PTCIRCLE_QUENE Q,unsigned char* data);

 
#endif
