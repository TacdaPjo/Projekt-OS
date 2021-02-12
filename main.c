#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "kernel.h"
#include "memory_manager.c"
//#include "testsreq/test.c"
#include "testsreq/testmsg.c"

//CTRL+ALT+L == FORMAT
//CTRL+ALT+F == FULLSCREEN



void main()
{
  SystemInit();
  SysTick_Config(100000);
  SCB->SHP[((uint32_t)(SysTick_IRQn)&0xF) - 4] = (0xE0);
  isr_off();

  //labOneTest();
  labTwoTest();

  while(1)
  {
  }

}
