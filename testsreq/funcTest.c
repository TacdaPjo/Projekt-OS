#include "assert.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

TCB *task;
listobj *objTest;

int retVal_t1;
int retVal_t2;
int retVal_t3;
int retVal_t4;
int retVal_t5;

char varChar_t1;

mailbox *charMbox;
mailbox *intMbox;
mailbox *floatMbox;

unsigned int low_deadline = 1000;
unsigned int high_deadline = 100000;

int deadLineValue;
int nTCntValue;
int ret_valList;
int ret_valDL;
int ret_valNtCNT;
int retValMSG;

unsigned int g0 = 0, g1 = 0, g2 = 0, g3 = 1;

int random_number(int min_num, int max_num)
{
  int result = 0, low_num = 0, hi_num = 0;

  if (min_num < max_num)
  {
    low_num = min_num;
    hi_num = max_num + 1;
  }
  else
  {
    low_num = max_num + 1;
    hi_num = min_num;
  }

  srand(time(NULL));
  result = (rand() % (hi_num - low_num)) + low_num;
  return result;
}

int assert_my(x)
{
  if (!(x))
    return FAIL;
  else
    return OK;
}

int pointerEquals(void *pointer1, void *pointer2)
{

  if (pointer1 == pointer2)
  {
    return 1;
  }
  return 0;
}

int intvarEquals(int varint1, int varint2)
{

  if (varint1 == varint2)
  {
    return 1;
  }
  return 0;
}

