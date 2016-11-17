#include <stdlib.h>

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
  rules = init_all_rules2(rules);
  rules = init_all_rules3(rules);
  rules = init_all_rules4(rules);
  rules = init_all_rules5(rules);
  rules = init_all_rules6(rules);

  return rules;
}

void rule_destroy(struct rule** rules)
{
  for (size_t i = 0; i < NB_RULE; ++i)
  {
    for (size_t j = 0; j < rules[i]->nb_s_r; ++j)
    {
      free(rules[i]->s_r[j].sym_arr);
    }
    free(rules[i]->s_r);
    free(rules[i]);
  }
  free(rules);
}
