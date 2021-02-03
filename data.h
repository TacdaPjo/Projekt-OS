#ifndef DATA_H_
#define DATA_H_

#include "kernel.h"

#pragma region var
typedef int i32;
typedef int b32;

typedef list TaskList;
typedef listobj TaskNode;

extern i32 memCounter = 0;
extern i32 kernelMode = 0;
extern i32 tickCounter = 0;

extern volatile i32 first = 0;

extern TaskList *timerList;
extern TaskList *waitList;
extern TaskList *readyList;
extern TCB *Running;

TCB *PreviousTask;
TCB *NextTask; /* Pointers to previous and next running tasks */

#pragma endregion var

#pragma region funcs
TaskNode *createListObj(TCB *task);
exception addTaskToList(list *taskList, TCB *task);
int compareListObjs(TaskNode *obj1, TaskNode *obj2);
TaskList *createTaskList(void);
exception removeTask(TaskNode *task, TaskList *list);
void TimerInt(void);
void *allocSafe(size_t size);
void memoryFree(void *Safedata);
TaskNode *firstTask(TaskList *list);
TaskNode *lastTask(TaskList *list);
#pragma endregion funcs

#endif
