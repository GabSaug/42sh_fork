#include "rules.h"

static struct rule *init_rule_for(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 5;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 5);
  create_sym(rule->s_r[0].sym_arr + 0, 1, MANDATORY, 0, FOR);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, WORD);
  create_sym(rule->s_r[0].sym_arr + 2, 0, OPTIONAL, FOR_RULE_AUX, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 4, 0, MANDATORY, DO_GROUP, 0);

  return rule;
}

static struct rule *init_rule_for_aux(void)
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
  create_sym(rule->s_r[0].sym_arr + 2, 0, STAR, WORD_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 3, 0, MANDATORY, FOR_RULE_AUX_2, 0);

  return rule;
}

static struct rule *init_rule_word(void)
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

static struct rule *init_rule_for_aux_2(void)
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

static struct rule *init_rule_while(void)
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

static struct rule *init_rule_until(void)
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

static struct rule *init_rule_case(void)
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
  create_sym(rule->s_r[0].sym_arr + 5, 0, OPTIONAL, CASE_CLAUSE, 0);
  create_sym(rule->s_r[0].sym_arr + 6, 1, MANDATORY, 0, ESAC);

  return rule;
}

static struct rule *init_rule_if(void)
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
  create_sym(rule->s_r[0].sym_arr + 4, 0, OPTIONAL, ELSE_CLAUSE, 0);
  create_sym(rule->s_r[0].sym_arr + 5, 1, MANDATORY, 0, FI);

  return rule;
}

struct rules **init_all_rules5(struct rule **rules)
{
  rules[RULE_FOR] = init_rule_for();
  rules[FOR_RULE_AUX] = init_rules_for_aux();
  rules[WORD_RULE] = init_rule_word();
  rules[FOR_RULE_AUX_2] = init_rule_for_aux_2();
  rules[RULE_WHILE] = init_rule_while();
  rules[RULE_UNTIL] = init_rule_until();
  rules[RULE_CASE] = init_rule_case();
  rules[RULE_IF] = init_rule_if();

  return rules;
}
