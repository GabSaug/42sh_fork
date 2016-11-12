#include <string.h>

#include "my_string.h"

/* If the string from start to end match with the begining of a string in
** s_list, is_in() return the index in s_list
** If no match is found, return -1;
**
** s_list must be null terminated
** The element of s_list must be null terminated
*/
int begin_as(char* start, char* end, char s_list[][10])
{
  size_t len = end - start + 1;
  for (int i = 0; s_list[i][0]; ++i)
    if (!strncmp(start, s_list[i], len) && strlen(s_list[i]) >= len)
      return i;
  return -1;
}

int is_in(char* start, char* end, char s_list[][10])
{
  size_t len = end - start + 1;
  for (int i = 0; s_list[i][0]; ++i)
    if (!strncmp(start, s_list[i], len) && strlen(s_list[i]) == len)
      return i;
  return -1;

}
