#include <stdlib.h>

#include <stdio.h>
#include "lexer.h"

#include "vector.h"
#include "my_malloc.h"

static char ts2string[][20] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&", "<>",
  "<<-", ">|", "{", "}", "(", ")", "!", "EOF",
  "IF", "THEN", "ELSE", "ELIF", "FI", "DO", "DONE", "CASE", "ESAC", "WHILE",
  "UNTIL", "FOR", "IN", "FUNCTION", "<NEW_LINE>",
  "WORD", "ASSIGNMENT_WORD", "NAME", "IO_NUMBER"
};

void v_print(struct vector* v)
{
  for (size_t i = 0; i < v->size; ++i)
  {
    struct token* token = v_get(v, i);
    if (token->id != WORD)
      printf("%s\n", token->id >= 0 ? ts2string[token->id] : "UNDIFINED");
    else
      printf("WORD = %s\n", token->s);
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
