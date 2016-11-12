#include "my_malloc.h"
#include "lexer.h"
#include "parser.h"
#include "vector.h"


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


