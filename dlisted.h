#ifndef _DLISTED_H_INCLUDED
#define _DLISTED_H_INCLUDED

#include "kernel.h"

extern list *timerList, *waitList, *readyList;
extern TCB *Running;

list *createList();

listobj *createListObj_nTCnt(int num);

exception removeList(list *myList);


#endif