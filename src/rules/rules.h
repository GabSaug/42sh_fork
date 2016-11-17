#ifndef RULES_H
# define RULES_H

//# include "parser.h"
# include "lexer.h"
# include "my_malloc.h"

/**
 * \file rules.h
*/

enum non_terminal_symbol
{
  INPUT,
  LIST,
  COM_AMP,
  AND_OR,
  BOOL_OP,
  NEW_LINE_RULE,
  PIPELINE,
  BANG_RULE,
  COMMAND,
  SIMPLE_COMMAND,
  SHELL_COMMAND,
  FUNCDEC,
  REDIRECTION,
  IO_NUMBER_RULE,
  PREFIX,
  ELEMENT,
  COMPOUND_LIST,
  RULE_FOR,
  WORD_RULE,
  RULE_WHILE,
  RULE_UNTIL,
  RULE_CASE,
  RULE_IF,
  ELSE_CLAUSE,
  DO_GROUP,
  CASE_CLAUSE,
  DSEMI_RULE,
  CASE_ITEM,
  L_PAR_RULE,
  // Auxilary function
  LIST_AUX,
  AND_OR_AUX,
  PIPELINE_AUX,
  COMPOUND_LIST_AUX,
  COMPOUND_LIST_AUX_2,
  FOR_RULE_AUX,
  FOR_RULE_AUX_2,
  CASE_CLAUSE_AUX,
  CASE_ITEM_AUX,
  NB_RULE
};

enum repeat
{
  MANDATORY,
  STAR,
  OPTIONAL,
  PLUS
};

struct symbol
{
  int terminal; // if true read only terminal_symbol
  enum repeat repeat; // mandatory, optional, star, plus
  enum non_terminal_symbol rule;
  enum terminal_symbol terminal_symbol;
};

struct simple_rule
{
  struct symbol* sym_arr;
  size_t nb_sym;
};

struct rule
{
  struct simple_rule* s_r;
  size_t nb_s_r;
};

void create_sym(struct symbol* sym, int terminal, enum repeat repeat,
                enum non_terminal_symbol rule,
                enum terminal_symbol terminal_symbol);
void rules_destroy(struct rule** rules);
/**
 * \brief Create the array of rules
 * \return The array of rules
*/
struct rule** init_all_rules(void);
struct rule** init_all_rules2(struct rule **rules);
struct rule** init_all_rules3(struct rule **rules);
struct rule** init_all_rules4(struct rule **rules);
struct rule** init_all_rules5(struct rule **rules);
struct rule** init_all_rules6(struct rule **rules);

#endif /* !RULES_H */
