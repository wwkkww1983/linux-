#ifndef  _TM_SERIAL_CORE_H
#define  _TM_SERIAL_CORE_H

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/un.h>
#include<netinet/in.h>
#include<pthread.h>
#include"tm_decoler.h"
#include"tm_sys_list.h"


#define SER_SELCT_TIME  (5000) 

/*����YMODEM ��־*/
#define  SOH                     (0x01)             
#define  STX                     (0x02)            
#define  EOT                     (0x04)             
#define  ACK                     (0x06)           
#define  NAK                     (0x15)          
#define  CAN                     (0x18)             
#define  YCRC                    (0x43) 
#define  RETRY                   (0X03)
#define  PACKET_SIZE             (128 )
#define  PACKET_1K_SIZE          (1024)
#define  FILE_NAME_LENGTH        (64)
#define  FILE_SIZE_LENGTH        (16)
#define  FILE_NAME_MAXLEN        (PACKET_SIZE-FILE_SIZE_LENGTH-2)


/*��ʱ�������������*/
#define  SERTIME_CLOCK_OUT       (0XFFFFFFFF)

/*15����ֵ��ѯ�Ͽ�������*/
#define  SERLINKMAX     (3)
 
typedef  int (*Ymodem_Call_Back ) (int  Packnum);

/*�����豸�ṹ�嶨��*/
typedef struct       SerialDevopr
{
    int              Serial_Fd; 
    int              Serial_Tag;
    int              Serial_Ymod; 
    const char*      Serial_Name  ;         
    const char*      Serial_Port  ; 
    unsigned char    DataBit;             /*��������λ    */
    unsigned char    StopBit;             /*����ֹͣλ    */
    unsigned char    Crc_Enable;          /*У��ʹ��      */
    unsigned int     BoardRate;           /*���ڲ�����    */
    unsigned int     Serial_systime;      /*��ǰ����ʱ��  */
    unsigned int     Serial_idletime;     /*���������ݲ�ѯ*/
    unsigned int     Serial_heart_link;   /*������ֵ�����ж�  */
    void *           Serial_private;      /*˽�еĴ������ݽӿ�*/
    pthread_mutex_t  Serial_Mutex ;       /*�豸���ͻ�����*/
    int  (*Serial_Init  )  (struct SerialDevopr *Pdev) ;                           /*�豸��ʼ��   */
    int  (*Serial_Reset )  (struct SerialDevopr *Pdev) ;                           /*�豸��λ   */    int  (*Serial_Inqury)  (struct SerialDevopr *Pdev) ;                           /*�豸״̬��ѯ */
	int  (*Serial_Ymodem)  (struct SerialDevopr *Pdev);                            /*ymodem ��ʽ����     */
	int  (*Serial_Probe )  (struct SerialDevopr *Pdev);                            /*�о�ͬ��������������*/
	int  (*Serial_Read  )  (struct SerialDevopr *Pdev);                            /*�豸��ȡ����*/
	int  (*Serial_Idle  )  (struct SerialDevopr *Pdev);                            /*�豸����״̬*/ 
	int  (*Serial_SendData)(struct SerialDevopr *Pdev,unsigned  char* buf,int len);/*�����豸���� */               
    struct  st_decoler*    Pst_decoler;  
    SysListHead            stListSerial; 
    
    
}TSerial_Dev,*PT_Serial_Dev;


/*�����豸����*/
typedef  struct  t_Serial_Mng
{
   int   (*Serial_IsAvail)(struct t_Serial_Mng  *Mng);
   int   (*Serial_FdsAdd )(struct t_Serial_Mng  *Mng);
   int   (*Serial_FdsRead)(struct t_Serial_Mng  *Mng);
   int   (*Serial_FdsIdle)(struct t_Serial_Mng  *Mng);
   int   (*Serial_FdsProc)(struct t_Serial_Mng  *Mng);
   fd_set           SerialFds;
   SysListHead      stListSerial;
   
}T_Serial_Mng,*PT_Serial_Mng;


/*Serial  Proc */
extern  void  Serial_Dev_Reg(PT_Serial_Dev Pdev);
extern  int   Serial_Write  (PT_Serial_Dev Pdev, unsigned char * buf, int len);
#endif
