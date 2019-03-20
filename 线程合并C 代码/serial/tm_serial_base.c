/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_serial_base.c
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
#include<linux/types.h>
#include"tm_serial_base.h"

/*****************************************************************************
 * Function      : Phone_block
 * Description   : Phone 设置阻塞模式
 * Input         : int  fd  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20171124
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Serial_block(int fd)
{
   int ret;
   int val = fcntl(fd, F_GETFL,0);
   val  &=  ~O_NONBLOCK;
   ret   =   fcntl(fd, F_SETFL, val); 	
   return    ret;
}
/*****************************************************************************
 * Function      : Phone_unblock
 * Description   : Phone  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20171124
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Srial_unblock(int fd)
{
   int ret;
   int val = fcntl(fd, F_GETFL, 0);
   val  |=   O_NONBLOCK;
   ret =     fcntl(fd, F_SETFL, val); 	
   return  ret;
}
/*****************************************************************************
 * Function      : Phone_clear_Port
 * Description   : 清空串口数据避免干扰
 * Input          : None
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20171124
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Serial_clear_Port(int fd)
{
   char buf ;
   int  rlen = 1;
   Srial_unblock(fd);
   while(rlen>0)
   {
     rlen = read (fd,&buf,1);
   }
   Serial_block(fd);
   return SUCESS;
}
/*****************************************************************************
 * Function      : Set_Serial_Port
 * Description   : 设置串口属性
 * Input         : PT_Serial_Dev  dev  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180607
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Set_Serial_Port(PT_Serial_Dev  dev)
{
    struct termios  opt;
    if (tcgetattr (dev->Serial_Fd,& opt) != 0)
    {
        perror ("tcgetattr");
        return ERROR;
    }
    tcgetattr(dev->Serial_Fd, &opt);
    if (tcgetattr(dev->Serial_Fd,&opt)<0) 
    {
        return   ERROR;
    }
    opt.c_lflag	&= ~(ECHO | ICANON | IEXTEN | ISIG);
    opt.c_iflag	&= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt.c_oflag &= ~(OPOST);
    opt.c_cflag &= ~(CSIZE | PARENB);
    
    switch (dev->DataBit)
    {
     case 7:
         opt.c_cflag |= CS7;
         break;
     case 8:
         opt.c_cflag |= CS8;
         break;
     default:
         opt.c_cflag |= CS8;
         break;
    }
    
    switch (dev->Crc_Enable)
    {
      case 'n':
         {
             opt.c_cflag &=~PARENB;
             opt.c_iflag &=~INPCK;
             break;
         }
     case 'o':
         {
             opt.c_cflag |= (PARODD | PARENB);
             opt.c_iflag |= INPCK;
             break;
         }
     case 'e':
         {
             opt.c_cflag |= PARENB;
             opt.c_cflag &=~PARODD;
             opt.c_iflag |= INPCK;
             break;
         }
     case 's':
         {
             opt.c_cflag &=~PARENB;
             opt.c_cflag &=~CSTOPB;
             break;
         }
     }
     
    cfsetispeed (&opt, dev->BoardRate);
    cfsetospeed (&opt, dev->BoardRate);
    if (tcsetattr(dev->Serial_Fd, TCSANOW,&opt)<0)
    {
       return   ERROR;
    }
    tcflush(dev->Serial_Fd,TCIOFLUSH);
    
    Serial_block(dev->Serial_Fd);
    return SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Init
 * Description   : 串口初始化
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20171125
 *   Author      : xundanqing
 *   Modification: Created function 

*****************************************************************************/
int  Serial_Init(PT_Serial_Dev  Pdev)
{
    int  iRet ;
    
    Pdev->Serial_Fd  = open (Pdev->Serial_Port, O_RDWR | O_NOCTTY);
    if (Pdev->Serial_Fd < 0)
    {
        return ERROR;
    }
    
    iRet = Set_Serial_Port (Pdev);
    if (iRet == ERROR)
    {
        close (Pdev->Serial_Fd);
        return  ERROR;
    }
    
    Pdev->Serial_Tag  = 1;
    Pdev->Serial_Ymod = 0;

    pthread_mutex_init(&(Pdev->Serial_Mutex),NULL);
    
	return SUCESS;
}
/*****************************************************************************
 * Function      : Serial_Ymode_star
 * Description   : 串口Ymodem 标记建立
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180613
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Serial_Ymode_star(PT_Serial_Dev  Pdev)
{
	Pdev->Serial_Ymod = 1;
}
/*****************************************************************************
 * Function      : Serial_Ymode_exit
 * Description   : YMODEM  数据通信退出
 * Input          : None
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180613
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Serial_Ymode_exit(PT_Serial_Dev  Pdev)
{
	Pdev->Serial_Ymod = 0;
}
/*****************************************************************************
 * Function      : Serial_Write
 * Description   : 发送数据串口
 * Input         : PT_Serial_Dev  Pdev     
                unsigned  char *buffer  
                int  len                
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180607
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Serial_Write(PT_Serial_Dev  Pdev,unsigned  char *buf,int  len)
{
	int  writen;
    int  left         = len;
    const char* pcbuf = (const  char *)buf;

    if(!Pdev||!Pdev->Serial_Fd||!len)
         return  ERROR;
          
    pthread_mutex_lock (&Pdev->Serial_Mutex);
    while (left > 0)
    {
        if ((writen = write (Pdev->Serial_Fd, pcbuf, left)) <= 0)
        {
            if ((writen < 0) && (errno == EINTR))
                 writen = 0;
            else
                break;
        }
        left  -=  writen;
        pcbuf +=  writen;
    }
    pthread_mutex_unlock (&Pdev->Serial_Mutex);
    
    return  (left == 0) ? SUCESS : ERROR;
    
}
