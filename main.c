#include "systick.h"

//CTRL+ALT+L == FORMAT

#pragma region Functions
void init_tasks();
void set_task(uint8_t);
#pragma endregion Functions
int main()
{
  init_tasks();

  while (1)
  {
    set_task_nr = 1 ;
  }

  return 1;
}

void set_task(uint8_t t_nr)
{
  if(t_nr == 1)
  {
    set_task_nr = 1;
  }
  if(t_nr == 2)
  {
    set_task_nr = 2;
  }
    if(t_nr == 3)
  {
    set_task_nr = 3;
  }
}
