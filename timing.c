#include "data.h"

#pragma region twice
void set_ticks(uint no_of_ticks);
uint ticks(void);
uint deadline(void);
void set_deadline(uint nNew);
exception wait(uint nTicks);
#pragma endregion twice

#pragma region var

#pragma endregion var

void set_ticks(uint nTicks)
{
    tickCounter = nTicks;
}

uint ticks(void)
{
    return tickCounter;
}

uint deadline(void)
{
}

void set_deadline(uint nNew)
{
}


/* exception wait(uint nTicks)
{
    first = TRUE;
    isr_off();
    SaveContext();

    if (first)
    {
        TaskNode* task;
        first = FALSE;
        task = removeTask(task,ReadyList);
        task->nTCnt = ticks() + nTicks;
        addTaskToList(timerList,task);
        Running = firstTask(readyList)->pTask;
        LoadContext();
    } else {
        if (Running->DeadLine <= tickCounter) {
            return DEADLINE_REACHED;
        }
    }
    return OK;
} */