#ifndef ARITHMETIC_H
# define ARITHMETIC_H

# include <unistd.h>

# include "hash_table.h"
# include "vector.h"
# include "stack.h"

/**
 *  \brief Perform arithmetic expansion on a given string
 *  \param exp The string to expand
*/
char* arithmetic_expansion(char* exp, struct hash_table *ht[]);

/**
 *  \brief Add a token in the token vector (numbers and operators)
 *  \param v_tok The vector of tokens
 *  \param v_tok The arithmetic epression in an string
 *  \param v_tok The starting index of the new token (included)
 *  \param v_tok The ending index of the new token (excluded)
*/
int add_tok(struct vector* v_tok, char* exp, size_t start, size_t end,
            struct hash_table *ht[]);

/**
 *  \brief Return 1 if a character is an operator character
 *  \param c The char to test
*/
int is_in_op(char c);

/**
 *  \brief Return 1 if a character is an expansion charecter
 *  \param c The char to test
*/
int is_in_exp(char c);

/**
 *  \brief Return 1 if an operator is unary
 *  \param op The operator to test
*/
int is_unary(enum a_exp_type op);

/**
 *  \brief Return the priority of an operator
 *  \param op The operator
*/
int priority(enum a_exp_type op);

/**
 *  \brief Compute the result on an operation
 *  \param operand1 The left operand
 *  \param op The left operand
 *  \param operand2 The right operand
*/
long int compute_simple_op(long int operand1, enum a_exp_type op,
                           long int operand2);
int lexer_loop(char* exp, ssize_t* start_tok, int* in_tok,
               struct vector* v_tok, size_t* i, struct hash_table *ht[]);
int match_op(char* str);
struct a_token* create_tok(char* str);
void a_v_append(struct vector* v, struct a_token* tok);
struct vector* a_lexer(char* exp, struct hash_table *ht[]);

/**
 *  \brief Return if a token is an operator (not a braket)
 *  \param op The operator
*/
int is_operator(enum a_exp_type op);
int eval_loop(struct a_token* tok, int* unary, int* last_num,
              stack_operator** s_operator, stack_result** s_result);

#endif /* !ARITHMETIC_H */
