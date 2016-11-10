#include "42.sh"

enum input_mode
{
  INTERACTIVE,
  COMMAND_LINE,
  INPUT_FILE
};

struct option
{
  int norc;
  enum input_mode intput_mode;
};

int main(int argc, char* argv[])
{
  //declare hasttable
  // Remove backslash followed by <newline> cf. 2.2.1
  struct option option = parse_option(hashtable);
  if (option.input_mode == INTERACTIVE)
  {
    while (1)
    {
      print_PS();
      token_list = lexer();
      ast = parse_command(token_list);
      if (print_AST)
        print_AST(qst);
      execute_command(option, ast);
    }
  }
  else
  {
    ast = parse_file();
    if (print_AST)
      print_AST(ast);
    execute_command(ast);
  }
}
