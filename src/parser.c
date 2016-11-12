#include "my_malloc.h"
#include "lexer.h"
#include "parser.h"
#include "vector.h"

struct rule* init_rule_input(void);

struct rule** init_all_rule(void)
{
  struct rule** rules = my_malloc(sizeof(struct rule*) * NB_RULE);
  rules[INPUT] = init_rule_input();
  rules[LIST] = init_rule_list();
  rules[AND_OR] = init_rule_and_or();
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
  create_sym(rule->s_r[0].sym_arr + 1, 1, 0, 0, NEW_LINE);

  // Rule 2
  rule->s_r[1].nb_sym = 2;
  rule->s_r[1].sym_arr = my_malloc(sizeof (struct symbol) * rule->s_r[1].nb_sym);
  create_sym(rule->s_r[1].sym_arr + 0, 0, MANDATORY, LIST, 0);
  create_sym(rule->s_r[1].sym_arr + 1, 1, 0, 0, EOF);

  // Rule 3
  rule->s_r[2].nb_sym = 1;
  rule->s_r[2].sym_arr = my_malloc(sizeof (struct symbol) * rule->s_r[2].nb_sym);
  create_sym(rule->s_r[2].sym_arr + 0, 1, 0, 0, NEW_LINE);
  
  // Rule 4
  rule->s_r[3].nb_sym = 1;
  rule->s_r[3].sym_arr = my_malloc(sizeof (struct symbol*) * rule->s_r[3].nb_sym);
  create_sym(rule->s_r[2].sym_arr + 0, 1, 0, 0, EOF);
  
  return rule;
}

/*struct rule* init_rule_list(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->s_r = my_malloc(sizeof (struct simple_rule*) * 2);
  // Rule 1
  rule->s_r[0]->sym_arr = my_malloc(sizeof (struct symbol*) * 4);
  rule->s_r[0]->sym_arr[0] = create_sym(0, MANDATORY, AND_OR, 0);
  rule->s_r[0]->sym_arr[1] = create_sym(0, STAR, LIST_AUX, 0);
  rule->s_r[0]->sym_arr[2] = create_sym(0, OPTIONAL, COM_AMP, 0);
  rule->s_r[0]->sym_arr[3] = NULL;

   rule->s_r[1] = NULL;
  return rule;
}

struct rule* init_rule_list_aux(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->s_r = my_malloc(sizeof (struct simple_rule*) * 2);
  // Rule 1
  rule->s_r[0]->sym_arr = my_malloc(sizeof (struct symbol*) * 3);
  rule->s_r[0]->sym_arr[0] = create_sym(0, MANDATORY, COM_AMP, 0);
  rule->s_r[0]->sym_arr[1] = create_sym(0, MANDATORY, AND_OR, 0);
  rule->s_r[0]->sym_arr[2] = NULL;

  rule->s_r[1] = NULL;
  return rule;
}

struct rule* init_rule_com_amp(void)
{
  struct rule* rule = my_malloc(sizeof (struct rule));
  rule->s_r = my_malloc(sizeof (struct simple_rule*) * 3);
  // Rule 1
  rule->s_r[0]->sym_arr = my_malloc(sizeof (struct symbol*) * 2);
  rule->s_r[0]->sym_arr[0] = create_sym(1, 0, 0, SEMI);
  rule->s_r[0]->sym_arr[1] = NULL;

  // Rule 2
  rule->s_r[1]->sym_arr = my_malloc(sizeof (struct symbol*) * 2);
  rule->s_r[1]->sym_arr[0] = create_sym(1, 0, 0, AND);
  rule->s_r[1]->sym_arr[1] = NULL;

  rule->s_r[2] = NULL;
  return rule;
}*/


// return -1 if no rules fit
// return the length of the string it has accepted otherwise
struct tree* parse(struct vector* v_token, enum symbol symbol, int* nb_token_read)
{
  struct tree* tree = tree_create(symbol);
  int nb_init_token_read = *nb_token_read;
//  initial_string = string; initial_length = length;i
  for (size_t i = 0; rules[symbol]->s_r[i]; ++i)
  {
    struct simple_rule* s_r = &(rules[symbol]->s_r[i]);

    delete_all_child(tree);
    *nb_token_read = nb_init_token_read; 
    //string = inital_string; length = inital_length;
    size_t j;
    for (j = 0; s_r->sym_arr[j]; ++j)
    {
      struct symbol* sym = &(s_r->sym_arr[j]);
      if (sym->terminal)
      {
        if (sym->terminal_symbol == v_get(v_token, nb_token_read))
        {
          *nb_token_read += 1;
          tree_add_child(tree, sym_terminal_symbol);
          continue;
        }
        break;
      }
      else // nonterminal
      {
        if (sym->repeat == MANDATORY)
        {
          struct tree* child = parse(v_token, sym->rule, nb_token_read);
          if (child == NULL)
          {
            tree_destroy(tree);
            *nb_token_read = nb_init_token_read;
            return NULL;
          }
          *nb_token_read += 1;
          tree_add_child(tree, child);
        }
        else if (sym->repeat == STAR)
        {
          while (1)
          {
            struct tree* child = parse(v_token, sym->rule, nb_token_read);
            if (child == NULL)
              break;
            *nb_token_read += 1;
            tree_add_child(tree, child);
          }
        }
        else if (sym->repeat == OPTIONAL)
        {
          struct tree* child = parse(v_token, sym->rule, nb_token_read);
          if (child != NULL)
          {
            *nb_token_read += 1;
            tree_add_child(tree, child);
          }
        }
        else // PLUS
        {
          struct tree* child = parse(v_token, sym->rule, nb_token_read);
          if (child == NULL)
          {
            tree_destroy(tree);
            *nb_token_read = nb_init_token_read;
            return NULL;
          }
          *nb_token_read += 1;
          tree_add_child(tree, child);
          while (1)
          {
            struct tree* child = parse(v_token, sym->rule, nb_token_read);
            if (child == NULL)
              break;
            *nb_token_read += 1;
            tree_add_child(tree, child);
          }
        }
      }
    }
    if (s_r->sym_arr[j] == NULL) // Rule match
    {
      // All symbol passed
      return tree;
    }
  }
  // No rules fit
  tree_destroy(tree);
  *nb_token_read = nb_init_token_read;
  return NULL;
}


/*struct tree* rule_input(char* s, size_t len, size_t* b_read)
{
  tree_create(INPUT);
  struct tree* child = rule_list(s, len);
  if (child == NULL)
    return 
}

struct tree* rule_list(char* s, size_t len, size_t* b_read)
{
  struct tree* tree = tree_create(LIST);
  struct tree* child_1 = rule_and_or(s, len, b_read);
  if (child_1 == NULL)
    return NULL;
  tree_add_child(tree, child_1);
  
  {
    int b_read_2 = 0;
    struct tree* child = rule_comma_esp(s + b_read, len, &b_read_2);
    if (child == NULL)
      break;
    struct tree* child = rule_and_or(s + b_read, len, &b_read_2);
    if (child == NULL)
      break;

  } while (1)


}*/


