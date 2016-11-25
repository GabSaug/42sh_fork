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
  BW_AND,       // 5
  BW_OR,        // 6
  OP_BRAKET,    // 7
  CL_BRAKET,    // 8  
  TILDE,        // 9
  BW_XOR,        // 9
  UMINUS,       // 10
  UPLUS,        // 11
};

struct a_token
{
  enum a_exp_type type;
  long int val;
};

char* arithmetic_expansion(char* exp);

#endif /* !ARITHMETIC_H */
