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

static char* get_PS(struct hash_table* ht);
static int process_input(char* buff, struct rule** rules, struct hash_table* ht);

static struct hash_table* ht;
static struct rule** rules;


int main(int argc, char* argv[])
{
  ht = create_hash(256);
  // Remove backslash followed by <newline> cf. 2.2.1
  struct option option = parse_options(argc, argv, ht);
  rules = init_all_rules();
  if (option.input_mode == INTERACTIVE)
  {
    while (1)
    {
      char* prompt = get_PS(ht);
      char* buff;
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
  {
    char buff[100]; // A MODIFER
    buff[read(STDOUT_FILENO, buff, 90)] = '\0';
    struct vector* v_token = v_create();
    lexer(buff, v_token);
    v_destroy(v_token);
    /*ast = parse_file();
    if (print_AST)
      print_AST(ast);
    execute_command(ast);*/
    destroy_hash(ht);
  }
  else
  {
    printf("file = %s\n", option.file_name);
    int fd = open(option.file_name, O_RDONLY);
    if (fd == -1)
      err(1, NULL);
    struct stat stat_buf;
    if (stat(option.file_name, &stat_buf) == -1)
      err(1, "Impossible to read stat from %s", option.file_name);
    size_t size_file = stat_buf.st_size;
    char* file = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd, 0);
    int ret = process_input(file, rules, ht);
    munmap(file, size_file);
    fclose(fd);
    return ret;
  }
}

static int process_input(char* buff, struct rule** rules, struct hash_table* ht)
{
  struct vector* v_token = v_create();
  if (!lexer(buff, v_token))
  {
    v_destroy(v_token);
    return 0;
  }
  v_print(v_token);
  struct tree* ast = parse(rules, v_token);
  if (ast == NULL)
    printf("Grammar error\n");
  else
  {
    tree_print(ast);
    if (!strcmp(get_data(ht, "ast-print"), "1"))
      tree_print_dot(ast);
    //printf("returned %i\n", execute(ast));
  }
  v_destroy(v_token);
  tree_destroy(ast);
}


static char* get_PS(struct hash_table* ht)
{
  char* ps1 = get_data(ht, "PS1");
  return ps1;
}

void exit_42sh(void)
{
  // free v_token, tree, hash_table
}
