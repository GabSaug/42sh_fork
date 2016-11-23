#include <stdlib.h>

#include "stack.h"
#include "arithmetic.h"
#include "my_malloc.h"

void stack_o_push(stack_operator **ptr_s, enum a_exp_type data)
{
  if (!ptr_s)
    return;
  stack_operator *s = *ptr_s;
  stack_operator *new = my_malloc(sizeof (stack_operator));
  if (!new)
    return;
  new->data = data;
  new->next = s;
  *ptr_s = new;
}

enum a_exp_type stack_o_pop(stack_operator **ptr_s)
{
  if (!ptr_s)
    return 0;
  stack_operator *s = *ptr_s;
  stack_operator *new_head = s->next;
  enum a_exp_type data = s->data;
  free(s);
  *ptr_s = new_head;
  return data;
}

enum a_exp_type stack_o_peek(stack_operator *s)
{
  return s->data;
}

int stack_o_size(stack_operator *s)
{
  int len = 0;
  for (stack_operator *tmp = s; tmp; tmp = tmp->next, ++len)
    continue;
  return len;
}

void stack_o_destroy(stack_operator **ptr_s)
{
  while (stack_o_size(*ptr_s) > 0)
    stack_o_pop(ptr_s);
  *ptr_s = NULL;
}

void stack_r_push(stack_result **ptr_s, long int data)
{
  if (!ptr_s)
    return;
  stack_result *s = *ptr_s;
  stack_result *new = my_malloc(sizeof (stack_result));
  if (!new)
    return;
  new->data = data;
  new->next = s;
  *ptr_s = new;
}

long int stack_r_pop(stack_result **ptr_s)
{
  if (!ptr_s)
    return 0;
  stack_result *s = *ptr_s;
  stack_result *new_head = s->next;
  long int data = s->data;
  free(s);
  *ptr_s = new_head;
  return data;
}

long int stack_r_peek(stack_result *s)
{
  return s->data;
}

int stack_r_size(stack_result *s)
{
  int len = 0;
  for (stack_result *tmp = s; tmp; tmp = tmp->next, ++len)
    continue;
  return len;
}

void stack_r_destroy(stack_result **ptr_s)
{
  //while (stack_r_size(*ptr_s) > 0)
  //  free(stack_r_pop(ptr_s));
  *ptr_s = NULL;
}
