#ifndef RULES_H
# define RULES_H

//# include "parser.h"
# include "lexer.h"
# include "my_malloc.h"

enum non_terminal_symbol
{
  INPUT,
  LIST,
  LIST_AUX,
  COM_AMP,
  AND_OR,
  AND_OR_AUX,
  BOOL_OP,
  NEW_LINE_RULE,
  PIPELINE,
  BANG_RULE,
  PIPELINE_AUX,
  COMMAND,
  SIMPLE_COMMAND,
  SHELL_COMMAND,
  FUNCDEC,
  REDIRECTION,
  IO_NUMBER_RULE,
  PREFIX,
  ELEMENT,
  COMPOUND_LIST,
  COMPOUND_LIST_AUX,
  COMPOUND_LIST_AUX_2,
  RULE_FOR,
  FOR_RULE_AUX,
  WORD_RULE
  FOR_RULE_AUX_2,
  RULE_WHILE,
  RULE_UNTIL,
  RULE_CASE,
  RULE_IF,
  ELSE_CLAUSE,
  DO_GROUP,
  CASE_CLAUSE,
  CASE_CLAUSE_AUX,
  DSEMI_RULE,
  CASE_ITEM,
  L_PAR_RULE,
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
struct rule** init_all_rules(void);
struct rule** init_all_rules2(struct rule **rules);
struct rule** init_all_rules3(struct rule **rules);
struct rule** init_all_rules4(struct rule **rules);
struct rule** init_all_rules5(struct rule **rules);
struct rule** init_all_rules6(struct rule **rules);

#endif /* !RULES_H */
