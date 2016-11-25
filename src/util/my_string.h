#ifndef MY_STRING_H
# define MY_STRING_H

#include <stddef.h>

/**
 * \file my_string.h
*/

enum quote_type
{
  BACKSLASH,
  SINGLE_QUOTE,
  DOUBLE_QUOTE
};
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
int my_begin_as(char* str, char s_list[][10]);

/**
 * \brief Compare two strings
 * \param str1 The first string to compare
 * \param str2 The second string to compare
 * \return Return 0 if the strings are equals
*/
int my_strcmp(char* str1, char* str2);

/**
 * \brief Verify if the character is a digit or not
 * \param c The character to verify
 * \return Return 1 if the character is a digit, 0 otherwise
*/
int is_digit(char c);

/**
 * \brief Verify if a string represents a number
 * \param s The string to verify
 * \return Return 1 if the string represents a number, 0 otherwise
*/
int is_number(char* s);

int is_name_char(char c);
int is_login_char(char c);

/**
 * \brief Convert a string to an int
 * \param str The string to convert
 * \return The intenger from the string, or - 1 if there is a problem
*/
int my_atoi(const char str[]);

/**
 * \brief Duplicate a string with a new allocation
 * \param s The string to duplicate
 * \return The duplicated string
*/
char* my_strdup(char* s);

/**
 * \brief Duplicate the n first byte of a string
 * \param s The string to duplicate
 * \param n The number of bytes to duplicate
 * \return The duplicated string
*/
char* my_strndup(char* s, size_t n);


/**
 * \brief Calculate the size of a string
 * \param s The string
 * \return The number of char in the string
*/
size_t my_strlen(char* s);

int is_prefix_arr(char* str, char s_list[][10]);

int update_quote(char c, char quoted[]);
int is_quoted(char quoted[3]);

#endif /* !MY_STRING_H */
