#include "data.h"

#pragma region twice
void set_ticks(uint nTicks);
uint ticks(void);
uint deadline(void);
void set_deadline(uint deadline);
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
    return ReadyList->pHead->pTask->Deadline;
}

void set_deadline(uint deadline)
{

    isr_off();

    ReadyList->pHead->pTask->Deadline = deadline;

    PreviousTask = NextTask;

    push(ReadyList, pop(ReadyList));

    NextTask = ReadyList->pHead->pTask;

    SwitchContext();
} 

void TimerInt(void)
{

    tickCounter++;

    check_TimerList();

    check_WaitingList();
}
