#ifndef VECTOR_H
# define VECTOR_H

# define MIN_CAPACITY 16

#include <stddef.h>

/**
 * \file vector.h
*/

struct vector
{
  size_t size;
  size_t capacity;
  void** data;
};

/**
 * \brief Display the vector
 * \param v The vector to display
*/
void v_print(struct vector* v);


/**
 * \brief Create a new vector
 * \return The created vector, or NULL if an error happend
*/
struct vector* v_create(void);

/**
 * \brief Get the size of a vector
 * \param v The vector to get the size
 * \return The size of the vector
*/
size_t v_size(struct vector* v);

/**
 * \brief Check if the vector is empty or not
 * \param v The vector to verify
 * \return Return 1 if the vector is empty, 0 otherwise
*/
int v_is_empty(struct vector* v);

/**
 * \brief Add an element at the end of the vector
 * \param v The vector where the data will be added
 * \param elmt The data to add in the vector
 * \return The position of the new element
*/
size_t v_append(struct vector* v, void* elmt);

/**
 * \brief Get an element from a vector
 * \param v The vector to get a value
 * \param i The index of the value to get
 * \return The element at the position i
*/
void* v_get(struct vector* v, size_t i);

/**
 * \brief Change the value of an element
 * \param v The vector where the value will be changed
 * \param i The position of the element which will be changed
 * \param elmt The new value to set
*/
void v_set(struct vector* v, size_t i, void* elmt);

/**
 * \brief Concatenate two vector
 * \param v1 The vector destination, where the other vector will be add at the
 * end
 * \param v2 The vector to add at the end of the other
*/
void v_concat(struct vector* v1, struct vector* v2);

/**
 * \brief Remove the last element of a vector
 * \param v The vector to remove the last element
 * \return The data of the last element
*/
void* v_remove_last(struct vector* v);

/**
 * \brief Empty the vector
 * \param v The vector to clean
*/
void v_erase(struct vector *v);

/**
 * \brief Destroy totally the vector
 * \param v The vector to destroy
 * \param free_elt The function to free an element of the vector
*/
void v_destroy(struct vector* v, void (*free_elt) (void*));

#endif /* !VECTOR_H */
