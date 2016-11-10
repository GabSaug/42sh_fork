#ifndef HASH_TABLE_H
# define HASH_TABLE_H

#include <stdlib.h>

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

struct hash_table *create_hash(size_t capacity);
void destroy_hash(struct hash_table *ht);
struct hash_table *add_hash(struct hash_table *ht, char *key, void *data);
void *get_hash(struct hash_table *ht, char *key);

#endif
