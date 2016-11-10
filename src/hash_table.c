#include "hash_table.h"

static size_t hash_func(char *key, size_t capacity)
{
  size_t res = 5381;
  for (size_t i = 0; key[i]; i++)
  {
    size_t n = key[i];
    res = res * 33 + n;
  }

  return res % capacity;
}

struct hash_table *create_hash(size_t capacity)
{
  struct hash_table *new = malloc(sizeof (struct hash_table));
  if (!new)
    return NULL;

  new->nb_elt = 0;
  new->table = calloc(capacity, sizeof (struct elt_hash *));
  if (!new->table)
  {
    free(new);
    return NULL;
  }
  new->capacity = capacity;

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
        free(temp->key);
        free(temp->data);
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

static struct hash_table *rehash(struct hash_table *ht)
{
  struct elt_hash **temp =
                  calloc(2 * ht->capacity, sizeof (struct elt_hash *));

  size_t tmp_nb = ht->nb_elt;
  for (size_t i = 0; i < ht->capacity && tmp_nb > 0; i++)
  {
    struct elt_hash *tmp = ht->table[i];
    while (tmp)
    {
      struct elt_hash *tmp2 = tmp;
      tmp = tmp->next;
      size_t pos = hash_func(tmp->key, 2 * ht->capacity);
      tmp2->next = temp[pos];
      temp[pos] = tmp2;
      tmp_nb--;
    }
  }

  ht->capacity *= 2;
  free(ht->table);
  ht->table = temp;
  return ht;
}

struct hash_table *add_hash(struct hash_table *ht, char *key, void *data)
{
  if (ht->nb_elt >= ht->capacity)
    ht = rehash(ht);

  struct elt_hash *new = malloc(sizeof (struct elt_hash));
  if (!new)
    return NULL;
  new->next = NULL;
  new->key = key;
  new->data = data;

  size_t pos = hash_func(key, ht->capacity);
  if (ht->table[pos])
    new->next = ht->table[pos];
  ht->table[pos] = new;

  return ht;
}

void *get_hash(struct hash_table *ht, char *key)
{
  size_t pos = hash_func(key, ht->capacity);
  struct elt_hash *found = ht->table[pos];

  while (found && strcmp(key, found->key) != 0)
    found = found->next;

  if (found)
    return found->data;
  else
    return NULL;
}

int del_hash(struct hash_table *ht, char *key)
{
  size_t pos = hash_func(key, ht->capacity);
  struct elt_hash *found = ht->table[pos];

  if (!found)
    return 0;

  if (strcmp(key, found->key) == 0)
  {
    ht->table[pos] = ht->table[pos]->next;
    free(found->key);
    free(found->data);
    free(found);
    return 1;
  }
  else
  {
    while (strcmp(key, found->next->key) != 0)
      found = found->next;

    if (!found->next)
      return 0;

    struct elt_hash *temp = found->next;
    found->next = temp->next;
    free(temp->key);
    free(temp->data);
    free(temp);
    return 1;
  }
}
