#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Buffer structure
typedef struct {
    char *data;
    size_t size;
} buffer;

// Initializes a buffer
buffer buffer_init(void);

// Frees a buffer
void buffer_free(buffer *buffer);

// Adds data of size data_size to a buffer
void buffer_add(buffer *buffer, const char *data, size_t data_size);

// Checks if a buffer is empty
int buffer_is_empty(buffer *buffer);

// Finds data of size data_size in a buffer and returns its position
int buffer_find(buffer *buffer, const char *data, size_t data_size);

// Finds data of size data_size in a buffer in a
// case-insensitive fashion and returns its position
int buffer_find_insensitive(buffer *buffer, const char *data, size_t data_size);

#endif // BUFFER_HPP
