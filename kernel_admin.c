#include "data.h"

uint32_t memCounter;
uint32_t kernelMode;
uint32_t tickCounter;

extern TCB idle_task;

#pragma region once
void idleTask();
void TimerInt();
exception init_kernel(void);
exception create_task(void (*body)(), uint d);
list *create_task_list(void);
void *allocSafe(size_t size);
void safeData_free(void *safeData);
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
void safeData_free(void *safeData)
{
  if (safeData)
  {
    isr_off();
    free(safeData);
    memCounter--;
    isr_on();
  }
}

exception init_kernel(void)
{

  //Set tick counter to zero
  set_ticks(0);

  // Create necessary data structures
  if ((ReadyList = create_task_list()) == NULL)
    return FAIL;
  if ((WaitingList = create_task_list()) == NULL)
    return FAIL;
  if ((TimerList = create_task_list()) == NULL)
    return FAIL;


  //Set the kernel in INIT mode
  kernelMode = INIT;


  //Create an Idle task
  if (!create_task(&idleTask, UINT_MAX))
    return FAIL;


  return SUCCESS;
}

/**
* \brief idleTask
* @param Inifinity loop
* @return returing a inf loop
*/
void idleTask()
{
  while (1);
  //TimerInt();
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

  if(kernelMode == INIT)
  { 
    return SUCCESS;
  }
  // after the mandatory initialization you can implement the rest of the suggested pseudocode

  return SUCCESS;
}

void terminate(void)
{
}

void run(void)
{
  //NextTask = ReadyList->pHead->pTask;
  //LoadContext_In_Run();

  /* supplied to you in the assembly file
 * does not save any of the registers
 * but simply restores registers from saved values
 * from the TCB of NextTask
 */
}

void PreviousTask(void)
{
}

void NextTask(void)
{
}


void TimerInt(void) 
{
    count++;
}
