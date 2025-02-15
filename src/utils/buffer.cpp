#include <stdexcept>

#include "buffer.hpp"

// Throws an error with the provided message
inline void error(const char *msg) {
    throw std::runtime_error(msg);
}

/**
 * Initializes a buffer.
 * @return An empty buffer.
 */
buffer buffer_init(void) {
    buffer buffer;
    buffer.data = NULL;
    buffer.size = 0;
    return buffer;
}

/**
 * Frees a buffer's allocated memory.
 * @param buffer The buffer to free.
 */
void buffer_free(buffer *buffer) {
    if (buffer->data != NULL) {
        free(buffer->data);
        buffer->data = NULL;
    }
    buffer->size = 0;
}

/**
 * Checks if a buffer is empty.
 * @param buffer The buffer to check.
 * @return 1 if empty, 0 otherwise.
 */
int buffer_is_empty(buffer *buffer) {
    return (buffer->data == NULL);
}

/**
 * Adds data to a buffer, resizing as needed.
 * @param buffer The buffer to add data to.
 * @param data The data to add.
 * @param data_size The size of the data.
 */
void buffer_add(buffer *buffer, const char *data, size_t data_size) {
    if (data == NULL || data_size == 0) return;

    if (buffer->data != NULL) {
        char *new_data = (char*)realloc(buffer->data, (buffer->size + data_size) * sizeof(char));
        if (new_data == NULL) {
            free(buffer->data);
            error("ERROR: Memory allocation failed");
        }
        buffer->data = new_data;
    } else {
        buffer->data = (char*)calloc(data_size, sizeof(char));
        if (buffer->data == NULL) {
            error("ERROR: Memory allocation failed");
        }
    }

    memcpy(buffer->data + buffer->size, data, data_size);
    buffer->size += data_size;
}

/**
 * Finds the first occurrence of `data` in the buffer.
 * @param buffer The buffer to search.
 * @param data The substring to find.
 * @param data_size The length of `data`.
 * @return The position of the match, or -1 if not found.
 */
int buffer_find(buffer *buffer, const char *data, size_t data_size) {
    if (buffer->data == NULL || data == NULL || data_size > buffer->size) return -1;

    size_t last_pos = buffer->size - data_size + 1;

    for (size_t i = 0; i < last_pos; ++i) {
        if (memcmp(buffer->data + i, data, data_size) == 0) {
            return i;
        }
    }

    return -1;
}

/**
 * Case-insensitive version of `buffer_find()`.
 * @param buffer The buffer to search.
 * @param data The substring to find.
 * @param data_size The length of `data`.
 * @return The position of the match, or -1 if not found.
 */
int buffer_find_insensitive(buffer *buffer, const char *data, size_t data_size) {
    if (buffer->data == NULL || data == NULL || data_size > buffer->size) return -1;

    size_t last_pos = buffer->size - data_size + 1;

    for (size_t i = 0; i < last_pos; ++i) {
        size_t j;
        for (j = 0; j < data_size; ++j) {
            if (tolower(buffer->data[i + j]) != tolower(data[j])) {
                break;
            }
        }
        if (j == data_size) {
            return i;
        }
    }

    return -1;
}
