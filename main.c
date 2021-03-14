#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "kernel.h"


//#include "testsreq/test.c" /* lab 1 */

//#include "testsreq/testmsg.c" /* lab 2 */

//#include "testsreq/testTimer.c" /* lab 3 */

#include "testsreq/funcTest.c" /* test of implemented functions */


//CTRL+ALT+L == FORMAT
//CTRL+ALT+F == FULLSCREEN



void main()
{
  
  SystemInit();
  SysTick_Config(100000);
  SCB->SHP[((uint32_t)(SysTick_IRQn)&0xF) - 4] = (0xE0);
  isr_off();

  //labOneTest();
  //labTwoTest();
  //labThreeTest();

    
  /* test functions */
  //testObj();
  //listTest();
 
 
 

  while(1)
  {
  }

}
