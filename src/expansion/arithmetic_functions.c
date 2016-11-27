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

static const int size_operators = 11;
static char* operators[11] =
{
  "+",
  "-",
  "*",
  "/",
  "**",
  "&",
  "|",
  "(",
  ")",
  "~",
  "^"
};

int is_in_op(char c)
{
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'
         || c == '&' || c == '|' || c == '~';
}

int is_in_exp(char c)
{
  return c == '$' || c == '_' || c == '`' || (c >= 'a' && c <= 'z')
         || (c >= 'A' && c <= 'Z') || c == '"' || c == '\'';
}

int is_unary(char op)
{
  return op == UPLUS || op == UMINUS || op == TILDE;
}

int priority(enum a_exp_type op)
{
  if (op == OP_BRAKET || op == CL_BRAKET)
    return 0;
  else if (op == UPLUS || op == UMINUS || op == TILDE)
    return 9;
  else if (op == POW)
    return 8;
  else if (op == TIMES || op == DIV)
    return 7;
  else if (op == PLUS || op == MINUS)
    return 6;
  else if (op == BW_AND)
    return 5;
  else if (op == BW_XOR)
    return 4;
  else if (op == BW_OR)
    return 3;
  return 0;
}

long int compute_simple_op(long int operand1, enum a_exp_type op,
                           long int operand2)
{
  if (op == UPLUS)
    return operand2;
  else if (op == UMINUS)
    return -operand2;
  else if (op == TILDE)
    return ~operand2;
  else if (op == BW_AND)
    return operand1 & operand2;
  else if (op == BW_OR)
    return operand1 | operand2;
  else if (op == BW_XOR)
    return operand1 ^ operand2;
  else if (op == PLUS)
    return operand1 + operand2;
  else if (op == MINUS)
    return operand1 - operand2;
  else if (op == TIMES)
    return operand1 * operand2;
  return 0;
}

int match_op(char* str)
{
  int op_num = -1;
  for (int i = 0; i < size_operators && op_num == -1; i++)
  {
    if (my_strcmp(operators[i], str))
      op_num = i;
  }
  return op_num;
}

struct a_token* create_tok(char* str)
{
//  printf("\t\t\t\t[%s]\n", str);
  int op_num = match_op(str);
  char* endptr;
  long int num;
  if (op_num == -1)
  {
    num = strtol(str, &endptr, 10);
    if (*endptr)
      num = 0;
  }
  struct a_token* token = my_malloc(sizeof (struct a_token));
  token->type = op_num;
  token->val = num;
  return token;
}

void a_v_append(struct vector* v, struct a_token* tok)
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

struct vector* a_lexer(char* exp, struct hash_table *ht[])
{
  struct vector* v_tok = v_create();
  ssize_t start_tok = 0;
  int in_tok = 0;
  size_t i;
  for (i = 0; i < my_strlen(exp); i++)
    if (!lexer_loop(exp, &start_tok, &in_tok, v_tok, &i, ht))
      return NULL;
  if (exp[i - 1] && !(exp[i - 1] == ' ' || exp[i - 1] == '\n'
      || exp[i - 1] == '\t'))
    add_tok(v_tok, exp, start_tok, i, ht);
  return v_tok;
}

int is_operator(enum a_exp_type op)
{
  return op == PLUS || op == MINUS || op == TIMES
         || op == DIV || op == UPLUS || op == UMINUS
         || op == POW || op == BW_AND || op == BW_OR
         || op == TILDE || op == BW_XOR;
}
