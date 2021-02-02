#ifndef DATA_H_
#define DATA_H_

#include "kernel.h"

typedef int i32;
typedef int b32;

typedef list TaskList;
typedef listobj TaskNode;

// ================================= TYPES/DATA =================================== //

//int Ticks; /* global sysTick counter */
//int KernelMode; /* can be equal to either INIT or RUNNING (constants defined * in “kernel_functions.h”)*/
//list *ReadyList, *WaitingList, *TimerList;

extern i32 memCounter = 0;
extern i32 kernelMode = 0;
extern i32 tickCounter = 0;

extern TaskList *timerList;
extern TaskList *waitList;
extern TaskList *readyList;
extern TCB *Running;

TCB *PreviousTask;
TCB *NextTask;   /* Pointers to previous and next running tasks */

/* // ====================================== TASKS API  ====================================== //

TCB *getTCB(const TaskNode *const task);
uint getDeadline(const TaskNode *const task);
uint getnTCnt(const TaskNode *const task);
msg *getTaskMsg(const TaskNode *const task);

TaskNode *getNextTask(const TaskNode *const task);
TaskNode *getPrevTask(const TaskNode *const task);
TaskNode *allocTask(void (*body)(), uint d);

void deleteTask(TaskNode *tasks);
void insertTask(TaskNode *const new, TaskNode *const prev, TaskNode *const next);
TaskNode *removeTask(TaskNode *const task);

TaskNode *getDummyTask(const TaskList *const tasks);
TaskNode *getFirstTask(const TaskList *const tasks);
TaskNode *getLastTask(const TaskList *const tasks);

TaskList *allocTaskList();

void addTask_Deadline(TaskList *const tasks, TaskNode *const new);
void addTask_nTCnt(TaskList *const tasks, TaskNode *const new);
void printTaskList(const TaskList *const tasks);

// ================================== MESSAGES API ========================================= //

TaskNode *getTask(const msg *const m);
void *getData(const msg *const m);

msg *createMsg(void *data, i32 size);

i32 getDataSize(const mailbox *const mBox);
b32 isEmpty(const mailbox *const mBox);

msg *getFirstMsg(const mailbox *const mBox);
msg *getLastMsg(const mailbox *const mBox);
msg *getDummyMsg(const mailbox *const mBox);

void insertMsg(msg *const new, msg *const prev, msg *const next);
msg *removeMsg(msg *const m);
b32 isFull(const mailbox *const mBox);
b32 msgPushFront(mailbox *const mBox, msg *const m);
b32 msgPushBack(mailbox *const mBox, msg *const m);
msg *msgPopFront(mailbox *const mBox);
msg *msgPopBack(mailbox *const mBox);

b32 msgRecIsWaiting(const mailbox *const mBox);
b32 msgSndIsWaiting(const mailbox *const mBox);

// ============================================================================================ // */

TaskNode *create_listobj(TCB *task);
exception Add_task_tolist(list *taskList, TCB *task);
int compare_listobj(TaskNode *obj1, TaskNode *obj2);
TaskList *create_task_list(void);
void deleteTask(TaskNode* task);
TaskNode* removeTask(TaskNode* task);

void TimerInt(void);

void *allocSafe(size_t size);
void safeData_free(void *Safedata);

#endif
