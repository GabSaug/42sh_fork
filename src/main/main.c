#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
//#include "main.h"
#include "option_parser.h"
#include "vector.h"
#include "hash_table.h"
#include "lexer.h"
#include "parser.h"
#include "tree.h"
#include "rules.h"
#include "execute.h"
#include "main.h"

static char* get_PS(struct hash_table* ht);
static void process_input(char* buff, struct rule** rules,
                          struct hash_table* ht);

static struct hash_table* ht = NULL;
static struct rule** rules = NULL;
static struct vector* v_token = NULL;
static struct tree* ast = NULL;
static char* buff = NULL;

static int processing = 0;

int main(int argc, char* argv[])
{
  atexit(exit_42sh);
  ht = create_hash(256);
  // Remove backslash followed by <newline> cf. 2.2.1
  struct option option = parse_options(argc, argv, ht);
  rules = init_all_rules();
  if (option.input_mode == INTERACTIVE)
  {
    while (1)
    {
      char* prompt = get_PS(ht);
      buff = readline(prompt);
      if (!buff)
      {
        printf("exit\n");
        return 0;
      }
      add_history(buff);
      process_input(buff, rules, ht);
      free(buff);
    }
  }
  else if (option.input_mode == COMMAND_LINE)
    process_input(option.input, rules, ht);
  else
  {
    int fd = open(option.input, O_RDONLY);
    if (fd == -1)
      err(1, NULL);
    struct stat stat_buf;
    if (stat(option.input, &stat_buf) == -1)
      err(1, "Impossible to read stat from %s", option.input);
    size_t size_file = stat_buf.st_size;
    char* file = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd, 0);
    process_input(file, rules, ht);
    munmap(file, size_file);
    close(fd);
    return 0;
  }
}

static void process_input(char* buff, struct rule** rules, struct hash_table* ht)
{
  processing = 1;
  v_token = v_create();
  if (!lexer(buff, v_token))
  {
    v_destroy(v_token);
    return;
  }
  //v_print(v_token);
  ast = parse(rules, v_token);
  if (ast == NULL)
    printf("Grammar error\n");
  else
  {
    //tree_print(ast);
    if (!strcmp(get_data(ht, "ast-print"), "1"))
      tree_print_dot(ast);
    printf("returned %i\n", execute(ast, ht));
  }
  v_destroy(v_token);
  tree_destroy(ast);
  processing = 0;
}


static char* get_PS(struct hash_table* ht)
{
  char* ps1 = get_data(ht, "PS1");
  return ps1;
}

void exit_42sh(void)
{
  destroy_hash(ht);
  rules_destroy(rules);
  if (processing)
  {
    v_destroy(v_token);
    tree_destroy(ast);
    free(buff);
  }
}
