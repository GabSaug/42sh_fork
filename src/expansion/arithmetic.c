#include <stdlib.h>
#include <err.h>
#include <stdio.h>

#include "arithmetic.h"
#include "vector.h"
#include "my_string.h"
#include "stack.h"
#include "my_malloc.h"
#include "my_math.h"

static const int size_operators = 7;
static char* operators[10] =
{
  "+",
  "-",
  "*",
  "/",
  "**",
  "(",
  ")"
};

static int is_in_op(char c)
{
//  int res = 0;
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

static inline int is_unary(char op)
{
  return op == UPLUS || op == UMINUS;
}

static int priority(enum a_exp_type op)
{
  if (op == OP_BRAKET || op == CL_BRAKET)
    return 0;
  else if (op == UPLUS || op == UMINUS)
    return 4;
  else if (op == POW)
    return 3;
  else if (op == TIMES || op == DIV)
    return 2;
  else if (op == PLUS || op == MINUS)
    return 1;
  return 0;
}

static long int compute(long int operand1, enum a_exp_type op, long int operand2)
{
  if (op == UPLUS)
    return operand2;
  else if (op == UMINUS)
    return -operand2;
  else if (op == PLUS)
    return operand1 + operand2;
  else if (op == MINUS)
    return operand1 - operand2;
  else if (op == TIMES)
    return operand1 * operand2;
  else if (op == DIV)
  {
    if (operand2 == 0)
    {
      warnx("%li/%li: division by zero (error token is \"%li\")", operand1,
            operand2, operand2);
      return 0;
    }
    return operand1 / operand2;
  }
  else if (op == POW)
    return my_pow(operand1, operand2);
  else
    return 0;
}

static int pop_and_eval(stack_operator **ptr_operator, stack_result **ptr_result)
{
  if (!(*ptr_result))
  {
    warnx("Expansion error");
    return 0;
  }
  long int operand2 = stack_r_pop(ptr_result);
  enum a_exp_type op = stack_o_pop(ptr_operator);
  long int operand1 = 0;
  if (!is_unary(op))
  {
    if (!(*ptr_result))
    {
      warnx("Expansion error");
      return 0;
    }
    operand1 = stack_r_pop(ptr_result);
  }
  long int res = compute(operand1, op, operand2);
  stack_r_push(ptr_result, res);
  return 1;
}

static struct a_token* a_create_tok(char* exp, size_t start, size_t end)
{
  char* str = exp + start;
  char end_c = exp[end];
  int op_num = -1;
  long int num;
  exp[end] = '\0';
  //printf("tok = [%s]\n", str);
  for (int i = 0; i < size_operators && op_num == -1; i++)
  {
    if (my_strcmp(operators[i], str))
    {
      //printf("Operator %i\n", i);
      op_num = i;
    }
  }
  if (op_num == -1)
    num = atol(str);
  struct a_token* token = my_malloc(sizeof (struct a_token));
  token->type = op_num;
  token->val = num;
  exp[end] = end_c;
  return token;
}

static void a_v_append(struct vector* v, struct a_token* tok)
{
  size_t size = v_size(v);
  struct a_token* prev = v_get(v, size - 1);
  if (size > 0 && tok->type == TIMES && prev->type == TIMES)
  {
    prev->type = POW;
    free(tok);
  }
  else
    v_append(v, tok);
}

static struct vector* a_lexer(char* exp)
{
  struct vector* v_tok = v_create();
  size_t start_tok = 0;
  int in_tok = 0;
  size_t i;
  for (i = 0; exp[i]; i++)
  {
    if (in_tok)
    {
      if (exp[i] == ' ' || exp[i] == '\n' || exp[i] == '\t')
      {
        in_tok = 0;
        struct a_token* tok = a_create_tok(exp, start_tok, i);
        a_v_append(v_tok, tok);
      }
      else if (i != 0 && ((is_in_op(exp[i]) && !is_in_op(exp[i - 1]))
               || is_in_op(exp[i - 1])))
      {
        struct a_token* tok = a_create_tok(exp, start_tok, i);
        start_tok = i;
        a_v_append(v_tok, tok);
      }
    }
    else
      if (!(exp[i] == ' ' || exp[i] == '\n' || exp[i] == '\t'))
      {
        in_tok = 1;
        start_tok = i;
      }
  }
  if (!(exp[i - 1] == ' ' || exp[i - 1] == '\n' || exp[i - 1] == '\t'))
  {
    struct a_token* tok = a_create_tok(exp, start_tok, i);
    v_append(v_tok, tok);
  }
  return v_tok;
}

static long int a_eval(struct vector* v_tok)
{
  stack_operator *s_operator = NULL;
  stack_result *s_result = NULL;
  int unary = 1;
  for (size_t i = 0; v_get(v_tok, i); ++i)
  {
  //  printf("token %zu\n", i);
    struct a_token* tok = v_get(v_tok, i);
    if (unary)
    {
      if (tok->type == MINUS)
        tok->type = UMINUS;
      else if (tok->type == PLUS)
        tok->type = UPLUS;
    }
    if (tok->type == ID)
    {
      stack_r_push(&s_result, tok->val);
      unary = 0;
    }
    else if (tok->type == OP_BRAKET)
    {
      stack_o_push(&s_operator, OP_BRAKET);
      unary = 1;
    }
    else if (tok->type == PLUS || tok->type == MINUS || tok->type == TIMES
             || tok->type == DIV || tok->type == UPLUS || tok->type == UMINUS
             || tok->type == POW)
    {
      while (s_operator && priority(tok->type) <= priority(stack_o_peek(s_operator)))
        if (!pop_and_eval(&s_operator, &s_result))
        {
	        warnx("Expansion error");
            return 0;
        }
      stack_o_push(&s_operator, tok->type);
      unary = 1;
    }
    else if (tok->type == CL_BRAKET)
    {
      while (s_operator && stack_o_peek(s_operator) != OP_BRAKET)
        if (!pop_and_eval(&s_operator, &s_result))
        {
          warnx("Expansion error");
            return 0;
        }
      stack_o_pop(&s_operator);
      unary = 0;
    }
  }
  while (s_operator)
    pop_and_eval(&s_operator, &s_result);
  if (stack_r_size(s_result) != 1)
  {
    warnx("Expansion error");
    return 0;
  }
  long int result = stack_r_peek(s_result);
  stack_o_destroy(&s_operator);
  stack_r_destroy(&s_result);
  return result;
}

char* arithmetic_expansion(char* s)
{
  if (my_strlen(s))
  {
    char *res = malloc(2);
    res[0] = '0';
    res[1] = '\0';
    return res;
  }
  struct vector* v_tok = a_lexer(s);
  long int res = a_eval(v_tok);
  char* s_res = malloc(50); // Change to dynamic allocation
  sprintf(s_res, "%ld", res);
  free(s);
  return s_res;
}
