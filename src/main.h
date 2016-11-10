#ifndef MAIN_H
# define MAIN_H

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


#endif /* !MAIN_H */
