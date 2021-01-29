#include "kernel.h"


#define ISR_OFF 0x80


extern volatile unsigned int counter=0;


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

void set_ticks(uint no_of_ticks) {
  counter=no_of_ticks;
}

uint ticks(void) {
  return counter;
}