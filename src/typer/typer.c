#include <stddef.h>

#include "typer.h"
#include "my_string.h"
#include "lexer.h"
#include "my_string.h"
#include "string.h"

static char operator_list[][10] =
{
  ";", "&", "|", "&&", "||", ";;", "<", ">", "<<", ">>", "<&", ">&",
  "<>", "<<-", ">|", "{", "}", "(", ")", "!", ""
};
static char reserved_word[][10] =
{
  "if", "then", "else", "elif", "fi", "do", "done", "case", "esac", "while",
  "until", "for", "in", "function", "\n", ""
};


static enum terminal_symbol brw_if[] =
{
  R_BRACE,
  BANG, PIPE,
  AND_IF, OR_IF,
  AND, SEMI, NL,
  L_BRACE, L_PAR, R_PAR, IF, THEN, ELSE, ELIF, DO, WHILE, UNTIL // + NULL
};

static enum terminal_symbol brw_function[] =
{
  BANG, PIPE,
  AND_IF, OR_IF,
  AND, SEMI, NL,
  L_BRACE, L_PAR, R_PAR, IF, THEN, ELSE, ELIF, DO, WHILE, UNTIL // + NULL
};

static enum terminal_symbol brw_end_compound_list[] =
{
  AND, SEMI, NL
};

static enum terminal_symbol brw_esac[] =
{
  DSEMI, IN, R_BRACE
};

static enum terminal_symbol* brw_match[NL - IF] = //nb reserved_word
{
  brw_if,
  brw_end_compound_list,
  brw_end_compound_list,
  brw_end_compound_list,
  brw_end_compound_list, // FI
  brw_end_compound_list, // DO
  brw_end_compound_list, // DONE
  brw_end_compound_list, // CASE
  brw_esac,
  brw_if, // WHILE
  brw_if, // UNTIL
  brw_if, // FOR
  NULL,
  brw_function
};

static size_t brw_match_len[NL - IF] = //nb reserved_word
{
  sizeof (brw_if) / sizeof (enum terminal_symbol),
  sizeof (brw_end_compound_list) / sizeof (enum terminal_symbol),
  sizeof (brw_end_compound_list) / sizeof (enum terminal_symbol),
  sizeof (brw_end_compound_list) / sizeof (enum terminal_symbol),
  sizeof (brw_end_compound_list) / sizeof (enum terminal_symbol),
  sizeof (brw_end_compound_list) / sizeof (enum terminal_symbol),
  sizeof (brw_end_compound_list) / sizeof (enum terminal_symbol),
  sizeof (brw_end_compound_list) / sizeof (enum terminal_symbol),
  sizeof (brw_esac) / sizeof (enum terminal_symbol),
  sizeof (brw_if) / sizeof (enum terminal_symbol), // WHILE
  sizeof (brw_if) / sizeof (enum terminal_symbol), // UNTIL
  sizeof (brw_if) / sizeof (enum terminal_symbol), // FOR
  0,
  sizeof (brw_function) / sizeof (enum terminal_symbol)
};

static int is_among(enum terminal_symbol sym, enum terminal_symbol arr[],
                    size_t len)
{
  for (size_t i = 0; i < len; ++i)
    if (sym == arr[i])
      return i;
  return -1;
}

void typer(struct vector* v_token)
{
  struct token* token;
  for (size_t i = 0; i < v_size(v_token); ++i)
  {
    token = v_get(v_token, i);
    if (token->id != UNDIFINED)
      continue;
    if (token->s == NULL)
    {
      token->id = EOF_SYM;
      continue;
    }
    //printf("lexer = %s\n", token->s);
    int index_operator_list = my_is_in(token->s, operator_list);
    if (index_operator_list != -1) // Rule 1
    {
      token->id = index_operator_list;
      continue;
    }
    if (is_number(token->s))
    {
      struct token* next = v_get(v_token, i + 1);
      if (next && next->s && (next->s[0] == '<' || next->s[0] == '>'))
      {
        token->id = IO_NUMBER;
        continue;
      }
    }
    struct token* prev = v_get(v_token, i - 1);
    int index_rw = my_is_in(token->s, reserved_word);
    if (index_rw != -1 && index_rw != IN - IF)
    {
      int can_be_at_begining = 0;
      if (brw_match[index_rw] == brw_if
          || brw_match[index_rw] == brw_function)
        can_be_at_begining = 1;

      size_t len = brw_match_len[index_rw];
      if ((can_be_at_begining && prev == NULL)
          || (prev && is_among(prev->id, brw_match[index_rw], len) != -1))
      {
        token->id = IF + index_rw;
        continue;
      }
    }
    // = not in 1st position
    if (strchr(token->s, '=') > token->s && !is_digit(token->s[0]))
    {
      token->id = ASSIGNMENT_WORD;
      continue;
    }

    struct token* prev_prev = v_get(v_token, i - 2);
    if (prev_prev && (prev_prev->id == FOR || prev_prev->id == CASE))
    {
      if (my_is_in(token->s, reserved_word) == IN - IF)
      {
        token->id = IN;
        continue;
      }
    }
    token->id = WORD;
    
  }
}
