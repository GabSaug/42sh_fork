#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "expansion.h"
#include "vector.h"
#include "str.h"
#include "string.h"
#include "tokenize.h"
#include "arithmetic.h"
#include "my_malloc.h"
#include "option_parser.h"
#include "my_string.h"
#include "main.h"

static char* tilde_expansion(char* s, struct hash_table *ht[]);
static char* parameter_expansion(char* s, struct hash_table *ht[]);
static char* remove_quote(char* s);
static int cmd_expansion(struct expansion, struct hash_table* ht[],
                         struct str* output);

static int ari_type(char *input, struct str *output, size_t i_input,
                   size_t *start, struct expansion exp, struct hash_table *ht[])
{
  str_append(output, input + *start, i_input - *start, 0);
  char* ret = arithmetic_expansion(my_strndup(exp.content_start,
                                              exp.content_size), ht);
  if (!ret)
  {
    str_destroy(output, 1);
    free(input);
    return 0;
  }
  str_append(output, ret, -1, 1);
  *start = i_input + exp.size;
  return 1;
}

static int test_exp(char *input, struct str *output, size_t *i_input,
                   size_t *start, struct hash_table *ht[], char quoted[3],
                   int in_ari_exp)
{
  if (update_quote(input[*i_input], quoted) == -1)
    return 1;
  struct expansion exp = tokenize_expansion(input + *i_input, in_ari_exp);

  if (exp.type == NO_EXPANSION || exp.type == DQ)
    return 1;
  else if (exp.type == ARI)
  {
    if (!ari_type(input, output, *i_input, start, exp, ht))
      return 0;
  }
  else if (exp.type == NORMAL || exp.type == BRACKET)
  {
    str_append(output, input + *start, *i_input - *start, 0);
    str_append(output, parameter_expansion(my_strndup(exp.content_start,
                                               exp.content_size), ht), -1, 1);
    *start = *i_input + exp.size;
  }
  else if (exp.type == CMD)
  {
    cmd_expansion(exp, ht, output);
    *start = *i_input + exp.size;
  }
  *i_input += exp.size - 1;
  return 1;
}

struct vector* expand(char* input, int in_ari_exp, struct hash_table *ht[])
{
  input = tilde_expansion(input, ht);
  struct str* output = str_create();
  size_t start = 0;
  size_t i_input;
  char quoted[3] = { 0 };
  for (i_input = 0; input[i_input]; ++i_input)
  {
    if (!test_exp(input, output, &i_input, &start, ht, quoted, in_ari_exp))
      return NULL;
  }
  str_append(output, input + start, i_input - start, 0);
  free(input);
  struct vector* v = v_create();
  v_append(v, output->s);
  str_destroy(output, 0);
  v = field_split(v, ht);
  for (size_t i = 0; i < v_size(v); ++i)
    v_set(v, i, remove_quote(v_get(v, i)));
  return v;
}

static int cmd_exp2(struct shell_tools cmd_tools, int temp_out,
                     struct str* output, int temp_err)
{
  if (dup2(temp_out, 1) == -1 || dup2(temp_err, 2) == -1)
    err(1, "cmd_expansion");

  int ret = process_input(&cmd_tools);
  char* file = NULL;
  if (ret == 0)
  {
    struct stat stat_buf;
    if (stat("temp_out", &stat_buf) == -1)
    {
      warn("Impossible to read stat from temp_out");
      return 0;
    }
    size_t size_file = stat_buf.st_size;
    file = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, temp_out, 0);
    str_append(output, file, size_file, 0);
    munmap(file, size_file);
  }
  return 1;
}

static int cmd_expansion(struct expansion exp, struct hash_table* ht[],
                         struct str* output)
{
  struct shell_tools cmd_tools;
  for (size_t i = 0; i < NB_HT; ++i)
    cmd_tools.ht[i] = cpy_hash(ht[i]);
  cmd_tools.option.input =  my_strndup(exp.content_start, exp.content_size);
  cmd_tools.v_token = NULL;
  cmd_tools.ast = NULL;
  cmd_tools.sub_shell = 1;

  int std_in = dup(0);
  int std_out = dup(1);
  int std_err = dup(2);
  int temp_out = open("temp_out", O_CLOEXEC | O_RDWR | O_CREAT | O_TRUNC, 0666);
  int temp_err = open("temp_err", O_CLOEXEC | O_RDWR | O_CREAT | O_TRUNC, 0666);

  if (!cmd_exp2(cmd_tools, temp_out, output, temp_err))
    return 0;

  close(temp_out);
  close(temp_err);

  dup2(std_in, 0);
  close(std_in);
  dup2(std_out, 1);
  close(std_out);
  dup2(std_err, 2);
  close(std_err);
  for (size_t ii = 0; ii < NB_HT; ++ii)
    destroy_hash(cmd_tools.ht[ii]);
  return 1;
}

static char* remove_quote(char* s)
{
  char* new = NULL;
  size_t new_size = 1;
  new = realloc(new, new_size);
  size_t j = 0;
  char quoted[3] = {0};
  for (size_t i = 0; s[i]; ++i)
  {
    if (quoted[BACKSLASH] > 0)
      quoted[BACKSLASH]--;
    if (!update_quote(s[i], quoted))
    {
      if (new_size - 1 <= j)
      {
        new_size *= 2;
        new = realloc(new, new_size);
      }
      new[j++] = s[i];
    }
  }
  free(s);

  new[j] = '\0';
  return new;
}

static char* parameter_expansion(char* param_name, struct hash_table *ht[])
{
  char* param_value = my_strdup(get_data(ht[VAR], param_name));
  free(param_name);
  if (param_value == NULL)
  {
    param_value = my_malloc(1);
    param_value[0] = '\0';
  }

  return param_value;
}

static ssize_t next_unquoted_slash(char* s)
{
  char quoted[3] = { 0 };
  size_t i;
  for (i = 0; s[i]; ++i)
  {
    update_quote(s[i], quoted);
    if (!is_quoted(quoted) && s[i] == '/')
      return i;
  }
  return -1;
}

static char* tilde_expansion(char* input, struct hash_table *ht[])
{
  if (!input || input[0] != '~')
    return input;

  ssize_t end_tp = next_unquoted_slash(input);
  if (end_tp <= 1)
  {
    struct str* output = str_create();
    str_append(output, get_data(ht[VAR], "HOME"), -1, 0);
    if (end_tp == -1)
      end_tp = 1;
    str_append(output, input + end_tp, -1, 0);
    free(input);
    char* res = output->s;
    str_destroy(output, 0);
    return res;
  }
  else
    return input;
}
