#ifndef VECTOR_H
# define VECTOR_H

# define MIN_CAPACITY 16

#include <stddef.h>

struct vector
{
  size_t size;
  size_t capacity;
  void** data;
};

struct vector* v_create(void);
size_t v_size(struct vector* v);
int v_is_empty(struct vector* v);
size_t v_append(struct vector* v, void* elmt);
void *remove_last(struct vector* v);
void* v_get(struct vector* v, size_t i);
void v_set(struct vector* v, size_t i, void* elmt);
void v_swap(struct vector* v, size_t i, size_t j);
void* v_top(struct vector* v);
void* v_remove_last(struct vector* v);
void v_destroy(struct vector* v);

#endif /* !VECTOR_H */
