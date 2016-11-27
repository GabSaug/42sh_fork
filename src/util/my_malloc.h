#ifndef MY_MALLOC_H
# define MY_MALLOC_H

#include <stddef.h>

/**
 * \brief Allocation of memory with error gestion
 * \param size The size in bytes of memory to allocate
 * \return The allocated pointer
*/
void *my_malloc(size_t size);

#endif /* !MY_MALLOC_H */
