#include "stdint.h"

typedef void (*OSThreadHandler)();

typedef struct
{
    void *sp; // stack pointer
    // other attributes associated witha thread
} OSThread;

void OSThread_start(OSThread *me, OSThreadHandler threadHandler, void *stkSto, uint32_t stkSize)
{
    //Round down the stack top to the 8-byte boundary
    //Stack grows down from high to low memory
    uint32_t *sp = (uint32_t *)((((uint32_t)stkSto + stkSize) / 8) * 8);
    uint32_t *stk_limit;

    *(--sp) = 0x21000000;                    // PSR
    *(--sp) = (uint_fast32_t)&threadHandler; // PC
    *(--sp) = 0xEU;                          // LR
    *(--sp) = 0xCU;                          // R12
    *(--sp) = 0x3U;                          // R3
    *(--sp) = 0x2U;                          // R2
    *(--sp) = 0x1U;                          // R1
    *(--sp) = 0x0U;                          // R0
    /* additionally, fake register R4-R11 */
    *(--sp) = 0x000000BU; // R11
    *(--sp) = 0x000000AU; // R10
    *(--sp) = 0x0000009U; // R9
    *(--sp) = 0x0000008U; // R8
    *(--sp) = 0x0000007U; // R7
    *(--sp) = 0x0000006U; // R6
    *(--sp) = 0x0000005U; // R5
    *(--sp) = 0x0000004U; // R4

    //Save the top of the stack in the thread's attibute
    me->sp = sp;
    //round up the bottom of the stack to the 8-byte boundary
    stk_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) / 8) + 1U) * 8);

    //pre-fill the unused part of the stack with 0xDEADBEEF (decimal 3735928559)
    for (sp = sp - 1U; sp >= stk_limit; --sp)
    {
        *sp = 0xDEADBEEFU;
    }
}
