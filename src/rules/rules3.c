#include "rules.h"

static struct rule *init_rule_pipeline(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONAL, BANG_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, COMMAND, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, PIPELINE_AUX, 0);

  return rule;
}

static struct rule *init_rule_bang(void)
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

static struct rule *init_rule_pipeline_aux(void)
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

static struct rule *init_rule_command(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 3;
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
  rule->s_r[2].nb_sym = 2;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[2].sym_arr + 0, 0, MANDATORY, FUNCDEC, 0);
  create_sym(rule->s_r[2].sym_arr + 1, 0, STAR, REDIRECTION, 0);

  return rule;
}

static struct rule *init_rule_simple_command(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * 2);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 0, PLUS, PREFIX, 0);

  // Rule 2
  rule->s_r[1].nb_sym = 2;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[1].sym_arr + 0, 0, STAR, PREFIX, 0);
  create_sym(rule->s_r[1].sym_arr + 1, 0, PLUS, ELEMENT, 0);

  return rule;
}

static struct rule *init_rule_shell_command2(struct rule *rule)
{
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

static struct rule *init_rule_shell_command(void)
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

  rule = init_rule_shell_command2(rule);
  return rule;
}

static struct rule *init_rule_function(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);

  // Rule 1
  rule->s_r[0].nb_sym = 6;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 6);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONAL, FUNCTION, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, MANDATORY, WORD, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, L_PAR);
  create_sym(rule->s_r[0].sym_arr + 3, 1, MANDATORY, 0, R_PAR);
  create_sym(rule->s_r[0].sym_arr + 4, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 5, 0, MANDATORY, SHELL_COMMAND, 0);

  return rule;
}

struct rule **init_all_rules3(struct rule **rules)
{
  rules[PIPELINE] = init_rule_pipeline();
  rules[BANG_RULE] = init_rule_bang();
  rules[PIPELINE_AUX] = init_rule_pipeline_aux();
  rules[COMMAND] = init_rule_command();
  rules[SIMPLE_COMMAND] = init_rule_simple_command();
  rules[SHELL_COMMAND] = init_rule_shell_command();
  rules[FUNCDEC] = init_rule_function();

  return rules;
}
