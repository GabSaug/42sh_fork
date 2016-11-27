#include "rules.h"

static struct rule *init_rule_else_clause(void)
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
  rule->s_r[1].nb_sym = 5;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * 5);
  create_sym(rule->s_r[1].sym_arr + 0, 1, MANDATORY, 0, ELIF);
  create_sym(rule->s_r[1].sym_arr + 1, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[1].sym_arr + 2, 1, MANDATORY, 0, THEN);
  create_sym(rule->s_r[1].sym_arr + 3, 0, MANDATORY, COMPOUND_LIST, 0);
  create_sym(rule->s_r[1].sym_arr + 4, 0, OPTIONAL, ELSE_CLAUSE, 0);

  return rule;
}

static struct rule *init_rule_do_group(void)
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

static struct rule *init_rule_case_clause(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 4;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 4);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, CASE_ITEM, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, CASE_CLAUSE_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, OPTIONAL, DSEMI_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 0, STAR, NEW_LINE_RULE, 0);

  return rule;
}

static struct rule *init_rule_case_clause_aux(void)
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

static struct rule *init_rule_dsemi(void)
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

static struct rule *init_rule_case_item(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 6;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 6);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONAL, L_PAR_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, WORD);
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, CASE_ITEM_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 1, MANDATORY, 0, R_PAR);
  create_sym(rule->s_r[0].sym_arr + 4, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 5, 0, OPTIONAL, COMPOUND_LIST, 0);

  return rule;
}

static struct rule *init_rule_l_par(void)
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

static struct rule *init_rule_case_item_aux(void)
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

struct rule **init_all_rules6(struct rule **rules)
{
  rules[ELSE_CLAUSE] = init_rule_else_clause();
  rules[DO_GROUP] = init_rule_do_group();
  rules[CASE_CLAUSE] = init_rule_case_clause();
  rules[CASE_CLAUSE_AUX] = init_rule_case_clause_aux();
  rules[DSEMI_RULE] = init_rule_dsemi();
  rules[CASE_ITEM] = init_rule_case_item();
  rules[L_PAR_RULE] = init_rule_l_par();
  rules[CASE_ITEM_AUX] = init_rule_case_item_aux();
  rules[NEW_LINE_RULE] = init_rule_new_line();

  return rules;
}
