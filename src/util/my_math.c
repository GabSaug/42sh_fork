#include "my_math.h"

long int my_pow(long int n, long int p)
{
  int res = 1;
  for (; p > 0; p--)
    res *= n;
  return res;
}
