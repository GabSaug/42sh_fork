#include "execute.h"

int builtin_execution(struct tree *ast, struct hash_table *ht, int bi)
{
  ht = ht;
  switch (bi)
  {
  case 0:
    exit(EXIT_SUCCESS);
  case 1:
    return 0;
  case 2:
    return 1;
  default:
    warn("%s: Invalid builtin", get_child_elt(ast, 0));
    return 1;
  // TODO Execute buitins
  }
}
