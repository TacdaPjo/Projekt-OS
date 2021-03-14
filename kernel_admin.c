#include "data.h"

//when memory is full do something so you get no hardfault.

extern TCB idle_task;

#pragma region once
void idleTask();
exception init_kernel(void);
exception create_task(void (*body)(), uint d);
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

exception init_kernel(void)
{

  //Set tick counter to zero
  set_ticks(0);

  // Create necessary data structures
  if ((ReadyList = createList()) == NULL)
    return FAIL;
  if ((WaitingList = createList()) == NULL)
    return FAIL;
  if ((TimerList = createList()) == NULL)
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
  {
  }
}

exception create_task(void (*taskBody)(), uint d)
{
  TCB *task;
  task = (TCB *)allocSafe(sizeof(TCB));
  if (task == NULL)
  {return FAIL;}

  /* you must check if calloc was successful or not! */

  task->PC = taskBody;
  task->SPSR = 0x21000000;
  task->Deadline = d;

  task->StackSeg[STACK_SIZE - 2] = 0x21000000;
  task->StackSeg[STACK_SIZE - 3] = (unsigned int)taskBody;
  task->SP = &(task->StackSeg[STACK_SIZE - 9]);

  if (kernelMode == INIT)
  {
    push(ReadyList, createListObj(task));
    return SUCCESS;
  }

  else
  {
    isr_off();

    PreviousTask = NextTask;

    push(ReadyList, createListObj(task));
    
    NextTask = ReadyList->pHead->pTask;

    SwitchContext();
  }

  return SUCCESS;
}

void terminate(void)
{

  isr_off();

  if(ReadyList->pHead != NULL)
  {
    listobj *temp = pop(ReadyList);

    if(temp != NULL)
    {
      isr_on();
      memoryFree(temp->pTask);
      isr_off();
      memoryFree(temp->pMessage);

    }
  }


  NextTask = ReadyList->pHead->pTask;

  switch_to_stack_of_next_task();

  LoadContext_In_Terminate();
}

void run(void)
{
  //Initialize interrupt timer {Ticks = 0;}
  
    set_ticks(0);
  //tickCounter = 0;

  kernelMode = RUNNING;

  NextTask = ReadyList->pHead->pTask;

  LoadContext_In_Run();

  /* supplied to you in the assembly file
 * does not save any of the registers
 * but simply restores registers from saved values
 * from the TCB of NextTask
 */
}

