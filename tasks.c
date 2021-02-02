#include "data.h"

#pragma region three
TaskNode *create_listobj(TCB *task);
exception Add_task_tolist(list *taskList, TCB *task);
int compare_listobj(TaskNode *obj1, TaskNode *obj2);
TaskList *create_task_list(void);
void deleteTask(TaskNode* task);
TaskNode* removeTask(TaskNode* task);
#pragma endregion three

TaskList *create_task_list(void)
{
    TaskList *task_list = allocSafe(sizeof(TaskList));
    if (task_list == NULL)
        return NULL;

    /*  TaskNode *object = allocSafe(sizeof(TaskNode));
    if (object == NULL)
        return NULL;

    object->pNext = object;
    object->pPrevious = object;
    task_list->pHead = object;
    task_list->pTail = object;
  */
    return task_list;
}

exception Add_task_tolist(TaskList *taskList, TCB *task)
{
    TaskNode *list_obj;
    TaskNode *temp;

    if (taskList->pHead == NULL)
    {

        list_obj = create_listobj(task);

        if (list_obj == NULL)
            return FAIL;

        taskList->pHead = list_obj;
        taskList->pTail = taskList->pHead;
    }
    else
    {
        list_obj = create_listobj(task);
        if (list_obj == NULL)
            return FAIL;

        temp = taskList->pHead;

        while (1)
        {
            if (temp == taskList->pTail && !compare_listobj(temp, list_obj))
            {
                temp->pNext = list_obj;
                taskList->pTail = list_obj;
                list_obj->pPrevious = temp;
                return OK;
            }

            else if (compare_listobj(temp, list_obj))
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

int compare_listobj(TaskNode *obj1, TaskNode *obj2)
{

    if((obj1->pTask->Deadline) > (obj2->pTask->Deadline))
    {
    return OK;
    }

    else
    {
    return FAIL;
    }
}

TaskNode *create_listobj(TCB *task)
{
    TaskNode *list_Obj;

    if ((list_Obj = allocSafe(sizeof(TaskNode))) == NULL)
        return NULL;

    /* task->Deadline = ticks() + d;
    task->PC = task;
    task->SP = &task->StackSeg[STACK_SIZE - 1]; */

    list_Obj->nTCnt = ticks();
    list_Obj->pTask = task;

    return list_Obj;
}

void deleteTask(TaskNode* task)
{
    safeData_free(task->pTask);
    safeData_free(task);
}

TaskNode* removeTask(TaskNode* task)
{
    if (task) {
        TaskNode* p   = task->pPrevious;
        TaskNode* n   = task->pNext;
        p->pNext            = n;
        n->pPrevious        = p;
    }
    return task;
}


/* list* allocTask(void (*body)(), uint d) {
    list* task = NULL;
    TCB*      tcb  = NULL;

    if (task = allocSafe(sizeof *task), !task)  { return NULL; }
    if (tcb  = allocSafe(sizeof *tcb),  !tcb)   { safeData_free(task); return NULL; }

    tcb->DeadLine   = ticks() + d;
    tcb->PC         = body;
    tcb->SP         = &tcb->StackSeg[STACK_SIZE - 1];

    task->nTCnt     = ticks();
    task->pTask     = tcb;

    return task;
} */