#ifndef PARSER_H
# define PARSER_H

enum non_terminal_symbol
{
  input,
  list,
  and_or,
  pipeline,
  command,
  simple_command,
  shell_command,
  funcdec,
  redirection,
  prefix,
  element,
  compound_list,
  rule_for,
  rule_while,
  rule_until,
  rule_case,
  rule_if,
  else_clause,
  do_group,
  case_clause,
  case_item
};

struct symbol
{
  int terminal; // if true read only terminal_symbol
  enum repeat repeat; // mandatory, optional, star
  struct rule* rule;
  enum terminal_symbol;
};

struct rule
{
  enum symbol* symbol_array;
}


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
