#include <stdio.h>

#include "lexer.h"
#include "my_malloc.h"
#include "my_string.h"
#include "vector.h"
#include "tokenize.h"

static char operator_list[][10] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&",
  "<>", "<<-", ">|", ""
};
/*static char reserved_word[][10] =
{
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "{", "}", "(", ")", "!", "in", "function", "\n", ""
};*/

static int is_digit(char c)
{
  return ('0' <= c && c <= '9');
}

static int is_number(char* s)
{
  for (size_t i = 0; s[i]; ++i)
    if (!is_digit(s[i]))
      return 0;
  return 1;
}

static char quote_symbol[][10] =
{
  "\\", "'", "\"", "\0"
};

static char blank_list[][10] =
{
  "\n", " ", "\t"
};

static size_t append_token(struct vector* v_token, char* start, char* end);
static int is_quoted(char quoted[3]);

static int apply_rule(struct vector* v_token, char quoted[],
                      char* part_of_operator,
                      char* part_of_word, char* s, size_t* i, char** start);
static int apply_rule_2(struct vector* v_token, char quoted[],
                        char* part_of_operator, char* part_of_word, char* s,
                        size_t* i, char** start);
static void rule_3(struct vector* v_token, char** start, char* s, size_t i,
                   char* part_of_operator);
static void rule_4(char c, char quoted[]);
static void rule_7(struct vector* v_token, char** start, char* s, size_t* i,
                   char* part_of_operator, char* part_of_word);

int lexer(char* s, struct vector* v_token)
{
  char* start = s;
  char part_of_operator = 0;
  char part_of_word = 0;
  char quoted[3] = {0};
  //enum terminal_symbol curr_token = UNDIFINED;
  // Contain { is_backslah_quoted, is_single_quoted, is double_quoted }
  size_t i;
  for (i = 0; s[i]; ++i)
  {
    //v_print(v_token);
    if (apply_rule(v_token, quoted, &part_of_operator,
                   &part_of_word, s, &i, &start) == -1)
      return 0;
  }
  if (i != 0) // There is not any token
    append_token(v_token, start, s + i - 1);
  append_token(v_token, NULL, NULL);
  return 1;
}

static int apply_rule(struct vector* v_token, char quoted[],
                      char* part_of_operator, char* part_of_word, char* s,
                      size_t* i, char** start)
{
  if (quoted[BACKSLASH] > 0)
    quoted[BACKSLASH]--;
  // Rule 1 is handle after the for loop
  // Rule 2
  if (*part_of_operator && !is_quoted(quoted)
      && begin_as(*start, s + *i, operator_list) != -1)
  {
    //*curr_token = begin_as(*start, s + *i, operator_list);
    return 0;
  }
  // Rule 3
  if (*part_of_operator && begin_as(*start, s + *i, operator_list) == -1)
    rule_3(v_token, start, s, *i, part_of_operator);
  // Rule 4
  if (begin_as(s + *i, s + *i, quote_symbol) >= 0)
    rule_4(s[*i], quoted);
  // Rule 5
  if ((s[*i] == '$' || s[*i] == '`') && !quoted[BACKSLASH]
      && !quoted[SINGLE_QUOTE])
  {
    int ret = tokenize_expansion(s + *i);
    if (ret == 0)
      return -1;
    *i += ret - 1;
    return 0;
  }
  apply_rule_2(v_token, quoted, part_of_operator,
                 part_of_word, s, i, start);
  return 0;
}

static int apply_rule_2(struct vector* v_token, char quoted[],
                        char* part_of_operator, char* part_of_word, char* s,
                        size_t* i, char** start)
{
  // Rule 6
  int op = begin_as(s + *i, s + *i, operator_list); 
  if (!is_quoted(quoted) && op != -1)
  {
    if (*start != s + *i)
    {
      size_t index = append_token(v_token, *start, s + *i - 1);
      struct token* tok = v_get(v_token, index);
      if (is_number(tok->s) && (op == LESS || op == GREAT || op == DLESS
          || op == CLOBBER))
        tok->id = IO_NUMBER;
    }
    *start = s + *i;
    *part_of_operator = 1;
    //*curr_token = begin_as(s + *i, s + *i, operator_list);
    return 0;
  }
  // Rule 7
  if (begin_as(s + *i, s + *i, blank_list) >= 0 && !is_quoted(quoted))
  {
    rule_7(v_token, start, s, i, part_of_operator, part_of_word);
    return 0;
  }
  // Rule 8
  if (*part_of_word)
    return 0;
  // Rule 9
  if (s[*i] == '#')
    *i += tokenize_comment(s, *i);
  // Rule 10
  else
    *part_of_word = 1;
    //*curr_token = WORD;
  return 0;
}

static void rule_7(struct vector* v_token, char** start, char* s, size_t* i,
                   char* part_of_operator, char* part_of_word)
{
  if (*start != s + *i)
    append_token(v_token, *start, s + *i - 1);
  /*if (s[*i] == '\n')
    append_token(v_token, NULL, NULL);*/
  *part_of_operator = 0;
  *part_of_word = 0;
  *start = s + *i + 1;
}

static void rule_3(struct vector* v_token, char** start, char* s, size_t i,
                   char* part_of_operator)
{
  if (is_in(*start, s + i - 1, operator_list) != -1)
  {
    append_token(v_token, *start, s + i - 1);
    *start = s + i;
    *part_of_operator = 0;
  }
  else
    *part_of_operator = 0;
}

static void rule_4(char c, char quoted[])
{
  // inverse boolean
  if (c == '\\' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
    quoted[BACKSLASH] = 2; // Decrease at each loop iteration
  if (c == '\'' && !quoted[BACKSLASH] && !quoted[DOUBLE_QUOTE])
    quoted[SINGLE_QUOTE] = 1;
  if (c == '"' && !quoted[BACKSLASH] && !quoted[SINGLE_QUOTE])
    quoted[DOUBLE_QUOTE] = 1;
}


static size_t append_token(struct vector* v_token, char* start, char* end)
{
  /*if (token_id == UNDIFINED)
    return;*/
  struct token* new_token = my_malloc(sizeof(struct token));
  new_token->id = UNDIFINED;
  size_t s_size = (end - start) + 1;
  char* s = my_malloc(s_size + 1); // +1 for '\0'
  if (start && end)
  {
    for (size_t i = 0; i < s_size + 1; ++i)
      s[i] = start[i];
    s[s_size] = '\0';
    new_token->s = s;
  }
  else
    new_token->s = NULL;

  /*if (token_id == WORD)
  {
    int index_reserved_word = is_in(start, end, reserved_word);
    if (index_reserved_word != -1)
    {
      new_token->id = IF + index_reserved_word;
      new_token->s = NULL;
    }
    else
    {
      size_t s_size = (end - start) + 1;
      char* s = my_malloc(s_size + 1); // +1 for '\0'
      for (size_t i = 0; i < s_size + 1; ++i)
        s[i] = start[i];
      s[s_size] = '\0';
      new_token->s = s;
    }
  }*/
  //new_token->id = UNDIFINED;
  return v_append(v_token, new_token);
}

static int is_quoted(char quoted[3])
{
  for (int i = 0; i < 3; i++)
    if (quoted[i])
      return 1;
  return 0;
}

void token_destroy(void* p)
{
  struct token* token = p;
  free(token->s);
  free(token);
}

