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

struct vector* expand(char* s)
{
  //char* tilde_s = tilde_expansion(s);
  char* param_s = parameter_expansion(s); // Change to tilde_s
struct vector* v = v_create();
v_append(v, param_s);
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

static char* parameter_expansion(char* s)
{
  if (!s || s[0] != '$')
    return s;
  //printf("param expansion; s= %s ", s);

  //char* param_name = s + 1; // CHANGE !!!
  /*size_t param_size = tokenize_exp_normal(s);
  if (param_size <= 1)
  {

    printf("fuck\n");
  }*/
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