void listTest()
{

  list *TimerList, *WaitingList, *ReadyList, *testList, *emptyList;

  objTest = NULL;

  init_kernel();

  emptyList = createList();
  testList = createList();
  ReadyList = createList();
  TimerList = createList();
  WaitingList = createList();

  ret_valList = assert_my(!pointerEquals(TimerList, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(TimerList->pHead, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(TimerList->pTail, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(TimerList->pHead, TimerList->pTail));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(TimerList->pHead->pNext, TimerList->pTail->pNext));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(TimerList->pHead->pPrevious, TimerList->pTail->pPrevious));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(!pointerEquals(WaitingList, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(WaitingList->pHead, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(WaitingList->pTail, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(WaitingList->pHead, WaitingList->pTail));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(WaitingList->pHead->pNext, WaitingList->pTail->pNext));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(WaitingList->pHead->pPrevious, WaitingList->pTail->pPrevious));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(!pointerEquals(ReadyList, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(ReadyList->pHead, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(ReadyList->pTail, NULL));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(ReadyList->pHead, TimerList->pTail));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(ReadyList->pHead->pNext, TimerList->pTail->pNext));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  ret_valList = assert_my(pointerEquals(ReadyList->pHead->pPrevious, TimerList->pTail->pPrevious));
  if (ret_valList != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  TaskNode *obj2, *obj3;
  TCB *testTask;

  TaskNode *obj = allocSafe(sizeof(TaskNode));
  TaskNode *obj4 = allocSafe(sizeof(TaskNode));

  obj4 = createListObj(testTask);

  obj = moveFromList(testList, testList->pHead); //Empty list
  if (obj != NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  obj = moveFromList(testList, testList->pHead); //doesnt exist
  if (obj != NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  push(testList, obj4);

  obj = moveFromList(testList, testList->pHead); //First obj
  if (obj == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  push(testList, obj4);

  obj = moveFromList(testList, testList->pTail); //the last
  if (obj == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  push(testList, obj4);

  obj = moveFromList(testList, testList->pHead->pNext->pNext); //In middle somewhere
  if (obj == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  obj2 = pop(emptyList); //pop emptylist

  if (obj2 != NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  push(emptyList, createListObj(task));

  obj2 = pop(emptyList); //pop with only one element in list
  if (obj2 == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  obj2 = createListObj(task);
  obj3 = createListObj(task);

  push(emptyList, obj2);
  push(emptyList, obj3);

  obj2 = pop(emptyList);
  if (obj2 == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  if (g1 == 0) /* the first test has been passed for the lists */
  {
    g1 = OK;
  }
  else
  {
    while (1)
    { /*the first test did not pass for the lists */
    }
  }
}

void testObj()
{
  g1 = OK;
  if (g1 == OK)
  {
    ReadyList = createList();
    TimerList = createList();
    WaitingList = createList();

    objTest = createListObj(task);

    if (objTest != NULL)
    {
      deadLineValue = objTest->pNext->pTask->Deadline;
      nTCntValue = objTest->pNext->nTCnt;

      ret_valDL = assert_my(!pointerEquals(objTest, NULL));
      if (ret_valDL != OK)
      {
        g1 = FAIL;
        while (1)
        {
        }
      }
      ret_valNtCNT = assert_my(intvarEquals(TimerList->pHead->nTCnt, (nTCntValue)));
      if (ret_valNtCNT != OK)
      {
        g1 = FAIL;
        while (1)
        {
        }
      }

      ret_valDL = assert_my(intvarEquals(TimerList->pHead->pTask->Deadline, (deadLineValue)));
      if (ret_valDL != OK)
      {
        g1 = FAIL;
        while (1)
        {
        }
      }

      ret_valDL = assert_my(intvarEquals(check_TimerList(), NULL));
      if (ret_valDL != OK)
      {
        g1 = FAIL;
        while (1)
        {
        }
      }

      timeListPush(objTest);

      ret_valDL = assert_my(intvarEquals(check_TimerList(), OK));
      if (ret_valDL != OK)
      {
        g1 = FAIL;
        while (1)
        {
        }
      }
    }
  }

  if (g1 == OK)
  {
    g0 = OK;
  } /* the second test has been passed for the nodes */
  else
  {
    while (1)
    {
    } /*the second test did not pass for the nodes */
  }
}

void task_body_1()
{
  char varChar_t1;
  int retVal_t1, varInt_t1;
  float varFloat_t1;

  // test on an empty mailbox
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 != FAIL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 != FAIL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 != FAIL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  assert_my(intvarEquals(remove_mailbox(charMbox), OK)); //After removing, you have to create a new one if you wanna send_no_wait

  charMbox = create_mailbox(3, sizeof(char));
  if (varChar_t1 == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  // now send
  varChar_t1 = 'a';
  retVal_t1 = send_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }
  varChar_t1 = 'b';
  retVal_t1 = send_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }
  varChar_t1 = 'c';
  retVal_t1 = send_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 != OK)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  assert_my(intvarEquals(remove_mailbox(charMbox), NOT_EMPTY)); //Not empty, won't remove the mailbox.
  // now read
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 == OK)
  {
    if (varChar_t1 != 'a')
    {
      g1 = FAIL;
      while (1)
      {
      }
    }
  }
  else
  {
    while (1)
    {
    }
  }

  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 == OK)
  {
    if (varChar_t1 != 'b')
    {
      g1 = FAIL;
      while (1)
      {
      }
    }
  }
  else
  {
    while (1)
    {
    }
  }

  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if (retVal_t1 == OK)
  {
    if (varChar_t1 != 'c')
    {
      g1 = FAIL;
      while (1)
      {
      }
    }
  }
  else
  {
    while (1)
    {
    }
  }

  assert_my(intvarEquals(remove_mailbox(charMbox), OK)); //It is empty, ok to remove it.
  //assert_my(intvarEquals(remove_mailbox(charMbox), OK));  //this call will show that it was really removed in the allocated memory

  varInt_t1 = 100;
  retVal_t1 = send_wait(intMbox, &varInt_t1);
  if (retVal_t1 != OK)
  {
    while (1)
    {
    }
  }

  g3 = FAIL;
  varFloat_t1 = 3.14159;
  retVal_t1 = send_wait(floatMbox, &varFloat_t1);
  if (retVal_t1 == OK)
  {
    while (1)
    {
    }
  }
  else
  {
    g3 = OK;
  }

  terminate(); /* if communication calls in this task worked */
}

void task_body_2()
{
  int retVal_t2;
  int varInt_t2;

  retVal_t2 = receive_wait(intMbox, &varInt_t2);
  if (varInt_t2 != 100)
  {
    while (1)
    {
    }
  }

  retVal_t2 = receive_wait(intMbox, &varInt_t2);
  if (varInt_t2 != 500)
  {
    while (1)
    {
    }
  }

  while (1)
  {
    if (g3 == OK)
      break;
  }

  while (1)
  {
    /* Alles Gut ! This unit test has been passed */
  }
}

void task_body_3()
{
  int retVal_t3;
  int varInt_t3;

  varInt_t3 = 500;
  retVal_t3 = send_wait(intMbox, &varInt_t3);

  terminate();
}

void task_body_4()
{
  int retVal_t4;
  int varInt_t4 = 0;

  int retValZ;

  set_deadline(random_number(4001, 5000)); //need set_deadline over wait time because of else deadline reached
  retValZ = assert_my(intvarEquals(ReadyList->pHead->pTask->Deadline, NextTask->Deadline));
  if (retValZ != OK)
  {
    while (1)
    {
    }
  }

  retVal_t4 = wait(900);
  if (retVal_t4 != OK)
  {
    while (1)
    { /* no use going further */
    }
  }

  retVal_t4 = receive_wait(intMbox, &varInt_t4);
  if (varInt_t4 != 255)
  {
    while (1)
    {
    }
  }

  retVal_t4 = wait(4000);
  if (retVal_t4 != DEADLINE_REACHED)
  {
    while (1)
    { /* no use going further */
    }
  }
  terminate();
}

void task_body_5()
{
  int retVal_t5;
  int varInt_t5 = 255;
  int retValZ;

  set_deadline(random_number(4000, 10000)); //need set_deadline over wait time because of else deadline reached
  retValZ = assert_my(intvarEquals(ReadyList->pHead->pTask->Deadline, NextTask->Deadline));
  if (retValZ != OK)
  {
    while (1)
    {
    }
  }

  retVal_t5 = send_wait(intMbox, &varInt_t5);
  if (retVal_t5 != OK)
  {
    while (1)
    {
    }
  }

  retVal_t5 = wait(200);
  if (retVal_t5 != OK)
  {
    while (1)
    { /* no use going further */
    }
  }

  terminate();
}


void unitTests()
{
  SystemInit();
  SysTick_Config(100000);
  SCB->SHP[((uint32_t)(SysTick_IRQn)&0xF) - 4] = (0xE0);
  isr_off();

  int retValZ;

  g0 = OK;
  exception retVal = init_kernel();
  if (retVal != OK)
  {
    g0 = FAIL;
    while (1)
    { /* no use going further */
    }
  }

  if (ReadyList->pHead != ReadyList->pTail)
  {
    g0 = FAIL;
  }
  if (WaitingList->pHead != WaitingList->pTail)
  {
    g0 = FAIL;
  }
  if (TimerList->pHead != TimerList->pTail)
  {
    g0 = FAIL;
  }

  if (g0 != OK)
  {
    while (1)
    { /* no use going further */
    }
  }

  charMbox = create_mailbox(3, sizeof(char));
  if (retVal == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  intMbox = create_mailbox(3, sizeof(int));
  if (retVal == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  floatMbox = create_mailbox(3, sizeof(float));
  if (retVal == NULL)
  {
    g1 = FAIL;
    while (1)
    {
    }
  }

  retVal = create_task(task_body_1, low_deadline);
  if (retVal != OK)
  {
    while (1)
    { /* no use going further */
    }
  }

  retVal = create_task(task_body_2, 8 * high_deadline);
  if (retVal != OK)
  {
    while (1)
    { /* no use going further */
    }
  }

  retVal = create_task(task_body_3, 2 * low_deadline);
  if (retVal != OK)
  {
    while (1)
    { /* no use going further */
    }
  }

  retVal = create_task(task_body_4, 3 * low_deadline);
  if (retVal != OK)
  {
    while (1)
    { /* no use going further */
    }
  }

  retVal = create_task(task_body_5, 4 * low_deadline);
  if (retVal != OK)
  {
    while (1)
    { /* no use going further */
    }
  }

  g3 = FAIL;
  run();

  while (1)
  { /* something is wrong with run */
  }
}
