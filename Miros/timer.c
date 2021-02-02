#include "kernel.h"

#define ISR_OFF 0x80

extern volatile unsigned int count = 0;

void set_ticks(uint nrOfTicks)
{
  count = nrOfTicks;
}

uint ticks(void)
{
  return count;
}

void *callocSec(unsigned int var, unsigned int size)
{
  void *temp;
  uint status = ISR_OFF;
  temp = calloc(var, size);

  if (temp == NULL)
  {

    return NULL;
  }

  return temp;
}

void freeIsOffOn(void *freeObj)
{
  uint status = ISR_OFF;
  free(freeObj);
}
