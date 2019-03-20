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

/*串口YMODEM 标志*/
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


/*超时的数据溢出定义*/
#define  SERTIME_CLOCK_OUT       (0XFFFFFFFF)

/*15秒阈值查询断开连连接*/
#define  SERLINKMAX     (3)
 
typedef  int (*Ymodem_Call_Back ) (int  Packnum);

/*串口设备结构体定义*/
typedef struct       SerialDevopr
{
    int              Serial_Fd; 
    int              Serial_Tag;
    int              Serial_Ymod; 
    const char*      Serial_Name  ;         
    const char*      Serial_Port  ; 
    unsigned char    DataBit;             /*串口数据位    */
    unsigned char    StopBit;             /*串口停止位    */
    unsigned char    Crc_Enable;          /*校验使能      */
    unsigned int     BoardRate;           /*串口波特率    */
    unsigned int     Serial_systime;      /*当前运行时间  */
    unsigned int     Serial_idletime;     /*允许无数据查询*/
    unsigned int     Serial_heart_link;   /*心跳阈值连接判断  */
    void *           Serial_private;      /*私有的串口数据接口*/
    pthread_mutex_t  Serial_Mutex ;       /*设备发送互斥锁*/
    int  (*Serial_Init  )  (struct SerialDevopr *Pdev) ;                           /*设备初始化   */
    int  (*Serial_Reset )  (struct SerialDevopr *Pdev) ;                           /*设备软复位   */    int  (*Serial_Inqury)  (struct SerialDevopr *Pdev) ;                           /*设备状态查询 */
	int  (*Serial_Ymodem)  (struct SerialDevopr *Pdev);                            /*ymodem 方式处理     */
	int  (*Serial_Probe )  (struct SerialDevopr *Pdev);                            /*列举同个串口连接数据*/
	int  (*Serial_Read  )  (struct SerialDevopr *Pdev);                            /*设备读取函数*/
	int  (*Serial_Idle  )  (struct SerialDevopr *Pdev);                            /*设备空闲状态*/ 
	int  (*Serial_SendData)(struct SerialDevopr *Pdev,unsigned  char* buf,int len);/*发送设备数据 */               
    struct  st_decoler*    Pst_decoler;  
    SysListHead            stListSerial; 
    
    
}TSerial_Dev,*PT_Serial_Dev;


/*串口设备管理*/
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
