#ifndef MY_STRING_H
# define MY_STRING_H

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

int begin_as(char* start, char* end, char** s_list);

#endif /* !MY_STRING_H */
