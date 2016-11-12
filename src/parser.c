#include "my_malloc.h"
#include "lexer.h"
#include "parser.h"
#include "vector.h"
#include "tree.h"
#include "rules.h"

static struct tree* parse_rec(struct rule** rules, struct vector* v_token,
                   enum non_terminal_symbol symbol, size_t* nb_token_read);

struct tree* parse(struct rule** rules, struct vector* v_token)
{
  size_t nb_token_read = 0;
  struct tree* tree = parse_rec(rules, v_token, INPUT, &nb_token_read);
  if (nb_token_read == v_size(v_token))
    return tree;
  else
  {
    tree_destroy(tree);
    return NULL;
  }
}

// return -1 if no rules fit
// return the length of the string it has accepted otherwise
static struct tree* parse_rec(struct rule** rules, struct vector* v_token,
                   enum non_terminal_symbol symbol, size_t* nb_token_read)
{
  struct tree* tree = tree_create(symbol);
  int nb_init_token_read = *nb_token_read;
//  initial_string = string; initial_length = length;i
  for (size_t i = 0; i < rules[symbol]->nb_s_r; ++i)
  {
    struct simple_rule* s_r = &(rules[symbol]->s_r[i]);

    tree_delete_all_child(tree);
    *nb_token_read = nb_init_token_read; 
    //string = inital_string; length = inital_length;
    size_t j;
    for (j = 0; j < s_r->nb_sym; ++j)
    {
      struct symbol* sym = &(s_r->sym_arr[j]);
      if (sym->terminal)
      {
        struct token* token = v_get(v_token, *nb_token_read);
        if (token && sym->terminal_symbol == token->id)
        {
          *nb_token_read += 1;
          tree_add_terminal_child(tree, sym->terminal_symbol);
          continue;
        }
        break;
      }
      else // nonterminal
      {
        if (sym->repeat == MANDATORY)
        {
          struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
          if (child == NULL)
          {
            /*tree_destroy(tree);
            *nb_token_read = nb_init_token_read;
            return NULL;*/
            break;
          }
          //*nb_token_read += 1;
          tree_add_non_terminal_child(tree, child);
        }
        else if (sym->repeat == STAR)
        {
          while (1)
          {
            struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
            if (child == NULL)
              break;
            //*nb_token_read += 1;
            tree_add_non_terminal_child(tree, child);
          }
        }
        else if (sym->repeat == OPTIONAL)
        {
          struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
          if (child != NULL)
          {
            //*nb_token_read += 1;
            tree_add_non_terminal_child(tree, child);
          }
        }
        else // PLUS
        {
          struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
          if (child == NULL)
          {
            tree_destroy(tree);
            *nb_token_read = nb_init_token_read;
            return NULL;
          }
          //*nb_token_read += 1;
          tree_add_non_terminal_child(tree, child);
          while (1)
          {
            struct tree* child = parse_rec(rules, v_token, sym->rule, nb_token_read);
            if (child == NULL)
              break;
            //*nb_token_read += 1;
            tree_add_non_terminal_child(tree, child);
          }
        }
      }
    }
    if (j == s_r->nb_sym) // Rule match
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

