#include "data.h"

//when memory is full do something so you get no hardfault.

extern TCB idle_task;

#pragma region once
void idleTask();
void TimerInt();
exception init_kernel(void);
exception create_task(void (*body)(), uint d);
TaskList *createTaskList(void);
exception addTaskToList(TaskList *taskList, TCB *task);
void *allocSafe(size_t size);
void memoryFree(void *safeData);
void terminate(void);
void run(void);
#pragma endregion once

#pragma region list
list *TimerList;
list *ReadyList;
list *WaitingList;
#pragma endregion list

/**
* \brief AllocSafe, allocating the memory safe.
* @param Size - is the size that is needed to be allocated
* @return Returning the address
*/
void *allocSafe(size_t size)
{
  void *safeData;

  isr_off();

  safeData = calloc(1, size);

  if (safeData != NULL)
    memCounter++;

  isr_on();

  return safeData;
}

/**
* \brief SafeData_free, freeing the memory of safeData
* @param SizeData
* @return Return
*/
void memoryFree(void *safeData)
{
  if (safeData)
  {
    isr_off();
    free(safeData);
    memCounter--;
    isr_on();
  }
}


//Make this work later on
exception necDataStruct(list *list)
{
   if ((ReadyList = createTaskList()) == NULL)
    return FAIL;
  if ((WaitingList = createTaskList()) == NULL)
    return FAIL;
  if ((TimerList = createTaskList()) == NULL)
    return FAIL;

    return OK;
}

exception init_kernel(void)
{

  //Set tick counter to zero
  set_ticks(0);

  // Create necessary data structures
  if ((ReadyList = createTaskList()) == NULL)
    return FAIL;
  if ((WaitingList = createTaskList()) == NULL)
    return FAIL;
  if ((TimerList = createTaskList()) == NULL)
    return FAIL;



 
  

  //Create an Idle task
  if (!create_task(idleTask, UINT_MAX))
    return FAIL;

  //Set the kernel in INIT mode
  kernelMode = INIT;
  return SUCCESS;
}

/**
* \brief idleTask
* @param Inifinity loop
* @return returing a inf loop
*/
void idleTask()
{
  while (1)
    ;
}

exception create_task(void (*taskBody)(), uint d)
{
  TCB *task;
  task = (TCB *)allocSafe(sizeof(TCB));
  if (task == NULL)
    return FAIL;

  /* you must check if calloc was successful or not! */

  task->PC = taskBody;
  task->SPSR = 0x21000000;
  task->Deadline = d;

  task->StackSeg[STACK_SIZE - 2] = 0x21000000;
  task->StackSeg[STACK_SIZE - 3] = (unsigned int)taskBody;
  task->SP = &(task->StackSeg[STACK_SIZE - 9]);

  if (kernelMode == INIT)
  {
    addTaskToList(ReadyList, task);
    return SUCCESS;
  }

  else
  {
    isr_off();

    PreviousTask = ReadyList->pHead->pTask;

    addTaskToList(ReadyList, task);

    NextTask = ReadyList->pHead->pTask;

    SwitchContext();
  }

  return SUCCESS;
}

void terminate(void)
{

  isr_off();

  removeTask(ReadyList->pHead, ReadyList);

  NextTask = ReadyList->pHead->pTask;

  switch_to_stack_of_next_task();

  LoadContext_In_Terminate();
}

void run(void)
{
  //Initialize interrupt timer {Ticks = 0;}

  set_ticks(0);

  kernelMode = RUNNING;

  NextTask = ReadyList->pHead->pTask;
  //NextTask = ReadyList->pHead->pTask;

  LoadContext_In_Run();

  /* supplied to you in the assembly file
 * does not save any of the registers
 * but simply restores registers from saved values
 * from the TCB of NextTask
 */
}

void TimerInt(void)
{
  tickCounter++;
}
