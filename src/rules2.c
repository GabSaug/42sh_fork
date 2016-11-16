#include "rules.h"

static struct rule* init_rule_input(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 4;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, LIST, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 1, 0, 0, EOF_SYM);

  // Rule 2
  rule->s_r[1].nb_sym = 1;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr + 0, 0, MANDATORY, LIST, 0);

  // Rule 3
  rule->s_r[2].nb_sym = 1;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[2].sym_arr + 0, 1, 0, 0, NL);
  
  // Rule 4
  rule->s_r[3].nb_sym = 1;
  rule->s_r[3].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[3].sym_arr + 0, 1, 0, 0, EOF_SYM);
  
  return rule;
}

static struct rule* init_rule_list(void)
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

static struct rule* init_rule_list_aux(void)
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

static struct rule* init_rule_com_amp(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 3;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, 0, 0, SEMI);

  // Rule 2
  rule->s_r[1].nb_sym = 1;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 1, 0, 0, AND);

  // Rule 3
  rule->s_r[2].nb_sym = 1;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[2].sym_arr, 1, 0, 0, NL);

  return rule;
}

static struct rule *init_rule_and_or(void)
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

static struct rule *init_rule_and_or_aux(void)
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

static struct rule *init_rule_bool_op(void)
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

static struct rule *init_rule_new_line(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule));
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, NL);

  return rule;
}

struct rule **init_all_rules2(struct rule **rules)
{
  rules[INPUT] = init_rule_input();
  rules[LIST] = init_rule_list();
  rules[LIST_AUX] = init_rule_list_aux();
  rules[COM_AMP] = init_rule_com_amp();
  rules[AND_OR] = init_rule_and_or();
  rules[AND_OR_AUX] = init_rule_and_or_aux();
  rules[BOOL_OP] = init_rule_bool_op();
  rules[NEW_LINE_RULE] = init_rule_new_line();

  return rules;
}
