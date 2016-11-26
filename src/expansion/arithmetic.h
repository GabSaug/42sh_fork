#ifndef ARITHMETIC_H
# define ARITHMETIC_H

# include <unistd.h>

# include "vector.h"
# include "stack.h"

char* arithmetic_expansion(char* exp);
int add_tok(struct vector* v_tok, char* exp, size_t start, size_t end);
int is_in_op(char c);
int is_in_exp(char c);
int is_unary(char op);
int priority(enum a_exp_type op);
long int compute_simple_op(long int operand1, enum a_exp_type op,
                           long int operand2);
int lexer_loop(char* exp, ssize_t* start_tok, int* in_tok,
               struct vector* v_tok, size_t* i);
int match_op(char* str);
struct a_token* create_tok(char* str);
void a_v_append(struct vector* v, struct a_token* tok);
struct vector* a_lexer(char* exp);
int is_operator(enum a_exp_type op);
int eval_loop(struct a_token* tok, int* unary, int* last_num,
              stack_operator** s_operator, stack_result** s_result);

#endif /* !ARITHMETIC_H */
