#include "data.h"

listobj* create_listobj(TCB *task);
exception app_task_tolist(list *taskList, TCB *task);
list* create_task_list(void);


list* create_task_list(void)
{
    list *task_list = allocSafe(sizeof(list));
    if (task_list == NULL)
        return NULL;

    /*  listobj *object = allocSafe(sizeof(listobj));
    if (object == NULL)
        return NULL;

    object->pNext = object;
    object->pNext = object;
    task_list->pHead = object;
    task_list->pTail = object;
 */
    return task_list;
}

exception app_task_tolist(list *taskList, TCB *task)
{
    listobj *list_obj;

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

        taskList->pTail->pNext = list_obj;
        taskList->pTail = taskList->pTail->pNext;
    }
    return SUCCESS;
}

listobj* create_listobj(TCB *task)
{
    listobj* list_Obj;

    if ((list_Obj = allocSafe(sizeof(listobj))) == NULL)
        return NULL;

    list_Obj->nTCnt = ticks();
    list_Obj->pTask = task;

    return list_Obj;
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