#ifndef ARITHMETIC_H
# define ARITHMETIC_H

enum a_exp_type
{
  ID = -1,
  PLUS,         // 0
  MINUS,        // 1
  TIMES,        // 2
  DIV,          // 3
  POW,          // 4
  OP_BRAKET,    // 5
  CL_BRAKET,    // 6
  UMINUS,       // 7
  UPLUS         // 8
};

struct a_token
{
  enum a_exp_type type;
  long int val;
};

char* arithmetic_expansion(char* exp);

#endif /* !ARITHMETIC_H */
