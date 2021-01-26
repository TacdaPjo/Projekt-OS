#include "systick.h"
#include "kernel_functions_march_2019.h"
//CTRL+ALT+L == FORMAT

#pragma region Functions
void init_tasks();
void systickone_config();
#pragma endregion Functions




int main()
{
  systickone_config();
  init_tasks();
  
  


  while (1)
  {
    set_task_nr = 1;
  }

  return 1;
}

void set_task(uint8_t t_nr)
{
}