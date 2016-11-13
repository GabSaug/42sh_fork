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
