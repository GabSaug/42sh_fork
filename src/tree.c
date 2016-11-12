#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"
#include "vector.h"
#include "tree.h"

char nts2string[][20] =
{
  "INPUT", "LIST", "LIST_AUX", "SEMI_AMP_NEWLINE", "AND_OR", "AND_OR_AUX", "BOOL_OP",
  "NEW_LINE_RULE", "PIPELINE", "COMMAND", "SIMPLE_COMMAND", "SHELL_COMMAND",
  "FUNCDEC", "REDIRECTION", "PREFIX", "ELEMENT", "COMPOUND_LIST", "RULE_FOR",
  "RULE_WHILE", "RULE_UNTIL", "RULE_CASE", "RULE_IF", "ELSE_CLAUSE",
  "DO_GROUP", "CASE_CLAUSE", "CASE_ITEM"
};

static char ts2string[][20] =
{
  "<new_line>", ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&", "<>",
  "<<-", ">|", "EOF",
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "{", "}", "(", ")", "!", "in", "function", "word",
  "assignment_word", "name", "IO_number"
};

static unsigned num_dot;

static void tree_print_dot_rec(struct tree* tree, FILE* file)
{
  if (tree == NULL)
    return;
  unsigned num_node = num_dot;
  if (tree->ts != UNDIFINED)
  {
    fprintf(file, "%u [label=\"%s\"]\n", num_dot, ts2string[tree->ts]);
  }
  else
  {
    //num_dot++;
    fprintf(file, "%u [label=\"%s\" ; shape=box]\n", num_node, nts2string[tree->nts]);
    for (size_t i = 0; i < tree->child->size; ++i)
    {
      fprintf(file, "%u -> %u\n", num_node, ++num_dot);
      tree_print_dot_rec(v_get(tree->child, i), file);
    }
    num_dot++;
  }
}

void tree_print_dot(struct tree* tree)
{
  FILE* file = fopen("ast.dot", "w+");
  if (!file)
    return;
  fprintf(file, "digraph ast {\n");
  num_dot = 0;
  tree_print_dot_rec(tree, file);
  fprintf(file, "}\n");
  fclose(file);
}

static void tree_print_rec(struct tree* tree, int indent)
{
  if (tree == NULL)
    return;
  if (tree->ts != UNDIFINED)
  {
    printf("leaf = %d\n", tree->ts);
  }
  else
  {
    printf("node has %zu child, name = %d\n", tree->child->size, tree->nts);
    for (size_t i = 0; i < tree->child->size; ++i)
    {
      printf("%*sch %zu : ", 8*indent, "", i);
      tree_print_rec(v_get(tree->child, i), indent + 1);
    }
  }
}

void tree_print(struct tree* tree)
{
  tree_print_rec(tree, 0);
}

struct tree* tree_create(enum non_terminal_symbol nts)
{
  struct tree* tree = my_malloc(sizeof (struct tree));
  tree->nts = nts;
  tree->ts = UNDIFINED;
  tree->child = v_create();
  return tree;
}

void tree_add_terminal_child(struct tree* tree, enum terminal_symbol ts)
{
  struct tree* leaf = tree_create(0);
  leaf->ts = ts;
  /*enum terminal_symbol* terminal_symbol = my_malloc(sizeof (enum terminal_symbol));
  tree->ts = my_malloc(sizeof (enum terminal_symbol));
  tree->ts = ts;*/
  v_append(tree->child, leaf);
}

void tree_add_non_terminal_child(struct tree* tree, struct tree* child)
{
  v_append(tree->child, child);
}

void tree_delete_all_child(struct tree* tree)
{
  v_erase(tree->child);
}

void tree_destroy(struct tree* tree)
{
  if (!tree)
    return;
  v_destroy(tree->child);
  free(tree);
}
