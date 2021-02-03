#include "data.h"

#pragma region three
TaskNode *createListObj(TCB *task);
exception addTaskToList(list *taskList, TCB *task);
int compareListObjs(TaskNode *obj1, TaskNode *obj2);
TaskList *createTaskList(void);
exception removeTask(TaskNode *task, TaskList *list);
TaskNode *firstTask(TaskList *list);
TaskNode *lastTask(TaskList *list);
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

TaskNode *firstTask(TaskList *list)
{
    return list->pHead->pNext;
}

TaskNode *lastTask(TaskList *list)
{
    return list->pHead->pPrevious;
}
