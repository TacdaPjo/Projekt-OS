#include "systick.h"

//CTRL+ALT+L == FORMAT

#pragma region Functions
void init_tasks();
#pragma endregion Functions
int main()
{
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