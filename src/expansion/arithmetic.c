#include <stdlib.h>
#include <err.h>
#include <stdio.h>

#include "arithmetic.h"
#include "vector.h"
#include "my_string.h"
#include "stack.h"
#include "my_malloc.h"

static const int size_operators = 4;
static char* operators[10] =
{
  "-",
  "+",
  "-",
  "*",
  "/",
  "(",
  ")"
};

static int is_in_op(char c)
{
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

static inline int is_unary(char op)
{
  return op == ID || op == UMINUS;
}

static int priority(enum a_exp_type op)
{
  if (op == ID || op == UMINUS)
    return 3;
  else if (op == TIMES || op == DIV)
    return 2;
  else if (op == PLUS || op == MINUS)
    return 1;
  return 0;
}

static long int compute(long int operand1, enum a_exp_type op, long int operand2)
{
  if (op == ID)
    return operand2;
  if (op == UMINUS)
    return -operand2;
  else if (op == PLUS)
    return operand1 + operand2;
  else if (op == MINUS)
    return operand1 - operand2;
  else if (op == TIMES)
    return operand1 * operand2;
  else if (op == DIV)
    return operand1 / operand2;
  else
    return 0;
}

static void pop_and_eval(stack_operator **ptr_operator, stack_result **ptr_result)
{
  long int operand2 = stack_r_pop(ptr_result);
  enum a_exp_type op = stack_o_pop(ptr_operator);
  long int operand1 = 0;
  if (!is_unary(op))
    operand1 = stack_r_pop(ptr_result);
  long int res = compute(operand1, op, operand2);
  stack_r_push(ptr_result, res);
}

static struct a_token* a_create_tok(char* exp, size_t start, size_t end)
{
  char* str = exp + start;
  char end_c = exp[end];
  int op_num = -1;
  long int num;
  exp[end] = '\0';

  for (int i = 0; i < size_operators && op_num == -1; i++)
  {
    if (my_strcmp(operators[i], str))
      op_num = i;
  }
  if (op_num == -1)
    num = atol(str);

  struct a_token* token = my_malloc(sizeof (struct a_token));
  token->type = op_num;
  token->val = num;
  exp[end] = end_c;
  return token;
}

static struct vector* a_lexer(char* exp)
{
  struct vector* v_tok = v_create();
  size_t start_tok = 0;
  int in_tok = 0;
  for (size_t i = 0; exp[i]; i++)
  {
    if (in_tok)
    {
      if (exp[i] == ' ' || exp[i] == '\n' || exp[i] == '\t')
      {
        in_tok = 0;
        struct a_token* tok = a_create_tok(exp, start_tok, i + 1);
        v_append(v_tok, tok);
      }
      else if (i != 0 && ((is_in_op(exp[i - 1]) && !is_in_op(exp[i]))
               || (!is_in_op(exp[i - 1]) && is_in_op(exp[i]))))
      {
        start_tok = i;
        struct a_token* tok = a_create_tok(exp, start_tok, i);
        v_append(v_tok, tok);
      }
    }
    else
      if (!(exp[i] == ' ' || exp[i] == '\n' || exp[i] == '\t'))
      {
        in_tok = 1;
        start_tok = i;
      }
  }
  return v_tok;
}

long int a_eval(struct vector* v_tok)
{
  stack_operator *s_operator = NULL;
  stack_result *s_result = NULL;
  for (size_t i = 0; v_get(v_tok, i); ++i)
  {
    struct a_token* tok = v_get(v_tok, i);
    if (tok->type == OP_BRAKET)
      stack_o_push(&s_operator, OP_BRAKET);
    else if (tok->type == ID)
      stack_r_push(&s_result, tok->val);
    else if (tok->type == PLUS || tok->type == MINUS || tok->type == TIMES
             || tok->type == DIV)//OPERATOR
    {
      while (s_operator && priority(tok->type) > priority(stack_o_peek(s_operator)))
        pop_and_eval(&s_operator, &s_result);
      stack_o_push(&s_operator, tok->type);
    }
    else if (tok->type == CL_BRAKET)
    {
      while (stack_o_peek(s_operator) != CL_BRAKET)
        pop_and_eval(&s_operator, &s_result);
    }
  }
  while (s_operator)
    pop_and_eval(&s_operator, &s_result);
 // if (stack_o_size(s_operator) != 0)
 //   printf("s_operator is");
  if (stack_r_size(s_result) != 1)
    warn("s_result.size != 1");
  long int result = stack_r_peek(s_result);
  stack_o_destroy(&s_operator);
  stack_r_destroy(&s_result);
  return result;
}

char* arithmetic_expansion(char* exp)
{
  struct vector* v_tok = a_lexer(exp);
  long int res = a_eval(v_tok);
  char* s_res = malloc(50);
  sprintf(s_res, "%ld", res);
  return s_res;
}
