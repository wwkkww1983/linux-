/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_serial_ymod.c
 * Author        : xundanqing
 * Date          : 2018-06-13
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-06-13
 *   Author      : xundanqing
 *   Modification: Created file

*************************************************************************************************************/
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/time.h>
#include<sys/types.h>
#include"tm_decoler.h"
#include"tm_serial_base.h"
#include"tm_serial_manager.h"
/*****************************************************************************
 Prototype    : _getchar
 Description  : 等待获得字符
 Input        : PT_Serial_Dev Pdev
                int  time
 Output       : None
 Return Value : static
 Calls        :
 Called By    :

  History        :
  1.Date         : 2018/3/20
    Author       : xundanqing
    Modification : Created function

*****************************************************************************/
static   char  Ymodem_Get_Char(PT_Serial_Dev Pdev,unsigned  char *buf,unsigned char   time)
{
    int     iret;   
    fd_set  rfdset;
    struct  timeval tv;
    
    tv.tv_sec  =  time;
    tv.tv_usec =  0;
    
    FD_ZERO(&rfdset);
    FD_SET (Pdev->Serial_Fd,&rfdset);
    
    iret = select((Pdev->Serial_Fd+1),&rfdset,NULL,NULL,&tv);
    
    if(iret > 0)
    {
		if(read(Pdev->Serial_Fd,buf,1))
        return  SUCESS;
    }
    
    return  ERROR;
}
/*****************************************************************************
 Prototype    : _putchar
 Description  : 发送单个字符
 Input        : PT_Serial_Dev Pdev
                char  data
 Output       : None
 Return Value : static
 Calls        :
 Called By    :

  History        :
  1.Date         : 2018/3/20
    Author       : xundanqing
    Modification : Created function

*****************************************************************************/
static   int   Ymodem_Put_Char (PT_Serial_Dev Pdev,  char  data)
{
    return  Serial_Write(Pdev, &data,1);
}
/*****************************************************************************
 * Function      : _put_chars
 * Description   : 发送一串字符串
 * Input         : PT_Serial_Dev Pdev  
                char  *data         
                int  len            
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180613
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int   Ymodem_Put_Chars(PT_Serial_Dev Pdev, unsigned   char  *data ,  int  len)
{
    return  Serial_Write(Pdev, data,  len);
}
/*****************************************************************************
 Prototype    : crc16
 Description  : ymodem 的校验
 Input        : const unsigned char *buf
                unsigned long count
 Output       : None
 Return Value : static
 Calls        :
 Called By    :

  History        :
  1.Date         : 2018/3/20
    Author       : xundanqing
    Modification : Created function

*****************************************************************************/
static unsigned short  Ymodem_CrC(const unsigned char *buf, unsigned long count)
{
	int i;
	unsigned short crc = 0;
	while(count--)
	{
		crc = crc ^ *buf++ << 8;
		for (i=0; i<8; i++)
		{
			 if (crc & 0x8000)
			    crc = crc << 1 ^ 0x1021;
			    
			 else
			    crc = crc << 1;
		 }
	 }
	 return crc;
}
/*****************************************************************************
 Prototype    : u32_to_str
 Description  : ymdoem 中的头包数据数字-字符转换
 Input        : unsigned int val
 Output       : None
 Return Value : static
 Calls        :
 Called By    :

  History        :
  1.Date         : 2018/3/20
    Author       : xundanqing
    Modification : Created function

*****************************************************************************/
static int Integer_Asc(unsigned int integer ,unsigned char *str)
{
   int  istr_num = 0;
   int  istr_pos = 15;
   unsigned  char  ucstr_tmp[16] = {0};

   while((integer!=0)&&(istr_pos))
   {
      ucstr_tmp[istr_pos--] = (integer%10)+'0';
      integer/=10;

   }

   for(istr_num = 0;istr_num <= (15-istr_pos);istr_num++)
   	   str[istr_num] = ucstr_tmp[istr_pos+istr_num];

   return  SUCESS;
}
/*****************************************************************************
 Prototype    : send_packet
 Description  : 发送数据包 包报号码
 Input        : unsigned char *data
                int block_no
 Output       : None
 Return Value : static
 Calls        :
 Called By    :

  History        :
  1.Date         : 2018/3/20
    Author       : xundanqing
    Modification : Created function

*****************************************************************************/
static void  Ymodem_Send_Packet(PT_Serial_Dev  Pdev, unsigned char *data, int block_no,Ymodem_Call_Back  Ymode_Fun)
{
    unsigned  short  uscrc = 0; 
    unsigned  int    packet_size = 0;
    
    (block_no == 0)?(packet_size = PACKET_SIZE):(packet_size = PACKET_1K_SIZE);
    
    uscrc = Ymodem_CrC(data, packet_size);
    
    Ymodem_Put_Char(Pdev,(block_no==0)?SOH : STX);
    Ymodem_Put_Char(Pdev, block_no  &  0xFF);
    Ymodem_Put_Char(Pdev,~block_no  &  0xFF);
    
    Ymodem_Put_Chars(Pdev,data,packet_size);
    
    Ymodem_Put_Char(Pdev,(uscrc >> 8)&0xFF);
    Ymodem_Put_Char(Pdev,uscrc & 0xFF);

    if(Ymode_Fun)
         Ymode_Fun (block_no);
}
/*****************************************************************************
 Prototype    : send_file_name
 Description  : 1阶段发送文件名称 发送文件名称
 Input        : PT_Serial_Dev Pdev
               
                unsigned  int  file_size
 Output       : None
 Return Value : static
 Calls        : 
 Called By    :

  History        :
  1.Date         : 2018/3/20
    Author       : xundanqing
    Modification : Created function

*****************************************************************************/
static  int  Ymodem_Send_File_Name(PT_Serial_Dev Pdev,char *filename,unsigned  long  file_size,Ymodem_Call_Back  Ymode_Fun)
{
    unsigned  char  ucack = 0;
    unsigned  int   iConut= 0;
	unsigned  int   iFile_Name_Len = 0;
	unsigned  char  ucBlock[PACKET_SIZE] = {0};
    unsigned  char* Packet =  ucBlock;
	
    if(!filename)
    	return  ERROR;

    iFile_Name_Len = strlen(filename);
    if(iFile_Name_Len > FILE_NAME_MAXLEN)
    	return   ERROR;

	while(*filename)
		*(Packet++) = *(filename++);

    Integer_Asc (file_size ,Packet);

    do
    {
   	 	Ymodem_Send_Packet(Pdev,ucBlock , 0,Ymode_Fun);
   	 	
        Ymodem_Get_Char   (Pdev , &ucack, 5);

        if(ucack ==  ACK)
        	return  SUCESS;

        if((ucack == YCRC)&&1)
        	continue;
        
        else if((ucack!= NAK)&&1)
            break;
   	 	
   	}while(0);
 
    return  ERROR;
}
/*****************************************************************************
 * Function      : Send_file_buffer
 * Description   : Ymodem  2阶段发送文件的内容
 * Input         : PT_Serial_Dev  Pdev  
                unsigned char* data  
                unsigned long size   
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180619
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int   Ymodem_Send_File_Buffer(PT_Serial_Dev  Pdev, unsigned char* data, unsigned long size,Ymodem_Call_Back pfun)
{
    int  Sendsize = 0;
    int  blockno  = 1;
    int  itry     = RETRY;
    unsigned  char  uack= 0;
    
    while (size > 0)
    {
        (size >PACKET_1K_SIZE)?(Sendsize = PACKET_1K_SIZE):(Sendsize = size);
    	
         Ymodem_Send_Packet(Pdev,data,blockno,pfun);

         Ymodem_Get_Char   (Pdev,&uack,3) ;
        	 
         if (uack == ACK)
         {
             data += Sendsize;
             size -= Sendsize;
             blockno++;
             continue ;
         }
         else if (uack==CAN ||itry-- < 0)
           return  ERROR;
    }
    return  SUCESS;
}
/*****************************************************************************
 * Function      : Send_file_end
 * Description   : 发送文件发送完结束
 * Input         : PT_Serial_Dev  Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180619
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int  Ymodem_Send_File_End(PT_Serial_Dev  Pdev)
{
    unsigned  char ucack = 0;
	unsigned  int  itry  = RETRY;
	
    do
    {
        Ymodem_Put_Char(Pdev , EOT);
        Ymodem_Get_Char(Pdev,&ucack,2);
        	
    }while((ucack!=ACK)&&((itry--) > 0));

    return  (ucack!=ACK) ? ERROR : SUCESS;
}
/*****************************************************************************
 * Function      : Send_file_Null
 * Description   : 发送128数据空格
 * Input         : PT_Serial_Dev  Pdev  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180619
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static   int  Ymodem_Send_File_Null(PT_Serial_Dev  Pdev)
{
     unsigned  char  uch = 0;
     unsigned  int   itry = RETRY;
     unsigned  char  buffer[128] = {0};

     do
     {
         Ymodem_Send_Packet(Pdev,buffer , 0 ,NULL);
         Ymodem_Get_Char   (Pdev , &uch , 3);
         
     }while((uch != ACK)&& ((itry--) > 0));
     
     return  (uch!=ACK ) ?  ERROR :  SUCESS;
}
/*****************************************************************************
 Prototype    : Send_data_packets
 Description  : 发送文件数据
 Input        : PT_Serial_Dev  Pdev
                unsigned char* data
                unsigned long size
 Output       : None
 Return Value : static
 Calls        :
 Called By    :

  History        :
  1.Date         : 2018/3/20
    Author       : xundanqing
    Modification : Created function

*****************************************************************************/
static int Ymdoem_Send_Packets(PT_Serial_Dev  Pdev, unsigned char* data, unsigned long size,Ymodem_Call_Back pfun)
{

    if(Ymodem_Send_File_Buffer(Pdev,data,size,pfun)==ERROR)
     	return   ERROR;
     	
    if(Ymodem_Send_File_End(Pdev ) ==  ERROR)
    	return   ERROR;
    	
    if(Ymodem_Send_File_Null(Pdev) ==  ERROR)
    	return   ERROR;

    return  SUCESS;
}
/*****************************************************************************
 * Function      : Map_file
 * Description   : 映射文件
 * Input         : char *filepath           
                unsigned  char **mapmem  
                unsigned  int  *maplen   
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180619
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  int Map_file(char *filepath,unsigned  char **mapmem,unsigned  int  *maplen)
{
     int             fd;  
     struct          stat  st; 	
     
     if(stat((const char *)filepath,&st)<0) 
         return ERROR;
         
     *maplen = st.st_size;
     
     if( *maplen ==0)
          return ERROR;
          
     fd = open((const  char *)filepath,O_RDWR) ;
     if(fd < 0)
         return ERROR;
         
     *mapmem = (unsigned  char *)mmap(NULL,(size_t) (*maplen),PROT_READ,MAP_SHARED,fd,0);
     if(*mapmem == MAP_FAILED)
     	  return  ERROR;
     	  
     return  fd;
}
/*****************************************************************************
 * Function      : UnMap_file
 * Description   : 解除文件映射
 * Input         : int  fd              
                unsigned  char *mem  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20180619
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
static  void   UnMap_file(int  fd,unsigned  char *mem ,unsigned  int  len)
{
	if(fd)
		close(fd);
		
	if(mem)
	    munmap(mem,len);
}
/*****************************************************************************
 * Function      : Serial_Ymodem_Process
 * Description   : YMODEM 升级流程
 * Input         : PT_Serial_Dev  Pdev  
                char *filename       
                ymodem_back_fun fun  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180619
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  Ymodem_File_Process(PT_Serial_Dev  Pdev,char *filename,Ymodem_Call_Back Pfun)
{

   int  file_fd = 0;
   unsigned  char   uack = 0;
   unsigned  int    file_len = 0;
   unsigned  char  *file_mem = NULL;

   Serial_Ymode_star(Pdev);
   
   file_fd =  Map_file(filename,&file_mem,&file_len);
   
   if(file_fd < 0)
	    goto  _Ymode_End;

   Ymodem_Get_Char(Pdev,&uack,5);

   if(uack != YCRC)
        goto _Ymode_End;
  
   if(Ymodem_Send_File_Name(Pdev, filename  ,file_len,Pfun)==ERROR)
   	    goto _Ymode_End;

   if(Ymdoem_Send_Packets  (Pdev, file_mem  ,file_len,Pfun)==ERROR)
        goto _Ymode_End;

   UnMap_file(file_fd ,file_mem , file_len);

   Serial_Ymode_exit(Pdev);
   return  SUCESS;

   
   
   _Ymode_End :
      UnMap_file(file_fd ,file_mem , file_len);
      Ymodem_Put_Char(Pdev,CAN);
      Ymodem_Put_Char(Pdev,CAN);
      Serial_Ymode_exit(Pdev);
      return  ERROR;     
}


 
 
 

 


 


 
