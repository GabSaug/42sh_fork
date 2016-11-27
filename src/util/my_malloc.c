#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"

void *my_malloc(size_t size)
{
  void* ptr = calloc(size, 1);
  if (ptr == NULL)
    exit(1);
  return ptr;
}
