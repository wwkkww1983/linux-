#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<termios.h>
#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/un.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"tm_debug.h"
#include"tm_sys_list.h"
#include"tm_socket_base.h"
#include"tm_serial_base.h"



static void SignalHander(int signal)
{
    
    if(signal == SIGPIPE )
    {
       printf("SIGPIPE  SIGNAL\n");
    }
   
}

static void CaptureAllSignal(void)
{
    int  i;
    for( i = 0; i < 32; i ++)
    {
        if(i!=SIGKILL && i!=SIGSTOP)
           signal(i, SignalHander);
    }

}


void  timer1()
{
	pr_info("timer  1  round %d\n",GetSysTimer32());
}

void  timer2()
{
	pr_info("timer  2  round %d\n",GetSysTimer32());
}
void  timer3()
{
	pr_info("timer  3  round %d\n",GetSysTimer32());
}

void  timer4()
{
	pr_info("timer  4  round %d\n",GetSysTimer32());
}
void  timer5()
{
	pr_info("timer  5  round %d\n",GetSysTimer32());
}

void  timer6()
{
	pr_info("timer  6  round %d\n",GetSysTimer32());
}


int  main()
{
  

  // CaptureAllSignal();
   
   //Serial_Mng_Proc();

   SYS_TimerInit(10);

   uint32 i;
   
   SYS_TimerAdd(2000,3, timer1,&i,1);
   printf("rimer  id  %d\n",i);
   
  

   sleep(20);




   while(1)
   {
     sleep(1);
   }
#if 0
   
   int  i = 0;
   int  iSockfd;
   int  iret;
   TSockAddrIn tSvrAddr;


   int  iSockfd2;
   int  iret2;
   TSockAddrIn tSvrAddr2;
   
   tSvrAddr.sin_family = AF_INET;
   tSvrAddr.sin_port   = htons(10051);
   tSvrAddr.sin_addr.s_addr = inet_addr("192.168.1.199");
   
   Connect_SocketiP_Server(tSvrAddr, &iSockfd);

  

   tSvrAddr2.sin_family = AF_INET;
   tSvrAddr2.sin_port   = htons(10055);
   tSvrAddr2.sin_addr.s_addr = inet_addr("192.168.1.199");
   
   Connect_SocketiP_Server(tSvrAddr2, &iSockfd2);


   while(1)
   {
        iret = write(iSockfd,"xundanqing\n",11);
        sleep(1);
        printf("%d \n",iret);
        printf("%s",strerror(errno));
        i++;
        if(i ==10)
        {
             close(iSockfd);
        }
   }

   printf("connect  end..\n");
   
   while(1)
   {



       
       int     ret;   
       fd_set  rfdset;
       struct  timeval tv;
    
       tv.tv_sec  =  5;
       tv.tv_usec =  0;
    
       FD_ZERO(&rfdset);
       FD_SET (iSockfd,&rfdset);

       int  len;
       unsigned  char buffer[1024]={0};
      
       ret = select(iSockfd+1,&rfdset,NULL,NULL,&tv);
     
       if(ret > 0)
       {
         if(FD_ISSET(iSockfd,&rfdset))
         {
            printf("socket  1  read able\n");
            len  = read(iSockfd,buffer,1024);
            if(len == 0)
            {
               close(iSockfd);
            }
            printf("read  socket  %d\n",len);
            printf("%s\n",buffer);
            
         }
         
      }

      if(ret ==0 )
      {
         printf("socket  time  out\n");
      }

      if(ret < 0)
      {
         printf("socket  error\n");
      }
    
   }
   return   0;
#endif
}
