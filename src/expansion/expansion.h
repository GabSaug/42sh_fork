#ifndef EXPANSION_H
# define EXPANSION_H

#include "hash_table.h"

struct vector* expand(char* s, int in_ari_exp, struct hash_table *ht[]);
struct vector* field_split(struct vector* v_input, struct hash_table *ht[]);

#endif /* !EXPANSION_H */
