#ifndef STACK_H
# define STACK_H

#include <stddef.h>

enum a_exp_type
{
  ID = -1,
  PLUS,         // 0
  MINUS,        // 1
  TIMES,        // 2
  DIV,          // 3
  POW,          // 4
  BW_AND,       // 5
  BW_OR,        // 6
  OP_BRAKET,    // 7
  CL_BRAKET,    // 8  
  TILDE,        // 9
  BW_XOR,       // 9
  UMINUS,       // 10
  UPLUS,        // 11
};

struct a_token
{
  enum a_exp_type type;
  long int val;
};

typedef struct stack_operator
{
  enum a_exp_type data;
  struct stack_operator *next;
} stack_operator;

void stack_o_push(stack_operator **ptr_s, enum a_exp_type data);
enum a_exp_type stack_o_pop(stack_operator **ptr_s);
enum a_exp_type stack_o_peek(stack_operator *s);
int stack_o_size(stack_operator *s);
void stack_o_destroy(stack_operator **ptr_s);

typedef struct stack_result
{
  long int data;
  struct stack_result *next;
} stack_result;

void stack_r_push(stack_result **ptr_s, long int data);
long int stack_r_pop(stack_result **ptr_s);
long int stack_r_peek(stack_result *s);
int stack_r_size(stack_result *s);
void stack_r_destroy(stack_result **ptr_s);

#endif /* !STACK_H */
