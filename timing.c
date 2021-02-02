#include "data.h"

#pragma region twice
exception wait(uint nTicks);
void set_ticks(uint no_of_ticks);
uint ticks(void);
uint deadline(void);
void set_deadline(uint nNew);
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

exception wait(uint nTicks)
{
    


}

uint deadline(void)
{
}

void set_deadline(uint nNew)
{
}
