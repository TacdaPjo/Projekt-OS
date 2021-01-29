#include "kernel.h"
#include "dlisted.h"

list *timerList, *waitList, *readyList;

exception init_kernel()
{
    set_ticks(0);
    timerList = createList();
    if (timerList == NULL)
    {
        remove_list(timerList);
    }
}