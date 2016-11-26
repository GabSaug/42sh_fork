#include "rules.h"

static struct rule *init_rule_redirection2(struct rule *rule)
{
  // Rule 4
  rule->s_r[3].nb_sym = 3;
  rule->s_r[3].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[3].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[3].sym_arr + 1, 1, MANDATORY, 0, DLESS);
  create_sym(rule->s_r[3].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 5
  rule->s_r[4].nb_sym = 3;
  rule->s_r[4].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[4].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[4].sym_arr + 1, 1, MANDATORY, 0, DLESSDASH);
  create_sym(rule->s_r[4].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 6
  rule->s_r[5].nb_sym = 3;
  rule->s_r[5].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[5].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[5].sym_arr + 1, 1, MANDATORY, 0, GREATAND);
  create_sym(rule->s_r[5].sym_arr + 2, 1, MANDATORY, 0, WORD);

  return rule;
}

static struct rule *init_rule_redirection3(struct rule *rule)
{
  // Rule 7
  rule->s_r[6].nb_sym = 3;
  rule->s_r[6].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[6].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[6].sym_arr + 1, 1, MANDATORY, 0, LESSAND);
  create_sym(rule->s_r[6].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 8
  rule->s_r[7].nb_sym = 3;
  rule->s_r[7].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[7].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[7].sym_arr + 1, 1, MANDATORY, 0, CLOBBER);
  create_sym(rule->s_r[7].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 9
  rule->s_r[8].nb_sym = 3;
  rule->s_r[8].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[8].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[8].sym_arr + 1, 1, MANDATORY, 0, LESSGREAT);
  create_sym(rule->s_r[8].sym_arr + 2, 1, MANDATORY, 0, WORD);

  return rule;
}

static struct rule *init_rule_redirection(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 9;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 1, MANDATORY, 0, GREAT);
  create_sym(rule->s_r[0].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 2
  rule->s_r[1].nb_sym = 3;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[1].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[1].sym_arr + 1, 1, MANDATORY, 0, LESS);
  create_sym(rule->s_r[1].sym_arr + 2, 1, MANDATORY, 0, WORD);

  // Rule 3
  rule->s_r[2].nb_sym = 3;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[2].sym_arr + 0, 0, OPTIONAL, IO_NUMBER_RULE, 0);
  create_sym(rule->s_r[2].sym_arr + 1, 1, MANDATORY, 0, DGREAT);
  create_sym(rule->s_r[2].sym_arr + 2, 1, MANDATORY, 0, WORD);

  rule = init_rule_redirection2(rule);
  rule = init_rule_redirection3(rule);
  return rule;
}

static struct rule *init_rule_io_number(void)
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

static struct rule *init_rule_prefix(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 2;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 1;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[0].sym_arr, 1, MANDATORY, 0, ASSIGNMENT_WORD);

  // Rule 2
  rule->s_r[1].nb_sym = 1;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol));
  create_sym(rule->s_r[1].sym_arr, 0, MANDATORY, REDIRECTION, 0);

  return rule;
}

static struct rule *init_rule_element(void)
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

static struct rule *init_rule_compound_list(void)
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
  create_sym(rule->s_r[0].sym_arr + 3, 0, OPTIONAL, COMPOUND_LIST_AUX_2, 0);

  return rule;
}

static struct rule *init_rule_compound_list_aux(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 3;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 3);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, COM_AMP_NL, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);
  create_sym(rule->s_r[0].sym_arr + 2, 0, MANDATORY, AND_OR, 0);

  return rule;
}

static struct rule *init_rule_compound_list_aux_2(void)
{
  struct rule *rule = my_malloc(sizeof (struct rule));
  rule->nb_s_r = 1;
  rule->s_r = my_malloc(sizeof (struct simple_rule) * rule->nb_s_r);
  // Rule 1
  rule->s_r[0].nb_sym = 2;
  rule->s_r[0].sym_arr = my_malloc(sizeof (struct symbol) * 2);
  create_sym(rule->s_r[0].sym_arr + 0, 0, MANDATORY, COM_AMP_NL, 0);
  create_sym(rule->s_r[0].sym_arr + 1, 0, STAR, NEW_LINE_RULE, 0);

  return rule;
}

struct rule **init_all_rules4(struct rule **rules)
{
  rules[REDIRECTION] = init_rule_redirection();
  rules[IO_NUMBER_RULE] = init_rule_io_number();
  rules[PREFIX] = init_rule_prefix();
  rules[ELEMENT] = init_rule_element();
  rules[COMPOUND_LIST] = init_rule_compound_list();
  rules[COMPOUND_LIST_AUX] = init_rule_compound_list_aux();
  rules[COMPOUND_LIST_AUX_2] = init_rule_compound_list_aux_2();

  return rules;
}
