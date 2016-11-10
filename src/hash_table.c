#include "hash_table.h"

struct hash_table *create_hash(size_t capacity)
{
  struct hash_table *new = malloc(sizeof (struct hash_table));
  if (!new)
    return NULL;

  new->nb_elt = 0;
  new->table = malloc(capacity * sizeof (struct elt_hash *));
  if (!new->table)
  {
    free(new);
    return NULL;
  }
  new->capacity = capacity;

  for (size_t i = 0; i < new->capacity; i++)
  {
    new->table[i] = NULL;
  }

  return new;
}

void destroy_hash(struct hash_table *ht)
{
  for (size_t i = 0; i < ht->capacity && ht->nb_elt > 0; i++)
  {
    if (ht->table[i])
    {
      while (ht->table[i]->next)
      {
        struct elt_hash *temp = ht->table[i]->next;
        ht->table[i] = ht->table[i]->next;
        free(temp);
        temp = NULL;
        ht->nb_elt -= 1;
      }
      free(ht->table[i]);
      ht->table[i] = NULL;
      ht->nb_elt -= 1;
    }
  }
}

struct hash_table *add_hash(struct hash_table *ht, char *key, void *data)
{
  if (/* TODO compare nb_elt with capacity */)
    // TODO expand hash table

  struct elt_hash *new = malloc(sizeof (struct elt_hash));
  if (!new)
    return NULL;
  new->next = NULL;
  new->key = key;
  new->data = data;

  size_t pos = ;// TODO hash function
  if (ht->table[pos])
    new->next = ht->table[pos];
  ht->table[pos] = new;

  return ht;
}

void *get_hash(struct hash_table *ht, char *key)
{
  size_t pos = ;// TODO hash function
  struct elt_hash found = ht->table[pos];

  while (found && strcmp(key, found->key) != 0)
    found = found->next;

  if (found)
    return found->data;
  else
    return NULL;
}
