#ifndef  _TM_CAN_BASE_H_
#define  _TM_CAN_BASE_H_

#include <pthread.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "tm_sys_list.h"

#define  MAX_CAN_NUM     (16)
#define  CAN_SELCT_TIME  (5000)

typedef struct  can_dev
{
    int               	  Device_fd;
    int                   Device_Tag;
    unsigned  char      * Device_Can_Name;
    unsigned  char      * Device_module_name;
    unsigned  char        Device_filter_num ;
    pthread_mutex_t       Device_mutex;
    struct    can_filter* Device_filter_table;
    int       	         (*Device_Read)  (struct can_dev *Pdev);
    int                  (*Device_Init)  (struct can_dev *Pdev);
    int                  (*Device_Inqury)(struct can_dev *Pdev);
    int                  (*Device_Send)(struct can_dev *Pdev, unsigned  char *buf,int  len);
    SysListHead          stCanlist;
}T_CAN_DEV,*PT_CAN_DEV;

#endif
