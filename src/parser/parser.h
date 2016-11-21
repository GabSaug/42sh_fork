#ifndef PARSER_H
# define PARSER_H

# include "rules.h"
# include "lexer.h"

/**
 * \file parser.h
*/

/**
 * \brief Parse the list of token to create an AST
 * \return The AST
*/
struct tree* parse(struct rule** rules, struct vector* v_token, int* fit_level);

#endif /* !PARSER_H */

