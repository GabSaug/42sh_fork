#include <stdlib.h>
#include <stdio.h>

#include "expansion.h"
#include "vector.h"
#include "str.h"
#include "string.h"
#include "hash_table.h"
#include "tokenize.h"
#include "arithmetic.h"
#include "my_malloc.h"

extern struct hash_table* ht[2];

//static char* tilde_expansion(char* s);
static char* parameter_expansion(char* s);
static char* remove_quote(char* s);

struct vector* expand(char* input, int in_ari_exp)
{
  /*struct vector* v = v_create();
v_append(v, input);

return v;*/

  struct str* output = str_create();
  size_t start = 0;
  size_t i_input;
  for (i_input = 0; input[i_input]; ++i_input)
  {
    struct expansion exp = tokenize_expansion(input + i_input);

    if (exp.type == ARI)
    {
      str_append(output, input + start, i_input - start, 0);
      str_append(output, arithmetic_expansion(my_strndup(exp.content_start, exp.content_size)), -1, 1);
      i_input += exp.size - 1;
      start = i_input;
    }
    else if (exp.type == NORMAL || exp.type == BRACKET || in_ari_exp)
      // in_ari_exp must be the last elseif
    {
      str_append(output, input + start, i_input - start, 0);
      str_append(output, parameter_expansion(my_strndup(exp.content_start, exp.content_size)), -1, 1);
      //printf("exp.size = %zu\n", exp.size);
      start = i_input + exp.size;
      i_input = start - 1;
    }
    /*else
      str_append(output, input, -1, 0);*/
  }
  str_append(output, input + start, i_input - start, 0);
  //output[i_output] = '\0';

  //char* tilde_s = tilde_expansion(s);
  free(input);
  /*if (!output)
  {
    output = my_malloc(1);
    output[0] = '\0';
  }*/

  struct vector* v = v_create();
  v_append(v, output->s);

  str_destroy(output, 0);

  //printf("v->size = %zu, s = %s\n", v->size, v_get(v, 0));

  for (size_t i = 0; i < v_size(v); ++i)
    v_set(v, i, remove_quote(v_get(v, i)));

  return v;

  /*char* param_s = parameter_expansion(tilde_s);
  char* comm_s = command_substitution(param_s);
  free(tilde_s);
  free(param_s);
  
  struct vector* v = v_create();
  v = v_add(v, comm_s);
  char* IFS = get_data(ht[VAR], "IFS");
  if (IFS && IFS != '\0') // IFS is not null
    field_split(v);*/
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
    //printf("s[i] = %c, %.*s\n", s[i], (int)j, new);
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

static char* parameter_expansion(char* param_name)
{
  //printf("param expansion; param_name= %s ", param_name);

  /*printf("exp.size = %zu, exp.type = %i, size_s = %i, exp.s = %.*s\n",
         exp.size, exp.type, (int)(exp.end - exp.start),
         (int)(exp.end - exp.start), exp.start);*/

  char* param_value = my_strdup(get_data(ht[VAR], param_name));
  free(param_name);
  if (param_value == NULL)
  {
    param_value = malloc(1);
    param_value[0] = '\0';
  }
  //printf("value = %s\n", param_value);
  return param_value;
}

/*static char* next_unquoted_slash(char* s)
{
  for (; *s; ++s)
  {
    if (*s == '\'')
    {
      for (s++; *s && *s != '\''; ++s)
        continue;
      continue;
    }
    if (*s == '"')
    {
      for (s++; *s && *s != '"'; ++s)
        continue;
      continue;
    }
    if (*s == '\\' && s[1])
    {
      s++;
      continue;
    }
    if (*s == '/')
      return s;
  }
  return s;
}

static char* tilde_expansion(char* s)
{
  if (!s)
    return s;
  char* tilde = NULL;
  char* new = NULL;
  size_t size = 0;
  char* start = s;
  char* end_tilde_prefix  = NULL;
  for (start = s; *start != '\0' && (tilde = strchr(start, '~')) != NULL;
       start = end_tilde_prefix)
  {
    size_t extra_size = tilde - start;
    new = realloc(new, size + extra_size);
    memcpy(new + size, start, extra_size);
    size += extra_size;
    end_tilde_prefix = next_unquoted_slash(tilde);
    //if (end_tile_prefix == NULL)
    //  end_tilde_prefix = tilde + strlen(tilde);
    printf("tilde expansion = %*s\n", (int)(end_tilde_prefix - tilde) + 1, tilde);

  }

  free(s);
  return s;
}*/
