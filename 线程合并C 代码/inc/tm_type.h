#ifndef  _TM_TYPE_H
#define  _TM_TYPE_H

#ifndef TRUE
#define TRUE			1
#endif

#ifndef FALSE
#define FALSE			0
#endif

#ifndef NULL
#define NULL		    0
#endif

/*bool  define */
#define SYS_TRUE		0
#define SYS_FALSE	   -1

/*type  defoine */
typedef int BOOL;
typedef unsigned char   uint8;         /* Unsigned  8 bit  */
typedef signed   char   int8;          /* Signed    8 bit  */
typedef unsigned short  uint16;        /* Unsigned  16 bit */
typedef signed   short  int16;         /* Signed    16 bit */
typedef unsigned int    uint32;        /* Unsigned  32 bit */
typedef signed   int    int32;         /* Signed    32 bit */
typedef unsigned long	ulong;         

/*mutex */
#define  Mutex_Lock(name)      pthread_mutex_lock(name)    
#define  Mutex_UnLock(name)    pthread_mutex_unlock(name) 
#define  Mutex_Init(name)      pthread_mutex_init(name,NULL)

#endif
