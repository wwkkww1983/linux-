/********************************************************************************
 ****        Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.           ****
 ********************************************************************************
 * File Name     : debug.h
 * Author        : xundanqing
 * Date          : 2017-10-17
 * Description   : 调试打印宏
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2017-09-07
 *   Author      : xundanqing
 *   Modification: Created file
*************************************************************************************************************/
#ifndef  _DEBUG_H_
#define  _DEBUG_H_

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

/*打印颜色宏*/
#ifndef _PRINT_COLOR_
#define _PRINT_COLOR_
#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"
#endif

typedef enum
{
    DEBUG,
    WARN,
    ERR,
} DEBUG_LEVEL;
extern DEBUG_LEVEL Debug_Val;

//#define SYS_LOG

/**
 *@调试打印十六进制字符串
 */
int    prc_hex   (void* buf, unsigned int len);

/**
 *@提示打印十六进制字符串
 */
int     pre_data  (void* buf, unsigned int len);

/**
 *@字符串打印信息
 */
int      prc_string(void* buf, unsigned int len);

/*日志初始化*/
void     Log_Init  ();
 
#ifdef   SYS_LOG
#define  pr_info(fmt,args...)   do{ syslog(LOG_INFO,  fmt,   ##args);}while(0);
#define  pr_debug(fmt,args...)  do{ syslog(LOG_DEBUG, fmt,   ##args);}while(0);
#define  pr_err(fmt,args...)    do{ syslog(LOG_ERR,   fmt,   ##args);}while(0);
#endif

#ifndef  SYS_LOG
/**
 *@终端打印提示信息提示黄色
 */
#define pr_info(fmt,args...) do{ \
                                 printf(YELLOW"[card:]<%s>[%d]", __FILE__, __LINE__);\
                                 printf(fmt, ##args);\
                                 printf("\r\n"NONE);\
                               }while(0);

/**
 *@终端打印提示信息提示绿色
 */
#define pr_debug(fmt,args...) do{ \
                                 printf(GREEN"[card:]<%s>[%d]", __FILE__, __LINE__);\
                                 printf(fmt, ##args);\
                                 printf("\r\n"NONE);\
                                }while(0);

/**
 *@终端打印错误信息红色打印
 */
#define pr_err(fmt,args...) do{  \
                                 printf(RED"[card:]<%s>[%d]", __FILE__, __LINE__);\
                                 printf(fmt, ##args);\
                                 printf("\r\n"NONE);\
                               }while(0);
#endif

#endif
