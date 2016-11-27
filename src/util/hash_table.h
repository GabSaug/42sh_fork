#ifndef HASH_TABLE_H
# define HASH_TABLE_H

#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#undef _GUN_SOURCE

#include "my_string.h"

/**
 * \file hash_table.h
*/

enum hash_table_id
{
  VAR,
  FUN,
  ALIAS,
  NB_HT
};

struct elt_hash
{
  char *key;
  void *data;
  struct elt_hash *next;
};

struct hash_table
{
  size_t capacity;
  size_t nb_elt;
  struct elt_hash **table;
};

/**
 * \brief Create a hash_table
 * \param capacity The number of element that trigger the increase in the hash
 * table capacity
 * \return The created hash_table, or NULL if an error occurred
*/
struct hash_table *create_hash(size_t capacity);

/**
 * \brief Destroy the hash_table
 * \param ht The hash_table to destroy
*/
void destroy_hash(struct hash_table *ht);

/**
 * \brief Add or modify the data of an element
 * \param ht The hash_table to add or modified the value
 * \param key The key of the value in the hash_table
 * \param data The data to add or modify
 * \return The hash_table modified, or NULL if an error occurred
*/
struct hash_table *add_hash(struct hash_table *ht, char *key, void *data);

/**
 * \brief Get a vaule stored in the hash_table
 * \param ht The hash_table where the data is stored
 * \param key The key of the data wanted
 * \return The value wanted, or NULL if the value doesn't exist or an error
 * occured
*/
void *get_data(struct hash_table *ht, char *key);

/**
 * \brief Delete an element of the hash_table
 * \param ht The hash_table where the element is stored
 * \param key The key of the element to delete
 * \return True if the removed worked, false in the other cases
*/
int del_hash(struct hash_table *ht, char *key);

/**
 * \brief Copy a hash table
 * \param ht The hash table to copy
 * \return The copide hash table, or NULL if an error occured
*/
struct hash_table *cpy_hash(struct hash_table *ht);

#endif
