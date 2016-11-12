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
  COMMAND,
  SIMPLE_COMMAND,
  SHELL_COMMAND,
  FUNCDEC,
  REDIRECTION,
  PREFIX,
  ELEMENT,
  COMPOUND_LIST,
  RULE_FOR,
  RULE_WHILE,
  RULE_UNTIL,
  RULE_CASE,
  RULE_IF,
  ELSE_CLAUSE,
  DO_GROUP,
  CASE_CLAUSE,
  CASE_ITEM,
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
struct rule* init_rule_input(void);
struct rule* init_rule_list(void);
struct rule* init_rule_list_aux(void);
struct rule* init_rule_com_amp(void);
struct rule *init_rule_and_or(void);
struct rule *init_rule_and_or_aux(void);
struct rule *init_rule_bool_op(void);
struct rule *init_rule_new_line(void);
struct rule *init_rule_pipeline(void);

#endif /* !RULES_H */
