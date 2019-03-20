/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_decoler.c
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
#include<string.h>
#include"decoler.h"
#include"circlebuf.h"
/*****************************************************************************
 * Function      : Reverse16
 * Description   : 双字节转换
 * Input         : short sVal  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180607
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
short Reverse16(short sVal)
{
    short sRtn;
    sRtn = (((sVal >> 8) & 0x00FF) | ((sVal << 8) & 0xFF00));
    return sRtn;
}
/*****************************************************************************
 Prototype    : Decoler_PoP_Terminal
 Description  : 检索结束符\r\n数据包
 Input        : PT_DECOLER decoler
                unsigned  char *Cmd
                int *Cmd_len
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2018/3/10
    Author       : xundanqing
    Modification : Created function

*****************************************************************************/
int   Decoler_PoP_Terminal(PT_DECOLER decoler,unsigned  char *Cmd,int *Cmd_len)
{
    int             inum  =0;
    int             buflen=0;
    unsigned  char  cmd   =0;
    int             left_count =0;
    unsigned  char  buffer[BUFFER_SIZE]     =   {'\0'};
    left_count =  TM_CirQue_Use  (decoler->Circle_Buf);
    for(inum = 0 ; inum < left_count ; inum++ )
    {
         cmd =   TM_CirQue_Out(decoler->Circle_Buf);
         if(cmd !='\r'&&cmd!='\n')
         {
             buffer[buflen++] = cmd;
         }
         if(cmd == '\r')
         {
             cmd =   TM_CirQue_Out(decoler->Circle_Buf);
             if(cmd == '\n')
             {
                break;
             }
         }
     }
    *Cmd_len =	 buflen;
     memcpy(Cmd, buffer,buflen);
     return          left_count;
}


