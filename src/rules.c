#include "parser.h"
#include "lexer.h"
#include "rules.h"

void create_sym(struct symbol* sym, int terminal, enum repeat repeat,
                enum non_terminal_symbol rule,
                enum terminal_symbol terminal_symbol)
{
  sym->terminal = terminal;
  sym->repeat = repeat;
  sym->rule = rule;
  sym->terminal_symbol = terminal_symbol;
}

struct rule** init_all_rules(void)
{
  struct rule** rules = my_malloc(sizeof(struct rule*) * NB_RULE);
  rules[INPUT] = init_rule_input();
  rules[LIST] = init_rule_list();
  rules[LIST_AUX] = init_rule_list_aux();
  rules[COM_AMP] = init_rule_com_amp();
  rules[AND_OR] = init_rule_and_or();
  rules[AND_OR_AUX] = init_rule_and_or_aux();
  rules[BOOL_OP] = init_rule_bool_op();
  rules[NEW_LINE_RULE] = init_rule_new_line();
  rules[PIPELINE] = init_rule_pipeline();

  return rules;
}

struct rule* init_rule_input(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 4;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * rule->s_r[0].nb_sym);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 1, 0, 0, NEWLINE);

  // Rule 2
  rule->s_r[1].nb_sym = 2;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * rule->s_r[1].nb_sym);
  create_sym(rule->s_r[1].sym_arr + 0, 0, MANDATORY, LIST, 0);
  create_sym(rule->s_r[1].sym_arr + 1, 1, 0, 0, EOF_SYM);

  // Rule 3
  rule->s_r[2].nb_sym = 1;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol) * rule->s_r[2].nb_sym);
  create_sym(rule->s_r[2].sym_arr + 0, 1, 0, 0, NEWLINE);
  
  // Rule 4
  rule->s_r[3].nb_sym = 1;
  rule->s_r[3].sym_arr = my_malloc(sizeof (struct symbol) * rule->s_r[3].nb_sym);
  create_sym(rule->s_r[3].sym_arr + 0, 1, 0, 0, EOF_SYM);
  
  return rule;
}

struct rule* init_rule_list(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, AND_OR, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, LIST_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, OPTIONAL, COM_AMP, 0);

  return rule;
}

struct rule* init_rule_list_aux(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, COM_AMP, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, AND_OR, 0);

  return rule;
}

struct rule* init_rule_com_amp(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
<<<<<<< HEAD
  rule->nb_s_r = 3;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, 0, 0, SEMI);

  // Rule 2
  rule->s_r[1].nb_sym = 1
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 1, 0, 0, AND);

  // Rule 3
  rule->s_r[2].nb_sym = 1;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[2].sym_arr, 1, 0, 0, NEW_LINE);

  return rule;
}

struct rule *init_rule_and_or(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, PIPELINE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, AND_OR_AUX, 0);

  return rule;
}

struct rule *init_rule_and_or_aux(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, BOOL_OP, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, MANDATORY, PIPELINE, 0);

  return rule;
}

struct rule *init_rule_bool_op(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * 2);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, AND_IF);

  // Rule 2
  rule->s_r[1].nb_sym = 1;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 1, MANDATORY, 0, OR_IF);

  return rule;
}

struct rule *init_rule_new_line(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, NEWLINE);

  return rule;
}

struct rule *init_rule_pipeline(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, IF);

  return rule;

  /*struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONNAL, BANG_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMMAND, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, PIPELINE_AUX, 0);

  return rule;*/
}


