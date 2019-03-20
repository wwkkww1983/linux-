/********************************************************************************
 ****        Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.            ****
 ********************************************************************************
 * File Name     : debug.cc
 * Author        : xundanqing
 * Date          : 2017-09-07
 * Description   : ���Դ�ӡ��Ϣ����
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2017-10-16
 *   Author      : xundanqing
 *   Modification: Created file
*************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include "tm_debug.h"
/*****************************************************************************
 * Function      : PRINT_DATA
 * Description   : ��ӡ���ݵ���
 * Input         : unsigned  char  *buf  
                unsigned  int  len    
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20171020
 *   Author      : xundanqing
 *   Modification: Created function
*****************************************************************************/
int pre_data (void* buf, unsigned int len)
{
    int i;
    unsigned char *pdest=(unsigned  char *)buf;
    printf (GREEN "************************************\n");
    for (i = 0; i < len; i++)
    {
        printf ("%02x ",* pdest++);
        if ((i + 1) % 5 == 0)
            printf ("\n");
    }
    printf ("\n");
    printf ("*************************************\n" NONE);
    return 0;
}
/*****************************************************************************
 * Function      : prc_string
 * Description   : ��ӡ�ַ���
 * Input         : unsigned char* buf  
                unsigned int len    
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20171115
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int prc_hex (void * buf, unsigned int len)
{
    int i;
    unsigned char *pdest=(unsigned  char *)buf;
    printf (GREEN "************************************\n");
    for (i = 0; i < len; i++)
    {
        printf ("%02x",* pdest++);
    }
    printf ("\n");
    printf ("*************************************\n" NONE);
    return 0;
}
/*****************************************************************************
 * Function      : pre_string
 * Description   : ��ӡ�����ַ�
 * Input         : unsigned char* buf  
                unsigned int len    
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20171115
 *   Author      : xundanqing
 *   Modification: Created function

*****************************************************************************/
int  prc_string(void * buf, unsigned int len)
{
    int i;
    printf (GREEN"");
    unsigned char *pdest=(unsigned  char *)buf;
    for (i = 0; i < len; i++)
    {
        printf ("%c",*pdest++);
    }
    printf ("\n"NONE);
    return 0;
}


/*��־��ʼ��*/
void Log_Init ()
{
    openlog ("TM6158", LOG_CONS | LOG_PID, LOG_LOCAL7);
}
