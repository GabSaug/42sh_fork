#include "main.h"
#include "option_parser.h"
#include "hash_table.h"

int main(int argc, char* argv[])
{
  struct hash_table *ht = create_hash(256);
  // Remove backslash followed by <newline> cf. 2.2.1
  struct option option = parse_options(argc, argv, ht);
  if (option.input_mode == INTERACTIVE)
  {
    while (1)
    {
      print_PS();
      char* buff[100]; // A MODIFER
      buff[read(STDOUT_FILENO, buff, 90)] = '\0';
      struct vector* v_token = v_create();
      lexer(v_token);
      /*ast = parse_command(token_list);
      char* ast_print = get_data(ht, "ast_print");
      if (ast_print && !strcmp("0", ast_print))
        print_AST(ast);
      execute_command(option, ast);*/
    }
  }
  else
  {
    token_list = lexer();
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
