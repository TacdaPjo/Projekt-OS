#include "includes.h"
//CTRL+ALT+L == FORMAT

#pragma region Functions
void init_tasks();
void systickone_config();
#pragma endregion Functions




int main()
{
  systickone_config();
  init_tasks();
  
  
  if(init_kernel() != OK)
  {
    set_task_nr = 1;
    while(1);
    
  }

  return 1;
}

void set_task(uint8_t t_nr)
{
}