#include <stdio.h>
#include <unistd.h>

//#include "main.h"
#include "option_parser.h"
#include "vector.h"
#include "hash_table.h"
#include "lexer.h"
#include "parser.h"
#include "tree.h"
#include "rules.h"

void print_PS(void);

int main(int argc, char* argv[])
{
  struct hash_table *ht = create_hash(256);
  // Remove backslash followed by <newline> cf. 2.2.1
  struct option option = parse_options(argc, argv, ht);
  struct rule** rules = init_all_rules();
  if (option.input_mode == INTERACTIVE)
  {
    while (1)
    {
      print_PS();
      char buff[100]; // A MODIFER
      buff[read(STDOUT_FILENO, buff, 90)] = '\0';
      struct vector* v_token = v_create();
      lexer(buff, v_token);
      v_print(v_token);
      int nb_token_read = 0;
      struct tree* ast = parse(rules, v_token, INPUT, &nb_token_read);
      tree_print(ast, 0);
      v_destroy(v_token);
      tree_destroy(ast);
      /*ast = parse_command(token_list);
      char* ast_print = get_data(ht, "ast_print");
      if (ast_print && !strcmp("0", ast_print))
        print_AST(ast);
      execute_command(option, ast);*/
    }
  }
  else
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
  }
}

void print_PS(void)
{
  printf(">");
  fflush(stdout);
}
