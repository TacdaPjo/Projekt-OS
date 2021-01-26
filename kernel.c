#include "kernel.h"
#include "dlist.h"
#include "limits.h"
#include "assert.h"

list *timerList, *waitList, *readyList;

exception init_kernel()
{
    set_ticks(0);
    timerList = create_list();
    if (timerList == NULL)
    {
        remove_list(timerList);
    }
}