/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : tm_decoler.h
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
#ifndef  _TM_DECOLER_H_
#define  _TM_DECOLER_H_

#include "tm_circle_buffer.h"

#define  SUCESS       ( 0)
#define  ERROR        (-1)
#define  MAX_HEAD_LEN (4 )

/*解包器解包状态*/
typedef enum
{
	DECOLER_HEAD,   
	DECOLER_LENT,   
	DECOLER_DATA,   
	DECOLER_TERM,   
	
}decoler_sta_en;

/*通用解析解包器定义*/
typedef struct      st_decoler
{   
    int             decoler_len;
    int             decoler_head_len;
    int             decoler_extra_len;
    int             decoler_maxavail_len;
	unsigned  char  decoler_head[MAX_HEAD_LEN]; 
    decoler_sta_en  decoler_sta;
    PTCIRCLE_QUENE  decoler_circle_buffer;
    
	int            (*Decoler_Crc ) (unsigned char     *buf,int  len);
    int            (*Decoler_Push) (struct   st_decoler* pt_decoler,unsigned  char *buf,int  len);
    int            (*Decoler_POP ) (struct   st_decoler* pt_decoler,unsigned  char *buf,int *len); 
}T_Decoler,*PT_Decoler;

extern     int      Normal_Protocal_POP (struct   st_decoler* pt_decoler,unsigned  char *buf,int  *len);
extern     int	    Normal_Protocal_Push(struct   st_decoler* pt_decoler,unsigned  char *buf,int   len);


#endif

