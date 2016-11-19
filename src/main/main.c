#include <stdio.h>
#include <unistd.h>
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

static char* get_PS(void);
static int process_input(char* buff, struct rule** rules);
static int process_file(struct option option, struct rule** rules);
static int process_interactive(void);

struct hash_table* ht[2] = { NULL, NULL };
static struct rule** rules = NULL;
static struct vector* v_token = NULL;
static struct tree* ast = NULL;
static char* buff = NULL;

static int processing = 0;

int main(int argc, char* argv[])
{
  int ret = 0;
  atexit(exit_42sh);
  ht[VAR] = create_hash(256);
  ht[ALIAS] = create_hash(256);
  // Remove backslash followed by <newline> cf. 2.2.1
  struct option option = parse_options(argc, argv);
  rules = init_all_rules();
  if (option.input_mode == INTERACTIVE)
    process_interactive();
  else if (option.input_mode == COMMAND_LINE)
    ret = process_input(option.input, rules);
  else
    process_file(option, rules);
  return ret;
}

static int process_interactive(void)
{
  int ret = 0;
  while (1)
  {
    char* prompt = get_PS();
    buff = readline(prompt);
    if (!buff)
    {
      printf("exit\n");
      return ret;
    }
    add_history(buff);
    ret = process_input(buff, rules);
    free(buff);
  }
  return ret;
}

static int process_file(struct option option, struct rule** rules)
{
  int ret = 0;
  int fd = open(option.input, O_RDONLY | O_CLOEXEC);
  if (fd == -1)
    err(1, NULL);
  struct stat stat_buf;
  if (stat(option.input, &stat_buf) == -1)
    err(1, "Impossible to read stat from %s", option.input);
  size_t size_file = stat_buf.st_size;
  char* file = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd, 0);
  ret = process_input(file, rules);
  munmap(file, size_file);
  close(fd);
  return ret;
}

static int process_input(char* buff, struct rule** rules)
{
  int ret = 0;
  processing = 1;
  v_token = v_create();
  if (!lexer(buff, v_token))
  {
    v_destroy(v_token, token_destroy);
    return 1;
  }
  //v_print(v_token);
  ast = parse(rules, v_token);
  if (ast == NULL)
    printf("Grammar error\n");
  else
  {
    //tree_print(ast);
    if (!strcmp(get_data(ht[VAR], "ast-print"), "1"))
      tree_print_dot(ast);
    ret = execute(ast);
    //printf("returned %i\n", execute(ast, ht));
    tree_destroy(ast);
  }
  v_destroy(v_token, token_destroy);
  processing = 0;
  return ret;
}


static char* get_PS(void)
{
  char* ps1 = get_data(ht[VAR], "PS1");
  return ps1;
}

void exit_42sh(void)
{
  destroy_hash(ht[VAR]);
  destroy_hash(ht[ALIAS]);
  rules_destroy(rules);
  if (processing)
  {
    v_destroy(v_token, token_destroy);
    tree_destroy(ast);
    free(buff);
  }
}
