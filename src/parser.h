#ifndef PARSER_H
# define PARSER_H

enum non_terminal_symbol
{
  INPUT,
  LIST,
  AND_OR,
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


  /*rules_list[LIST][0] =
{
  and_or,
  kleen_star {and_or},
  bracket {';', '&'}
}

rules_list[AND_OR][0] =
{
  pipeline,
  kleen_start {kleen_start {'\n', pipeline} }
}

rules_list[COMMAND]
[0] = { simple_command }
[1] =
{
  shell_command,
  kleen_star {redirection}
}
[2] = ...

rule_list[RULE_IF] =
{
  "if",
  compound_list,
  do_group
}*/


#endif /* !PARSER_H */



