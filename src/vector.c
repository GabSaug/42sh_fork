#include <stdlib.h>

#include "vector.h"
#include "my_malloc.h"

struct vector* v_create(void)
{
  struct vector* new_vector = my_malloc(sizeof (struct vector));
  new_vector->size = 0;
  new_vector->capacity = MIN_CAPACITY;
  new_vector->data = my_malloc(MIN_CAPACITY * sizeof (void*));
  return new_vector;
}

size_t v_size(struct vector* v)
{
  return v->size;
}

int v_is_empty(struct vector* v)
{
  return v_size(v) == 0;
}

size_t v_append(struct vector* v, void* elmt)
{
  if (v->size + 1 >= v->capacity)
  {
    v->data = realloc(v->data, v->capacity * 2 * sizeof (void*));
    v->capacity *= 2;
  }
  v->data[v->size] = elmt;
  v->size += 1;
  return v->size - 1;
}

void* v_get(struct vector* v, size_t i)
{
  if (i >= v->size)
    return NULL;
  return v->data[i];
}

void v_set(struct vector* v, size_t i, void* elmt)
{
  if (i < v->size)
    v->data[i] = elmt;
}

void v_swap(struct vector* v, size_t i, size_t j)
{
  if (i >= v->size || j >= v->size)
    return;
  void* tmp = v->data[i];
  v->data[i] = v->data[j];
  v->data[j] = tmp;
}

void* v_top(struct vector* v)
{
  if (v->size == 0)
    return NULL;
  return v->data[0];
}

void* v_remove_last(struct vector* v)
{
  if (v->size == 0)
    return NULL;
  v->size--;
  void* res = v->data[v->size];
  if (v->size < v->capacity / 2 + 1)
  {
    v->data = realloc(v->data, v->capacity / 2 * sizeof (void*));
    v->capacity /= 2;
  }
  return res;
}

void v_destroy(struct vector* v)
{
  if (v->data != NULL)
    free(v->data);
  free(v);
}
