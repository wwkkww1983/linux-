#ifndef  _TM_SYS_LIST_H
#define  _TM_SYS_LIST_H

#include"tm_type.h"

#define  TIMER_ROUTINE  ( 1 )
#define  TIMER_ONEROUND ( 0 )
/*���нṹ��*/
typedef struct SysListHead_st
{
	struct  SysListHead_st *pstNext ,  *pstPrev;
	
}SysListHead;
/**
 * ��ʼ��Listͷ���
 * @name:   SysListHead���ͱ�����
 *
 */
#define SYS_LIST_HEAD_INIT(name)    name.pstNext = &name; name.pstPrev = &name
/**
 * ����List������ʼ��ͷ���
 * @name:   SysListHead���ͱ�����
 *
 */
#define SYS_LIST_HEAD(name) 	  SysListHead name = { &(name), &(name) }
/**
 * ȡ��list�е�Ԫ�����ڽṹ���ƫ��
 * @name:   SysListHead���ͱ�����
 *
 */
#define sys_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
/**
 * ��ȡSysListHeadǶ��Ľṹ�����ַ
 * @ptr:    ָ��SysListHead���ͳ�Աָ��
 * @type:  	SysListHead���ͳ�ԱǶ��Ľṹ������
 * @member: SysListHead���ͳ�Ա��
 *
 */
#define SYS_LIST_ENTRY(ptr, type, member)    ({          \
			const typeof(((type *)0)->member)*mptr = (ptr);    \
		    (type *)((int8 *)mptr - sys_offsetof(type, member)); })
/**
 * ����List
 * @pos:   �����õ��α�
 * @head:  Listͷ���
 *
 */
#define SYS_LIST_FOR_EACH(pos, head) \
			for (pos = (head)->pstNext; pos != (head); pos = pos->pstNext)

/**
 * ����List�е����нڵ㼰Ԫ��
 * @pos:   �����õ��α�
 * @head:  Listͷ���
 *
 */
#define SYS_LIST_FOR_EACH_ENTRY(pos, head, member)	\
			for (pos = SYS_LIST_ENTRY ((head)->pstNext,typeof(*pos), member);	\
	     	&pos->member != (head); 	\
	     	pos = SYS_LIST_ENTRY (pos->member.pstNext, typeof(*pos), member))

#endif
