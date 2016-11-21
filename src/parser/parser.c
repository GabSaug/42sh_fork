#include "my_malloc.h"
#include "lexer.h"
#include "parser.h"
#include "vector.h"
#include "tree.h"
#include "rules.h"

static struct tree* parse_rec(struct rule** rules, struct vector* v_token,
                              enum non_terminal_symbol symbol,
                              size_t* nb_token_read);

static struct tree* parse_rec_chap(struct rule** rules, struct vector* v_token,
                                   size_t* nb_token_read)
{
  *nb_token_read = 0;
  return parse_rec(rules, v_token, INPUT, nb_token_read);
}


struct tree* parse(struct rule** rules, struct vector* v_token, int* fit_level)
{
  size_t nb_token_read;
  *fit_level = 0;
  struct tree* tree = parse_rec_chap(rules, v_token, &nb_token_read);
  if (nb_token_read == v_size(v_token))
    return tree;
  else
  {
    tree_destroy(tree);
    if (nb_token_read == v_size(v_token))
      *fit_level = 1;
    return NULL;
  }
}

static int parse_mandatory(struct tree* tree, struct rule** rules,
                           struct vector* v_token, size_t* nb_token_read,
                           struct symbol* sym)
{
  struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
  if (child == NULL)
    return 0;
  tree_add_non_terminal_child(tree, child);
  return 1;
}

static int parse_star(struct tree* tree, struct rule** rules,
                      struct vector* v_token, size_t* nb_token_read,
                      struct symbol* sym)
{
  while (1)
  {
    struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
    if (child == NULL)
      break;
    tree_add_non_terminal_child(tree, child);
  }
  return 1;
}

static int parse_optional(struct tree* tree, struct rule** rules,
                          struct vector* v_token, size_t* nb_token_read,
                          struct symbol* sym)
{
  struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
  if (child != NULL)
    tree_add_non_terminal_child(tree, child);
  return 1;
}

static int parse_plus(struct tree* tree, struct rule** rules,
                      struct vector* v_token, size_t* nb_token_read,
                      struct symbol* sym)
{
  struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
  if (child == NULL)
    return 0;
  tree_add_non_terminal_child(tree, child);
  while (1)
  {
    struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
    if (child == NULL)
      break;
    tree_add_non_terminal_child(tree, child);
  }
  return 1;
}

static int parse_nonterminal(struct tree* tree, struct rule** rules,
                             struct vector* v_token, size_t* nb_token_read,
                             struct symbol* sym)
{
  if (sym->repeat == MANDATORY)
    return parse_mandatory(tree, rules, v_token, nb_token_read, sym);
  else if (sym->repeat == STAR)
    return parse_star(tree, rules, v_token, nb_token_read, sym);
  else if (sym->repeat == OPTIONAL)
    return parse_optional(tree, rules, v_token, nb_token_read, sym);
  else if (sym->repeat == PLUS)
    return parse_plus(tree, rules, v_token, nb_token_read, sym);
  else
    return 0;
}

static int parse_terminal(struct tree* tree, struct rule** rules,
                          struct vector* v_token, size_t* nb_token_read,
                          struct symbol* sym)
{
  rules = rules;
  struct token* token = v_get(v_token, *nb_token_read);
  if (token && sym->terminal_symbol == token->id)
  {
    *nb_token_read += 1;
    tree_add_terminal_child(tree, token);
    return 1;
  }
  return 0;
}

// return -1 if no rules fit
// return the length of the string it has accepted otherwise
static struct tree* parse_rec(struct rule** rules, struct vector* v_token,
                              enum non_terminal_symbol symbol,
                              size_t* nb_token_read)
{
  struct tree* tree = tree_create(symbol);
  int nb_init_token_read = *nb_token_read;
  for (size_t i = 0; i < rules[symbol]->nb_s_r; ++i)
  {
    struct simple_rule* s_r = &(rules[symbol]->s_r[i]);
    tree_delete_all_child(tree);
    *nb_token_read = nb_init_token_read;
    size_t j;
    for (j = 0; j < s_r->nb_sym; ++j)
    {
      struct symbol* sym = &(s_r->sym_arr[j]);
      if (sym->terminal) // terminal
        if (!parse_terminal(tree, rules, v_token, nb_token_read, sym))
          break;
        else
          continue;
      // non terminal
      else if (!parse_nonterminal(tree, rules, v_token, nb_token_read, sym))
        break;
    }
    if (j == s_r->nb_sym) // Rule match
      return tree;
  }
  tree_destroy(tree); // No rules fit
  *nb_token_read = nb_init_token_read;
  return NULL;
}
