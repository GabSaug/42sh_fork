#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"
#include "vector.h"
#include "tree.h"


static char nts2string[][20] =
{
  "INPUT",
  "LIST",
  "COM_AMP",
  "AND_OR",
  "BOOL_OP",
  "NEW_LINE_RULE",
  "PIPELINE",
  "BANG_RULE",
  "COMMAND",
  "SIMPLE_COMMAND",
  "SHELL_COMMAND",
  "FUNCDEC",
  "REDIRECTION",
  "IO_NUMBER_RULE",
  "PREFIX",
  "ELEMENT",
  "COMPOUND_LIST",
  "RULE_FOR",
  "WORD_RULE",
  "RULE_WHILE",
  "RULE_UNTIL",
  "RULE_CASE",
  "RULE_IF",
  "ELSE_CLAUSE",
  "DO_GROUP",
  "CASE_CLAUSE",
  "DSEMI_RULE",
  "CASE_ITEM",
  "L_PAR_RULE",
  "LIST_AUX",
  "AND_OR_AUX",
  "PIPELINE_AUX",
  "COMPOUND_LIST_AUX",
  "COMPOUND_LIST_AUX_2",
  "FOR_RULE_AUX",
  "FOR_RULE_AUX_2",
  "CASE_CLAUSE_AUX",
  "CASE_ITEM_AUX"
};

static char ts2string[][20] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&", "<>",
  "<<-", ">|", "EOF",
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "{", "}", "(", ")", "!", "in", "function", "<new_line>",
  "word", "assignment_word", "name", "IO_number"
};

static unsigned num_dot;

static void tree_print_dot_rec(struct tree* tree, FILE* file)
{
  if (tree == NULL)
    return;
  unsigned num_node = num_dot;
  if (tree->token)
  {
    if (tree->token->id != WORD)
      fprintf(file, "%u [label=\"%s\"]\n", num_dot, ts2string[tree->token->id]);
    else
      fprintf(file, "%u [label=\"%s = %s\"]\n", num_dot,
              ts2string[tree->token->id], tree->token->s);
  }
  else
  {
    fprintf(file, "%u [label=\"%s\" ; shape=box]\n", num_node,
            nts2string[tree->nts]);
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
  if (tree->token)
  {
    printf("leaf = %d\n", tree->token->id);
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
  tree->token = NULL;
  tree->child = v_create();
  return tree;
}

void tree_add_terminal_child(struct tree* tree, struct token* token)
{
  struct tree* leaf = tree_create(0);
  leaf->token = token;
  v_append(tree->child, leaf);
}

void tree_add_non_terminal_child(struct tree* tree, struct tree* child)
{
  if (child->nts >= LIST_AUX)
  {
    for (size_t j = 0; j < child->child->size; ++j)
      tree_add_non_terminal_child(tree, v_get(child->child, j));
    v_destroy(child->child, NULL);
    free(child);
  }
  else
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
  for (size_t i = 0; i < v_size(tree->child); ++i)
    tree_destroy(v_get(tree->child, i));
  v_destroy(tree->child, NULL);
  free(tree);
}
