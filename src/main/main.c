#define _DEFAULT_SOURCE
#include <stdio.h>
#undef _DEFAULT_SOURCE
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "vector.h"
#include "lexer.h"
#include "parser.h"
#include "tree.h"
#include "rules.h"
#include "execute.h"
#include "main.h"
#include "typer.h"
#include "signals.h"
#include "str.h"

static int process_file(struct shell_tools tools);
static int process_interactive(struct shell_tools tools);

static struct rule** rules = NULL;
static struct vector* v_token = NULL;
static struct tree* ast = NULL;
static char *buff;

static int processing = 0;
static int tty;
static int pid;
struct option option = { 0 };
extern int g_in_readline;

static char* prompt = NULL;

static FILE* get_null_file(void)
{
  FILE* f = fopen("/dev/null", "a");
  return f;
}

void print_prompt(void)
{
  write(STDERR_FILENO, prompt, my_strlen(prompt));
}

int main(int argc, char* argv[])
{
  rl_already_prompted = 1;
  g_in_readline = 0;
  set_sigacts();
  atexit(exit_42sh);
  struct shell_tools tools;
  tools.ht[VAR] = create_hash(256);
  tools.ht[FUN] = create_hash(256);
  tools.ht[ALIAS] = create_hash(256);
  // Remove backslash followed by <newline> cf. 2.2.1
  tools.option = parse_options(argc, argv, tools.ht);
  prompt = NULL;
  rules = init_all_rules();
  tty = isatty(STDIN_FILENO);
  pid = getpid();
  if (!tty)
    rl_outstream = get_null_file();
  if (tools.option.input_mode == INTERACTIVE)
    return process_interactive(tools);
  else if (tools.option.input_mode == COMMAND_LINE)
    return process_input(tools.option.input, v_token, tools);
  else
    return process_file(tools);
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

static int process_interactive(struct shell_tools tools)
{
  int ret = 0;
  while (1)
  {
    prompt = get_PS(tools.ht);
    if (tty)
      print_prompt();
    g_in_readline = 1;
    buff = readline(prompt);
    g_in_readline = 0;
    if (!buff)
    {
      return ret;
    }
    if (strlen(buff) != 0)
    {
      add_history(buff);
      write_history_log(buff);
      ret = process_input(buff, v_token, tools);
    }
    free(buff);
  }
  return ret;
}

static int process_file(struct shell_tools tools)
{
  int ret = 0;
  int fd = open(tools.option.input, O_RDONLY | O_CLOEXEC);
  if (fd == -1)
  {
    warn("Error to open file");
    return 1;
  }

  struct stat stat_buf;
  if (stat(tools.option.input, &stat_buf) == -1)
  {
    warn("Impossible to read stat from %s", tools.option.input);
    return 1;
  }
  size_t size_file = stat_buf.st_size;
  char* file = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd, 0);
  ret = process_input(file, v_token, tools);
  munmap(file, size_file);
  close(fd);
  return ret;
}

static int run_ast(struct tree *ast, struct vector *token,
                   struct shell_tools tools)
{
  int ret = 0;
  if (ast == NULL)
  {
    warnx("Grammar error");
    ret = 1;
  }
  else
  {
    ret = execute(ast, tools.ht);
    char* ret_itoa = my_malloc(sizeof (char) * 50);
    sprintf(ret_itoa, "%i", ret);
    add_hash(tools.ht[VAR], "?", ret_itoa);
    free(ret_itoa);
  }

  return ret;
}

int process_input(char* buff, struct vector *token, struct shell_tools tools)
{
  processing = 1;
  token = v_create();
  if (!lexer(buff, token))
  {
    v_destroy(token, token_destroy);
    token = NULL;
    return 1;
  }
  typer(token);

  int fit_level = 0;
  ast = parse(rules, token, &fit_level);
  int ret = 0;
  if (ast != NULL)
  {
    if (!strcmp(get_data(tools.ht[VAR], "ast-print"), "1"))
      tree_print_dot(ast);

    for (size_t i = 0; i < v_size(ast->child); ++i)
    {
      //printf("input %zu\n", i);
      struct tree* input = v_get(ast->child, i);
      ret = run_ast(input, token, tools);
    }

    tree_destroy(ast);
  }
  else
  {
    warnx("Grammar error");
    ret = 1;
  }

  v_destroy(token, token_destroy);
  token = NULL;
  processing = 0;
  return ret;
}

char* get_PS(struct hash_table *ht[])
{
  char* ps1 = get_data(ht[VAR], "PS1");
  return ps1;
}

void exit_42sh(void)
{
  if (!tty)
    fclose(rl_outstream);
  //else if (tools.option.input_mode == INTERACTIVE && getpid() == pid)
  //  printf("exit\n");
  //destroy_hash(tools.ht[VAR]);
  //destroy_hash(tools.ht[FUN]);
  //destroy_hash(tools.ht[ALIAS]);
  rules_destroy(rules);
  if (processing)
  {
    v_destroy(v_token, token_destroy);
    tree_destroy(ast);
    free(buff);
  }
}
