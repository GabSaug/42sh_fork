#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>

#include "arithmetic.h"
#include "expansion.h"
#include "vector.h"
#include "my_string.h"
#include "stack.h"
#include "my_malloc.h"
#include "my_math.h"
#include "tokenize.h"

static long int compute(long int operand1, enum a_exp_type op,
                 long int operand2, int* success)
{
  *success = 1;
  if (op == DIV)
  {
    if (operand2 == 0)
    {
      *success = 0;
      warnx("%li/%li: division by zero (error token is \"%li\")", operand1,
            operand2, operand2);
      return 0;
    }
    return operand1 / operand2;
  }
  else if (op == POW)
  {
    if (operand2 < 0)
    {
      *success = 0;
      warnx("%li**%li: exponent less than zero (error token is \"%li\")",
            operand1, operand2, operand2);
      return 0;
    }
    return my_pow(operand1, operand2);
  }
  else
    return compute_simple_op(operand1, op, operand2);
}

static int pop_and_eval(stack_operator **ptr_operator,
                        stack_result **ptr_result)
{
  if (!(*ptr_result))
  {
    warnx("Expansion error: missing operand");
    return 0;
  }
  long int operand2 = stack_r_pop(ptr_result);
  enum a_exp_type op = stack_o_pop(ptr_operator);
  long int operand1 = 0;
  if (!is_unary(op))
  {
    if (!(*ptr_result))
    {
      warnx("Expansion error: missing operand");
      return 0;
    }
    operand1 = stack_r_pop(ptr_result);
  }
  int success;
  long int res = compute(operand1, op, operand2, &success);
  if (!success)
    return 0;
  stack_r_push(ptr_result, res);
  return 1;
}

int add_tok(struct vector* v_tok, char* exp, size_t start, size_t end,
            struct hash_table *ht[])
{
  char* str = exp + start;
  char end_c = exp[end];
  exp[end] = '\0';
  if (!(str[0] == ' ' || str[0] == '\n' || str[0] == '\t'))
  {
    if (is_in_exp(str[0]))
    {
      struct vector* v_new_str;
      char* str2 = my_strdup(str);
      v_new_str = expand(str2, 1, ht);
      if (v_size(v_new_str) == 0)
        a_v_append(v_tok, create_tok("0"));
      for (size_t i = 0; i < v_size(v_new_str); i++)
      {
        char* str = v_get(v_new_str, i);
        a_v_append(v_tok, create_tok(str));
      }
      v_destroy(v_new_str, free);
    }
    else
      a_v_append(v_tok, create_tok(str));
  }
  exp[end] = end_c;
  return 1;
}

static ssize_t new_start_tok(struct vector* v_tok, char* exp, size_t i,
                             struct hash_table *ht[])
{
  if (is_in_exp(exp[i]))
  {
    size_t new_pos = tokenize_expansion(exp + i, 1).size;
    if (new_pos == 0)
      return -1;
    else
    {
      add_tok(v_tok, exp, i, i + new_pos, ht);
      return i + new_pos;
    }
  }
  return i;
}

int lexer_loop(char* exp, ssize_t* start_tok, int* in_tok,
               struct vector* v_tok, size_t* i, struct hash_table *ht[])
{
  if (*in_tok)
  {
    if (exp[*i] == ' ' || exp[*i] == '\n' || exp[*i] == '\t')
    {
      *in_tok = 0;
      add_tok(v_tok, exp, *start_tok, *i, ht);
    }
    else if (*i != 0 && ((is_in_op(exp[*i]) && !is_in_op(exp[*i - 1]))
             || is_in_op(exp[*i - 1]) || is_in_exp(exp[*i])))
    {
      add_tok(v_tok, exp, *start_tok, *i, ht);
      *i = *start_tok = new_start_tok(v_tok, exp, *i, ht);
      if (*start_tok == -1)
        return 0;
    }
  }
  else
    if (!(exp[*i] == ' ' || exp[*i] == '\n' || exp[*i] == '\t'))
    {
      *in_tok = 1;
      *i = *start_tok = new_start_tok(v_tok, exp, *i, ht);
      if (*start_tok == -1)
        return 0;
    }
  return 1;
}

static int eval_looop(struct a_token* tok, int* unary,
                     stack_operator** s_operator, stack_result** s_result)
{
  if (tok->type == OP_BRAKET)
  {
    stack_o_push(s_operator, OP_BRAKET);
    *unary = 1;
  }
  else if (is_operator(tok->type))
  {
    while (*s_operator && priority(tok->type)
           <= priority(stack_o_peek(*s_operator)))
      if (!pop_and_eval(s_operator, s_result))
        return 0;
    stack_o_push(s_operator, tok->type);
    *unary = 1;
  }
  else if (tok->type == CL_BRAKET)
  {
    while (*s_operator && stack_o_peek(*s_operator) != OP_BRAKET)
      if (!pop_and_eval(s_operator, s_result))
        return 0;
    stack_o_pop(s_operator);
    *unary = 0;
  }
  return 1;
}

int eval_loop(struct a_token* tok, int* unary, int* last_num,
              stack_operator** s_operator, stack_result** s_result)
{
  if (*unary)
  {
    if (tok->type == MINUS)
      tok->type = UMINUS;
    else if (tok->type == PLUS)
      tok->type = UPLUS;
  }
  if (tok->type == ID)
  {
    if (*last_num == 1)
    {
      warnx("Expansion error: operator missing");
      return 0;
    }
    *last_num = 1;
    stack_r_push(s_result, tok->val);
    *unary = 0;
  }
  else
    *last_num = 0;
  return eval_looop(tok, unary, s_operator, s_result);
}

static int return_and_free(int ret, stack_operator** s_operator,
                           stack_result** s_result)
{
  stack_o_destroy(s_operator);
  stack_r_destroy(s_result);
  return ret;
}

static int a_eval(struct vector* v_tok, long int* res)
{
  stack_operator* s_operator = NULL;
  stack_result* s_result = NULL;
  int unary = 1;
  int last_num = 0;
  for (size_t i = 0; v_get(v_tok, i); ++i)
  {
    if (!eval_loop(v_get(v_tok, i), &unary, &last_num, &s_operator, &s_result))
      return return_and_free(0, &s_operator, &s_result);
  }
  while (s_operator)
  {
    if (!pop_and_eval(&s_operator, &s_result))
      return return_and_free(0, &s_operator, &s_result);
  }
  if (stack_r_size(s_result) != 1)
  {
    warnx("Expansion error: operator missing");
    return return_and_free(0, &s_operator, &s_result);
  }
  *res = stack_r_peek(s_result);
  stack_o_destroy(&s_operator);
  stack_r_destroy(&s_result);
  return 1;
}

char* arithmetic_expansion(char* s, struct hash_table *ht[])
{
  if (!my_strlen(s))
  {
    free(s);
    char *res = malloc(2);
    res[0] = '0';
    res[1] = '\0';
    return res;
  }
  struct vector* v_tok = a_lexer(s, ht);
  if (!v_tok)
    return NULL;
  long int res;
  int success = a_eval(v_tok, &res);
  v_destroy(v_tok, free);
  free(s);
  if (!success)
    return NULL;
  char* s_res = malloc(50);
  sprintf(s_res, "%ld", res);
  return s_res;
}
