#ifndef _DLIST_H_
#define _DLIST_H_

#include "kernel.h"

extern volatile unsigned int send, counter, startUp;
extern int volatile deadlineSend;

extern list *timerList, *waitList, *readyList;
extern mailbox *m, *fullbox;
extern TCB *Running;

list *create_list();
listobj *create_listobj_nTCnt(int num);
listobj *create_listobj_dl(int num);
void insertSortByDeadLine(list *mylist, listobj *object);
void insertSortBynTCnt(list *mylist, listobj *object);
listobj *extractionFirst(list *mylist);
listobj *extractionAnyObj(listobj *object);
void idle(void);
exception wait(uint nTicks);
exception remove_list(list *myList);

#endif