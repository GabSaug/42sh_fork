#ifndef MY_STRING_H
# define MY_STRING_H

/**
 * \file my_string.h
*/

/**
 * \brief Determine if the string could be the begining of one of the string in
 * the array of string
 * \param s_list must be NULL_terminated and its elements must be \0 terminated
 * \return The index of the string such that the argument is its prefix. Return
 * -1 if no such string has been found
*/
int begin_as(char* start, char* end, char s_list[][10]);
/**
 * \brief Determine if the string is among the string in a
 * array of string
 * \param s_list must be NULL_terminated and its elements must be \0 terminated
 * \return The index of the string that is equal to the argument. Return -1 if
 * no string match the argument
*/
int is_in(char* start, char* end, char s_list[][10]);
/**
 * \brief Duplicate the string given in argument
 * \param s The string to duplicate
 * \return The address of the newly malloc'd string
*/
int my_is_in(char* str, char s_list[][10]);
int my_strcmp(char* str1, char* str2);
int is_digit(char c);
int is_number(char* s);
char* my_strdup(char* s);
char* my_strndup(char* s, size_t n);
int is_prefix_arr(char* str, char s_list[][10]);

#endif /* !MY_STRING_H */
