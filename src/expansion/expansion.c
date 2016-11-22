#include <stdlib.h>
#include <stdio.h>

#include "expansion.h"
#include "vector.h"
#include "string.h"
#include "hash_table.h"
#include "tokenize.h"

extern struct hash_table* ht[2];

//static char* tilde_expansion(char* s);
static char* parameter_expansion(char* s);
static char* remove_quote(char* s);

struct vector* expand(char* s)
{
  //char* tilde_s = tilde_expansion(s);
  char* param_s = parameter_expansion(s); // Change to tilde_s
struct vector* v = v_create();
v_append(v, param_s);

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

static char* parameter_expansion(char* s)
{
  if (!s || s[0] != '$')
    return s;
  //printf("param expansion; s= %s ", s);

  struct expansion exp = tokenize_expansion(s);
  /*printf("exp.size = %zu, exp.type = %i, size_s = %i, exp.s = %.*s\n",
         exp.size, exp.type, (int)(exp.end - exp.start),
         (int)(exp.end - exp.start), exp.start);*/
  char* param_name = my_strndup(s + 1, exp.size - 1);
  char* param_value = my_strdup(get_data(ht[VAR], param_name));
  free(param_name);
  free(s);
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
