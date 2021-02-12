#include "data.h"

#pragma region three
TaskNode *createListObj(TCB *task);
exception addTaskToList(list *taskList, TCB *task);
int compareListObjs(TaskNode *obj1, TaskNode *obj2);
TaskList *createTaskList(void);
exception removeTask(TaskNode *task, TaskList *list);
TaskNode *firstTask(TaskList *list);
TaskNode *lastTask(TaskList *list);
exception removeTCB(TaskList *list, TCB *task);
exception moveListObj(list *firstList, list *secondList, listobj* obj);
#pragma endregion three

TaskList *createTaskList(void)
{
    TaskList *task_list = allocSafe(sizeof(TaskList));
    if (task_list == NULL)
        return NULL;

    return task_list;
}

exception addTaskToList(TaskList *taskList, TCB *task)
{
    TaskNode *list_obj;
    TaskNode *temp;

    if (taskList->pHead == NULL)
    {

        list_obj = createListObj(task);

        if (list_obj == NULL)
            return FAIL;

        taskList->pHead = list_obj;
        taskList->pTail = taskList->pHead;
    }
    else
    {
        list_obj = createListObj(task);
        if (list_obj == NULL)
            return FAIL;

        temp = taskList->pHead;

        while (1)
        {
            if (temp == taskList->pTail && !compareListObjs(temp, list_obj))
            {
                temp->pNext = list_obj;
                taskList->pTail = list_obj;
                list_obj->pPrevious = temp;
                return OK;
            }

            else if (compareListObjs(temp, list_obj))
            {
                //if list_obj has a tighter deadline

                if (temp != taskList->pHead)
                {
                    list_obj->pNext = temp;
                    list_obj->pPrevious = temp->pPrevious;
                    temp->pPrevious = list_obj;
                    list_obj->pPrevious->pNext = list_obj;
                }

                else
                {
                    temp->pPrevious = list_obj;
                    list_obj->pNext = temp;
                    taskList->pHead = list_obj;
                }
                return OK;
            }
            temp = temp->pNext;
        }

        taskList->pTail->pNext = list_obj;
        taskList->pTail = taskList->pTail->pNext;
    }
    return SUCCESS;
}

int compareListObjs(TaskNode *obj1, TaskNode *obj2)
{

    //if((obj1->pTask->Deadline == obj2->pTask->Deadline))
    if ((obj1->pTask->Deadline) > (obj2->pTask->Deadline))
    {
        return OK;
    }

    else
    {
        return FAIL;
    }
}

TaskNode *createListObj(TCB *tcb)
{
    TaskNode *list_Obj;

    if ((list_Obj = allocSafe(sizeof(TaskNode))) == NULL)
        return NULL;

    list_Obj->nTCnt = ticks();
    list_Obj->pTask = tcb;

    return list_Obj;
}

TaskNode *firstTask(TaskList *list)
{
    return list->pHead->pNext;
}

TaskNode *lastTask(TaskList *list)
{
    return list->pHead->pPrevious;
}

exception findTask(TaskList *list, TCB *task)
{
    listobj *temp = list->pHead;
    while (temp != NULL)
    {
        if (temp->pTask == task)
            return OK;
        if (temp->pNext == NULL)
            break;
        temp = temp->pNext;
    }
    return FAIL;
}

exception removeTCB(TaskList *list, TCB *task)
{

    if (task)
    {
        if (task == list->pHead->pTask && task == list->pTail->pTask)
        {
            list->pHead->pTask = NULL;
            list->pTail->pTask = NULL;
        }

        else if (task == list->pTail->pTask)
        {
            list->pTail->pTask = PreviousTask;
            PreviousTask = NULL;
        }

        else if (task == list->pHead->pTask)
        {
            list->pHead->pTask = NextTask;
            NextTask = NULL;
        }
        else
        {
            TCB *p = PreviousTask;
            TCB *n = NextTask;
            PreviousTask = n;
            NextTask = p;
        }

        memoryFree(task);
        return OK;
    }
    return FAIL;
}

exception removeTask(TaskNode *task, TaskList *list)
{

    if (task)
    {
        if (task == list->pHead && task == list->pTail)
        {
            list->pHead = NULL;
            list->pTail = NULL;
        }

        else if (task == list->pTail)
        {
            list->pTail = task->pPrevious;
            task->pPrevious = NULL;
        }

        else if (task == list->pHead)
        {
            list->pHead = task->pNext;
            task->pNext = NULL;
        }
        else
        {
            TaskNode *p = task->pPrevious;
            TaskNode *n = task->pNext;
            p->pNext = n;
            n->pPrevious = p;
        }

        memoryFree(task->pTask);
        memoryFree(task);
        return OK;
    }
    return FAIL;
}

exception moveListObj(list *firstList, list *secondList, listobj* obj)
{
    if(findTask(firstList, obj->pTask)==FAIL){return FAIL;}
    if(findTask(secondList,obj->pTask)==OK){return OK;}
    if(obj == firstList->pHead)
    {
        firstList->pHead = firstList->pHead->pNext;
        firstList->pHead->pPrevious = NULL;
    }
    if(obj == firstList->pTail)
    {
        firstList->pTail = firstList->pTail->pPrevious;
        firstList->pTail->pNext = NULL;
    }
    else
    {
        obj->pNext->pPrevious = obj->pPrevious;
        obj->pPrevious->pNext = obj->pNext;
    }
    addTaskToList(secondList, obj->pTask);
    memoryFree(obj);

}
