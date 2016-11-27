#include "expansion.h"
#include "vector.h"

static void split_for(char* ifs, struct vector* v_input,
                      struct vector* v_output, size_t i)
{
    char* s = v_get(v_input, i);
    size_t start = 0;
    size_t j;
    char quoted[3] = { 0 };
    for (j = 0; s[j]; ++j)
    {
      update_quote(s[j], quoted);
      if (is_quoted(quoted))
        continue;
      char* dl = strchr(ifs, s[j]);
      if (dl)
      {
        v_append(v_output, my_strndup(s + start, j - start));
        while (s[j] && strchr(ifs, s[j]))
          j++;
        start = j;
        if (!s[j])
          break;
      }
    }
    if (start < j)
      v_append(v_output, my_strndup(s + start, j - start));
}

struct vector* field_split(struct vector* v_input, struct hash_table *ht[])
{
  char* ifs = get_data(ht[VAR], "IFS");
  if (!v_input || !ifs)
    return v_input;
  struct vector* v_output = v_create();
  for (size_t i = 0; i < v_size(v_input); ++i)
  {
    split_for(ifs, v_input, v_output, i);
  }
  v_destroy(v_input, free);
  return v_output;
}