/*struct rule *init_rule_pipeline(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONNAL, BANG_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMMAND, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, PIPELINE_AUX, 0);

  return rule;
}

struct rule *init_rule_bang(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, BANG);

  return rule;
}

struct rule *init_rule_pipeline_aux(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, PIPE);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, MANDATORY, COMMAND, 0);

  return rule;
}

struct rule *init_rule_command(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 3
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 0, MANDATORY, SIMPLE_COMMAND, 0);

  // Rule 2
  rule->s_r[1].nb_sym = 2;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[1].sym_arr + 0, 0, MANDATORY, SHELL_COMMAND, 0);
  create_sym(rule->s_r[1].sym_arr + 1, 0, STAR, REDIRECTION, 0);

  // Rule 3
  rule->s_r[2].nb_sym = 2
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[2].sym_arr + 0, 0, MANDATORY, FUNCDEC, 0);
  create_sym(rule->s_r[2].sym_arr + 1, 0, STAR, REDIRECTION, 0);

  return rule;
}

struct rule *init_rule_simple_command(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * 2);
  // Rule 1
  rule->s_r[0].nb_sym = 1
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 0, PLUS, PREFIX, 0);

  // Rule 2
  rule->s_r[1].nb_sym = 2;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[1].sym_arr + 0, 0, STAR, PREFIX, 0);
  create_sym(rule->s_r[1].sym_arr + 1, 0, PLUS, ELEMENT, 0);

  return rule;
}

struct rule *init_rule_shell_command(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 7;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, L_BRACE);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, R_BRACE);

  // Rule 2
  rule->s_r[1].nb_sym = 3;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[1].sym_arr + 0, 1, MANDATORY, 0, L_PAR);
  create_sym(rule->s_r[1].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[1].sym_arr + 2, 1, MANDATORY, 0, R_PAR);

  // Rule 3
  rule->s_r[2].nb_sym = 1;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[2].sym_arr, 0, MANDATORY, RULE_FOR, 0);

  // Rule 4
  rule->s_r[3].nb_sym = 1;
  rule->s_r[3].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[3].sym_arr, 0, MANDATORY, RULE_WHILE, 0);

  // Rule 5
  rule->s_r[4].nb_sym = 1;
  rule->s_r[4].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[4].sym_arr, 0, MANDATORY, RULE_UNTIL, 0);

  // Rule 6
  rule->s_r[5].nb_sym = 1;
  rule->s_r[5].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[5].sym_arr, 0, MANDATORY, RULE_CASE, 0);

  // Rule 7
  rule->s_r[6].nb_sym = 1;
  rule->s_r[6].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[6].sym_arr, 0, MANDATORY, RULE_IF, 0);

  return rule;
}

struct rule *init_rule_function(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);

  // Rule 1
  rule->s_r[0].nb_sym = 6;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 6);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONNAL, FUNCTION, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, WORD, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, L_PAR);
  create_sym(rule->s_r[0].sym_arr + 3, 1, MANDATORY, 0, R_PAR);
  create_sym(rule->s_r[0].sym_arr + 4, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 5, 0, MANDATORY, SHELL_COMMAND, 0);

  return rule;
}

struct rule *init_rule_redirection(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 9;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, GREAT);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 2
  rule->s_r[1].nb_sym = 3;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[1].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[1].sym_arr + 1, 1, MANDATORY, 0, LESS);
  create_sym(rule->s_r[1].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 3
  rule->s_r[2].nb_sym = 3;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[2].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[2].sym_arr + 1, 1, MANDATORY, 0, DGREAT);
  create_sym(rule->s_r[2].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 4
  rule->s_r[3].nb_sym = 3;
  rule->s_r[3].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[3].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[3].sym_arr + 1, 1, MANDATORY, 0, DLESS);
  create_sym(rule->s_r[3].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 5
  rule->s_r[4].nb_sym = 3;
  rule->s_r[4].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[4].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[4].sym_arr + 1, 1, MANDATORY, 0, DLESSDASH);
  create_sym(rule->s_r[4].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 6
  rule->s_r[5].nb_sym = 3;
  rule->s_r[5].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[5].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[5].sym_arr + 1, 1, MANDATORY, 0, GREATAND);
  create_sym(rule->s_r[5].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 7
  rule->s_r[6].nb_sym = 3;
  rule->s_r[6].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[6].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[6].sym_arr + 1, 1, MANDATORY, 0, LESSAND);
  create_sym(rule->s_r[6].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 8
  rule->s_r[7].nb_sym = 3;
  rule->s_r[7].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[7].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[7].sym_arr + 1, 1, MANDATORY, 0, CLOBBER);
  create_sym(rule->s_r[7].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 9
  rule->s_r[8].nb_sym = 3;
  rule->s_r[8].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[8].sym_arr + 0, 0, OPTIONNAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[8].sym_arr + 1, 1, MANDATORY, 0, LESSGREAT);
  create_sym(rule->s_r[8].sym_arr + 2, 1, MANDATORY, 0, WORD);

  return rule;
}

struct rule *init_rule_ionumber(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, IO_NUMBER);

  return rule;
}

struct rule *init_rule_prefix(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, ASSIGNEMENT_WORD);

  // Rule 2
  rule->s_r[1].nb_sym = 1;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 0, MANDATORY, REDIRECTION, 0);

  return rule;
}

struct rule *init_rule_element(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, WORD);

  // Rule 2
  rule->s_r[1].nb_sym = 1;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 0, MANDATORY, REDIRECTION, 0);

  return rule;
}

struct rule *init_rule_compound_list(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 4;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 4);
  create_sym(rule->s_r[0].sym_arr + 0, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, AND_OR, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, COMPOUND_LIST_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 0, OPTIONNAL, COMPOUND_AUX_2, 0);

  return rule;
}

struct rule *init_rule_compoud_list_aux(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, COM_AMP, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, MANDATORY, AND_OR, 0);

  return rule;
}

struct rule *init_rule_compoud_aux_2(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, COM_AMP, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);

  return rule;
}

struct rule *init_rule_for(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 5;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 5);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, FOR);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, WORD);
  create_sym(rule->s_r[0].sym_arr + 2, 0, OPTIONNAL, FOR_RULE_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 4, 0, MANDATORY, DO_GROUP, 0);

  return rule;
}

struct rule *init_rule_for_aux(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 1);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, SEMI);

  // Rule 1
  rule->s_r[0].nb_sym = 4;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 4);
  create_sym(rule->s_r[0].sym_arr + 0, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, IN);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, WORD_RULE);
  create_sym(rule->s_r[0].sym_arr + 3, 0, MANDATORY, FOR_RULE_AUX_2, 0);

  return rule;
}

struct rule *init_rule_word(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 1);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, WORD);

  return rule;
}

struct rule *init_rule_for_aux_2(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, 0, 0, SEMI);

  // Rule 2
  rule->s_r[1].nb_sym = 1
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 1, 0, 0, NEW_LINE);

  return rule;
}

struct rule *init_rule_while(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, WHILE);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, MANDATORY, DO_GROUP, 0);

  return rule;
}

struct rule *init_rule_until(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, UNTIL);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, MANDATORY, DO_GROUP, 0);

  return rule;
}

struct rule *init_rule_case(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 7;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 7);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, CASE);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, WORD);
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 1, MANDATORY, 0, IN);
  create_sym(rule->s_r[0].sym_arr + 4, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 5, 0, OPTIONNAL, CASE_CLAUSE, 0);
  create_sym(rule->s_r[0].sym_arr + 6, 1, MANDATORY, 0, ESAC);

  return rule;
}

struct rule *init_rule_if(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 6;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 6);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, IF);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, THEN);
  create_sym(rule->s_r[0].sym_arr + 3, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 4, 0, OPTIONNAL, ELSE_CLAUSE, 0);
  create_sym(rule->s_r[0].sym_arr + 5, 1, MANDATORY, 0, FI);

  return rule;
}

struct rule *init_rule_else_clause(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, ELSE);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);

  // Rule 2
  rule->s_r[0].nb_sym = 5;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 5);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, ELIF);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, THEN);
  create_sym(rule->s_r[0].sym_arr + 3, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 4, 0, OPTIONNAL, ELSE_CLAUSE, 0);

  return rule;
}

struct rule *init_rule_do_group(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, DO);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, DONE);

  return rule;
}

struct rule *init_rule_case_clause(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 4;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 4);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, CASE_ITEM, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, CASE_CLAUSE_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, OPTIONNAL, DSEMI_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 0, STAR, NEW_LINE_RULE, 0);

  return rule;
}

struct rule *init_rule_case_clause_aux(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, DSEMI);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, MANDATORY, CASE_ITEM, 0);

  return rule;
}

struct rule *init_rule_dsemi(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, DSEMI);

  return rule;
}

struct rule *init_rule_case_item(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 6;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 6);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONNAL, L_PAR_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, WORD);
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, CASE_ITEM_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 1, MANDATORY, 0, R_PAR);
  create_sym(rule->s_r[0].sym_arr + 4, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 5, 0, OPTIONNAL, COMPOUND_LIST, 0);

  return rule;
}

struct rule *init_rule_l_par(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, L_PAR);

  return rule;
}

struct rule *init_rule_case_item_aux(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, PIPE);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, WORD);

  return rule;
}*/
