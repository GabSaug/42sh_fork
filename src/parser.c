#include "parser.h"

enum symbol* 

struct simple_rule
{
  enum symbol* symbol;
}

struct rule
{
  struct simple_rule* simple_rule
}

struct rule rules[] =
{
  rule_input,
  rule_list,
  rule_and_or,
  rule_pipeline,
  ...
};

// return -1 if no rules fit
// return the length of the string it has accepted otherwise
int parse(struct vector* v_token, enum symbol symbol)
{
  create_tree(symbol);
  initial_string = string; initial_length = length;
  for rule in rules_list[symbol]
  {
    delete_all_child(tree);
    string = inital_string; length = inital_length;
    for token in rule
    {
      if (token.operator == atomic)
      {
        if (token.sym == non_terminal)
        {
          int l;
          ret = parse(token.sym, string, length, &l);
          if (ret == NULL) break; // exit and check next rule
          string += l; length -= l;
          add_child(tree, ret);
        }
        else
        {
          if (strncmp(string, token.string) == 0)
            string += token.string.length;
          else
          {
            ret = -1;
            break;
          }
        }
      }
      else if (token.operator == star)
      {
        do {
          ret = parse(token.sym, string, length);
        } while (ret > 0)
        string += ret; length -= ret;
      }
      else if (token.operator == braket)
      {
        for sym in token.list
        {
          ret = parse(sym, string, length);
          if (ret > 0)
          {
            string += ret; length -= ret;
            break;
          }
        }
      }
    }
    if (ret != -1)
    {
      // All symbol passed
      return inital_length - length;
    }
  }
  // No rules fit
  return -1;
}


