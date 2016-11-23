#ifndef ARITHMETIC_H
# define ARITHMETIC_H

enum a_exp_type
{
  ID = -1,
  UMINUS,
  PLUS,
  MINUS,
  TIMES,
  OP_BRAKET,
  CL_BRAKET,
  DIV
};

struct a_token
{
  enum a_exp_type type;
  long int val;
};

char* arithmetic_expansion(char* exp);

#endif /* !ARITHMETIC_H */
