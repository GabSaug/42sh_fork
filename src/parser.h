#ifndef PARSER_H
# define PARSER_H

# include "rules.h"
# include "lexer.h"

struct tree* parse(struct rule** rules, struct vector* v_token,
                   enum non_terminal_symbol symbol, int* nb_token_read);

#endif /* !PARSER_H */

