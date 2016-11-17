#ifndef PARSER_H
# define PARSER_H

# include "rules.h"
# include "lexer.h"

struct tree* parse(struct rule** rules, struct vector* v_token);

#endif /* !PARSER_H */

