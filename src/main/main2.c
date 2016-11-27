#include <stdio.h>

#include "execute.h"
#include "main.h"
#include "my_malloc.h"

int run_ast(struct shell_tools* tools)
{
  int ret = 0;
  for (size_t i = 0; i < v_size(tools->ast->child); ++i)
  {
    //printf("input %zu\n", i);
    struct tree* input = v_get(tools->ast->child, i);
    if (input == NULL)
    {
      warnx("Grammar error");
      ret = 1;
    }
    else
    {
      ret = execute(input, tools->ht);
      char* ret_itoa = my_malloc(sizeof (char) * 50);
      sprintf(ret_itoa, "%i", ret);
      add_hash(tools->ht[VAR], "?", ret_itoa);
      free(ret_itoa);
    }
  }
  return ret;
}
