#define _DEFAULT_SOURCE
#include <stdio.h>
#undef _DEFAULT_SOURCE
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "option_parser.h"
#include "vector.h"
#include "hash_table.h"
#include "lexer.h"
#include "parser.h"
#include "tree.h"
#include "rules.h"
#include "execute.h"
#include "main.h"
#include "typer.h"
#include "signals.h"
#include "str.h"

static int process_file(struct option option);
static int process_interactive(void);

extern int g_in_readline;

struct hash_table* ht[3] = { NULL, NULL, NULL };
static struct rule** rules = NULL;
static struct vector* v_token = NULL;
static struct tree* ast = NULL;
static char* buff = NULL;

static int processing = 0;
static int tty;

int main(int argc, char* argv[])
{
  rl_already_prompted = 1;
  g_in_readline = 0;
  atexit(exit_42sh);
  set_sigacts();
  ht[VAR] = create_hash(256);
  ht[FUN] = create_hash(256);
  ht[ALIAS] = create_hash(256);
  // Remove backslash followed by <newline> cf. 2.2.1
  struct option option = parse_options(argc, argv);
  rules = init_all_rules();
  tty = isatty(STDIN_FILENO);
  if (option.input_mode == INTERACTIVE)
    return process_interactive();
  else if (option.input_mode == COMMAND_LINE)
    return process_input(option.input, v_token);
  else
    return process_file(option);
}

static void write_history_log(char* s)
{
  char* home = getenv("HOME");
  if (!home)
    return;
  struct str* str = str_create();
  str_append(str, home, -1, 0);
  str_append(str, "/.42sh_history", -1, 0);
  int fd = open(str->s, O_WRONLY | O_CLOEXEC | O_CREAT | O_APPEND, 0666);
  str_destroy(str, 1);
  if (fd == -1)
  {
    warn("write_history");
    return;
  }
  dprintf(fd, "%s\n", s);
  close(fd);
}

static int process_interactive(void)
{
  int ret = 0;
  while (1)
  {
    char* prompt = get_PS();
    if (tty)
      write(STDERR_FILENO, prompt, my_strlen(prompt));
    g_in_readline = 1;
    buff = readline(prompt);
    g_in_readline = 0;
    if (!buff)
    {
      printf("exit\n");
      return ret;
    }
    if (strlen(buff) != 0)
    {
      add_history(buff);
      write_history_log(buff);
      ret = process_input(buff, v_token);
    }
    free(buff);
  }
  return ret;
}

static int process_file(struct option option)
{
  int ret = 0;
  int fd = open(option.input, O_RDONLY | O_CLOEXEC);
  if (fd == -1)
  {
    warn("Error to open file");
    return 1;
  }
  struct stat stat_buf;
  if (stat(option.input, &stat_buf) == -1)
  {
    warn("Impossible to read stat from %s", option.input);
    return 1;
  }
  size_t size_file = stat_buf.st_size;
  char* file = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd, 0);
  ret = process_input(file, v_token);
  munmap(file, size_file);
  close(fd);
  return ret;
}

static int run_ast(struct tree *ast, struct vector *token)
{
  int ret = 0;
  if (ast == NULL)
  {
    warnx("Grammar error");
    ret = 1;
  }
  else
  {
    //tree_print(ast);
    if (!strcmp(get_data(ht[VAR], "ast-print"), "1"))
      tree_print_dot(ast);
    ret = execute(ast);
    //printf("returned %i\n", execute(ast, ht));
    //struct vector* v_fun = v_create();
    //tree_destroy_ast_extract_fun(ast, v_fun);
    tree_destroy(ast);
    //v_destroy(v_fun, tree_destroy_fun);
  }
  v_destroy(token, token_destroy);
  token = NULL;
  processing = 0;

  return ret;
}

int process_input(char* buff, struct vector *token)
{
  processing = 1;
  //printf("buff = %s\n", buff);
  token = v_create();
  if (!lexer(buff, token))
  {
    //printf("lexer failure\n");
    v_destroy(token, token_destroy);
    token = NULL;
    return 1;
  }
  typer(token);
  //v_print(token);
  //printf("lexer success\n");
  int fit_level = 0;
  ast = parse(rules, token, &fit_level);
  int ret = run_ast(ast, token);

  return ret;
}

char* get_PS(void)
{
  char* ps1 = get_data(ht[VAR], "PS1");
  return ps1;
}

void exit_42sh(void)
{
  destroy_hash(ht[VAR]);
  destroy_hash(ht[FUN]);
  destroy_hash(ht[ALIAS]);
  rules_destroy(rules);
  if (processing)
  {
    v_destroy(v_token, token_destroy);
    tree_destroy(ast);
    free(buff);
  }
}
