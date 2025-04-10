#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"
#include "vector.h"
#include "tree.h"
#include "my_string.h"

static char nts2string[][20] =
{
  "SCRIPT",
  "INPUT",
  "LIST",
  "COM_AMP",
  "COM_AMP_NL",
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
  "<<-", ">|", "{", "}", "(", ")", "!", "EOF",
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "in", "function", "<new_line>",
  "word", "assignment_word", "name", "IO_number"
};

static unsigned num_dot;

static void tree_print_dot_rec_run(struct tree* tree, FILE* file)
{
  unsigned n = num_dot;
  if (tree->token)
  {
    if (tree->token->id == WORD)
    {
      char* escaped = escape_quote(tree->token->s);
      fprintf(file, "%u [label=\"%s\"; shape=note]\n", num_dot, escaped);
      free(escaped);
    }
    else if (tree->token->id == IO_NUMBER)
      fprintf(file, "%u [label=\"%s = %s\"]\n", num_dot,
              ts2string[tree->token->id], tree->token->s);
    else
      fprintf(file, "%u [label=\"%s\"]\n", num_dot, ts2string[tree->token->id]);
  }
  else
  {
    fprintf(file, "%u [label=\"%s\" ; shape=box]\n", n, nts2string[tree->nts]);
    for (size_t i = 0; i < tree->child->size; ++i)
    {
      fprintf(file, "%u -> %u\n", n, ++num_dot);
      tree_print_dot_rec_run(v_get(tree->child, i), file);
    }
    num_dot++;
  }
}

static void tree_print_dot_rec(struct tree* tree, FILE* file)
{
  if (tree == NULL)
    return;
  tree_print_dot_rec_run(tree, file);
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
  for (size_t i = 0; i < v_size(tree->child); ++i)
    tree_destroy(v_get(tree->child, i));
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

// What if 2 functions are nested in each other ?
void tree_destroy_ast_extract_fun(struct tree* tree, struct vector* v_fun)
{
  if (!tree)
    return;
  for (size_t i = 0; i < v_size(tree->child); ++i)
  {
    struct tree* child = v_get(tree->child, i);
    if (child->nts == FUNCDEC)
      v_append(v_fun, child);
    else
      tree_destroy_ast_extract_fun(child, v_fun);
  }
  v_destroy(tree->child, NULL);
  free(tree);
}

void tree_destroy_fun(void* p)
{
  struct tree* tree = p;
  if (!tree)
    return;
  for (size_t i = 0; i < v_size(tree->child); ++i)
  {
    struct tree* child = v_get(tree->child, i);
    if (child->nts != FUNCDEC)
      tree_destroy_fun(child);
  }
  v_destroy(tree->child, NULL);
  free(tree);
}
