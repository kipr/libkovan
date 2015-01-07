#include "kovan.h"

void my_msleep(long msecs)
{
  (*g_msleep_func_default)(msecs * 10);
}

__attribute__((constructor))
static void setFunctions()
{
  g_msleep_func = &my_msleep;
}

int main()
{
  msleep(1000);
  
  return 0;
}