#include "data.h"

#pragma region three
TaskNode *createListObj(TCB *task);
TaskList *createList(void);
exception moveListObj(list *firstList, list *secondList, listobj *obj);
TaskNode *pop(TaskList *listPop);
void push(TaskList *listo, TaskNode *addObj);
msg *msg_dequeue(mailbox *mBox);
void msg_enqueue(mailbox *mBox, msg *mes);
void timeListPush(TaskNode *addObj);
#pragma endregion three

TaskList *createList(void)
{
    TaskList *task_list = allocSafe(sizeof(TaskList));
    if (task_list == NULL)
        return NULL;

    return task_list;
}

TaskNode *createListObj(TCB *task)
{
    TaskNode *list_Obj;

    if ((list_Obj = allocSafe(sizeof(TaskNode))) == NULL)
        return NULL;

    list_Obj->nTCnt = ticks();
    list_Obj->pTask = task;

    return list_Obj;
}

TaskNode *moveFromList(TaskList *firstList, TaskNode *task)
{
    if (firstList->pHead == NULL || firstList == NULL || task == NULL)
    {
        return NULL;
    }
    TaskNode *temp;


    if (firstList->pHead == task)
    {
        temp = firstList->pHead;
        firstList->pHead = firstList->pHead->pNext;
        firstList->pHead->pPrevious = NULL;
        return temp;
    }

    while (temp->pNext != NULL )
    {

        if (temp == task || temp->pTask == task->pTask)
        {
            break;
        }
        temp = temp->pNext;
    }

    if (temp->pNext == NULL)
    {

        temp->pPrevious->pNext = NULL;
        return temp;
    }
    else
    {
        temp->pPrevious->pNext = temp->pNext;
        temp->pNext->pPrevious = temp->pPrevious;
        return temp;
    }
}

TaskNode *pop(TaskList *listPop)
{
    listobj *deleteObj;
    deleteObj = listPop->pHead;

    if (listPop->pHead == listPop->pTail)
    {
        listPop->pHead = listPop->pTail = NULL;
    }
    else
    {
        listPop->pHead = listPop->pHead->pNext;
        listPop->pHead->pPrevious = NULL;
    }

    deleteObj->pNext = NULL;
    deleteObj->pPrevious = NULL;
    return deleteObj;
}

void push(TaskList *listo, TaskNode *addObj)
{
    TaskNode *temp;
    temp = listo->pHead;

    if (listo->pHead == NULL && listo->pTail == NULL)
    {

        listo->pHead = addObj;
        listo->pTail = addObj;
        temp->pNext = NULL;
        return;
    }
    else
    {
        if (addObj->pTask->Deadline < temp->pTask->Deadline)
        {

             addObj->pNext = temp;
            temp->pPrevious = addObj;
            listo->pHead = addObj;
            return;
        }
        else
        {
            while (temp->pNext != NULL && temp->pTask->Deadline <= addObj->pTask->Deadline)
            {
                temp = temp->pNext;
            }
            if (temp->pNext == NULL && temp->pTask->Deadline <= addObj->pTask->Deadline)
            {

                temp->pNext = addObj;
                addObj->pPrevious = temp;
                listo->pTail = addObj;
                addObj->pNext = NULL;
                return;
            }
            else
            {
                temp->pPrevious->pNext = addObj;
                addObj->pPrevious = temp->pPrevious;
                temp->pPrevious = addObj;
                addObj->pNext = temp;
                return;
            }
        }
    }
}

exception check_TimerList()
{
    listobj *temp = TimerList->pHead;

    uint comp;

    while (temp != NULL)
    {
        if (temp->nTCnt <= temp->pTask->Deadline)
        {
            comp = temp->nTCnt;
        }
        else
        {
            comp = temp->pTask->Deadline;
        }
        if (comp > tickCounter)
        {
            break;
        }
        else
        {
            temp = temp->pNext;
            push(ReadyList, pop(TimerList));
            PreviousTask = NextTask;
            NextTask = ReadyList->pHead->pTask;
        }
        return OK;
    }
}

void check_WaitingList()
{
    listobj *temp = WaitingList->pHead;

    while (temp != NULL)
    {
        if (temp->pTask->Deadline > tickCounter)
        {
            break;
        }
        else
        {
            temp->pMessage->pPrevious->pNext = temp->pMessage->pNext;
            temp->pMessage->pNext->pPrevious = temp->pMessage->pPrevious;
            memoryFree(temp->pMessage);
            temp = temp->pNext;
            push(ReadyList, pop(WaitingList));
            PreviousTask = NextTask;
            NextTask = ReadyList->pHead->pTask;
        }
    }
}

void timeListPush(TaskNode *addObj)
{

    listobj *temp = TimerList->pHead;

    if (addObj->pTask->Deadline >= addObj->nTCnt)
    {
        compVal = addObj->nTCnt;
    }
    else
    {
        compVal = addObj->pTask->Deadline;
    }
    if (temp == NULL)
    {

        TimerList->pHead = TimerList->pTail = addObj;
        TimerList->pHead->pNext = NULL;
    }
    else
    {
        if (compVal <= TimerList->pHead->pTask->Deadline || compVal <= TimerList->pHead->nTCnt)
        {
            
            addObj->pNext = TimerList->pHead;
            TimerList->pHead->pPrevious = addObj;
            TimerList->pHead = addObj;
        }
        else
        {
            while (temp->pNext != NULL && (temp->pTask->Deadline <= compVal || temp->nTCnt <= compVal))
            {
                temp = temp->pNext;
            }
            if (temp->pNext == NULL && (temp->pTask->Deadline <= compVal || temp->nTCnt <= compVal))
            {

                temp->pNext = addObj;
                addObj->pPrevious = temp;
                return;
            }
            else
            {
                temp->pPrevious->pNext = addObj;
                addObj->pPrevious = temp->pPrevious;
                temp->pPrevious = addObj;
                addObj->pNext = temp;
            }
        }
    }
}

//Enqueue from the tail
void msg_enqueue(mailbox *mBox, msg *mes)
{
    if (mBox->nMessages == 0)
    {
        mBox->pHead = mBox->pTail = mes;
        mBox->nMessages++;
    }
    else
    {
        mes->pPrevious = mBox->pTail;
        mBox->pTail->pNext = mes;
        mBox->pTail = mes;
        mBox->nMessages++;
    }
}

//dequeue from the head
msg *msg_dequeue(mailbox *mBox)
{

    if (mBox->pHead == NULL)
        return NULL;

    msg *temp = mBox->pHead;
    if (mBox->pHead == mBox->pTail)
    {
        mBox->pHead = NULL;
        mBox->pTail = NULL;
        mBox->nMessages--;
        return temp;
    }
    else
    {
        mBox->pHead = mBox->pHead->pNext;
        mBox->pHead->pPrevious = NULL;
        mBox->nMessages--;
        return temp;
    }
}