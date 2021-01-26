#include "includes.h"

uint8_t set_task_nr = 0;
uint8_t task = 0;

uint32_t f1 = 0;
uint32_t f2 = 0;
uint32_t f3 = 0;

uint_fast32_t stack_f1[40];
uint_fast32_t stack_f2[40];
uint_fast32_t stack_f3[40];
uint32_t *sp_f1 = &stack_f1[40];
uint32_t *sp_f2 = &stack_f2[40];
uint32_t *sp_f3 = &stack_f3[40];

#pragma region Functions
void function1();
void function2();
void function3();
#pragma endregion Functions

#define CLKSPEED 83999
#define MAXTICKS 10
volatile uint32_t tickss = 0;

void init_tasks()
{

    *(--sp_f1) = 0x21000000;                // PSR
    *(--sp_f1) = (uint_fast32_t)&function1; // PC
    *(--sp_f1) = 0xEU;                      // LR
    *(--sp_f1) = 0xCU;                      // R12
    *(--sp_f1) = 0x3U;                      // R3
    *(--sp_f1) = 0x2U;                      // R2
    *(--sp_f1) = 0x1U;                      // R1
    *(--sp_f1) = 0x0U;                      // R0

    *(--sp_f2) = 0x21000000;                // PSR
    *(--sp_f2) = (uint_fast32_t)&function2; // PC
    *(--sp_f2) = 0xEU;                      // LR
    *(--sp_f2) = 0xCU;                      // R12
    *(--sp_f2) = 0x3U;                      // R3
    *(--sp_f2) = 0x2U;                      // R2
    *(--sp_f2) = 0x1U;                      // R1
    *(--sp_f2) = 0x0U;                      // R0

    *(--sp_f3) = 0x21000000;                // PSR
    *(--sp_f3) = (uint_fast32_t)&function3; // PC
    *(--sp_f3) = 0xEU;                      // LR
    *(--sp_f3) = 0xCU;                      // R12
    *(--sp_f3) = 0x3U;                      // R3
    *(--sp_f3) = 0x2U;                      // R2
    *(--sp_f3) = 0x1U;                      // R1
    *(--sp_f3) = 0x0U;                      // R0
}

void systickone_config()
{

    SysTick_Config(CLKSPEED);
}

void SysTick_Handler()
{
    tickss++;
    if (tickss == MAXTICKS)
    {
        tickss = 0;
        if (set_task_nr == 1)
        {
            set_task_nr = 2;
            __set_SP((unsigned int)sp_f1);
        }
        else if (set_task_nr == 2)
        {
            set_task_nr = 3;
            __set_SP((unsigned int)sp_f2);
        }
        else if (set_task_nr == 3)
        {
            set_task_nr = 1;
            __set_SP((unsigned int)sp_f3);
        }
    }
}

void function1()
{

    while (1)
    {
        f1++;
    }
}

void function2()
{

    while (1)
    {
        f2++;
    }
}

void function3()
{

    while (1)
    {
        f3++;
    }
}