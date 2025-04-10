#include <stdlib.h>

#include "vector.h"

void v_set(struct vector* v, size_t i, void* elmt)
{
  if (i < v->size)
    v->data[i] = elmt;
}

void v_concat(struct vector* v1, struct vector* v2)
{
  for (size_t i = 0; i < v2->size; ++i)
    v_append(v1, v_get(v2, i));
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

void v_erase(struct vector *v)
{
  v->size = 0;
  v->data = realloc(v->data, MIN_CAPACITY * sizeof (void*));
  v->capacity = MIN_CAPACITY;
}

void v_destroy(struct vector* v, void (*free_elt) (void*))
{
  if (!v)
    return;
  if (free_elt)
  {
    for (size_t i = 0; i < v_size(v); ++i)
      free_elt(v_get(v, i));
  }
  free(v->data);
  free(v);
}
