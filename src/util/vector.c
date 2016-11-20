#include <stdlib.h>

#include <stdio.h>
#include "lexer.h"

#include "vector.h"
#include "my_malloc.h"

static char ts2string[][20] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&", "<>",
  "<<-", ">|", "EOF",
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "{", "}", "(", ")", "!", "in", "function", "<new_line>",
  "word", "assignment_word", "name", "IO_number"
};

void v_print(struct vector* v)
{
  for (size_t i = 0; i < v->size; ++i)
  {
    struct token* token = v_get(v, i);

    printf("%s, ", ts2string[token->id]);
  }
  printf("\n");
}

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
