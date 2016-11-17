#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"

void *my_malloc(size_t size)
{
  void* ptr = calloc(size, 1);
  if (ptr == NULL)
  {
    //printf("Impossible to malloc");
    exit(1);
  }
  return ptr;
}
