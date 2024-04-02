#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef MALLOC
#define MALLOC malloc
#endif
#ifndef FREE
#define FREE free
#endif

#define VECTOR(className, T)                                                                       \
    typedef T className##_value_type;                                                              \
    typedef T *className##_iterator;                                                               \
    typedef struct className {                                                                     \
        size_t capacity;                                                                           \
        className##_value_type *_first, *_last, *content;                                          \
    } className;                                                                                   \
    static className *className##_create();                                                        \
    static void className##_push_back(className *this, className##_value_type val);                \
    static void className##_destroy(className *this);                                              \
    static inline size_t className##_size(className *this);                                        \
    static void className##_shrink_to_fit(className *this);                                        \
    static bool className##_empty(className *this);                                                \
    static inline className##_iterator className##_begin(className *this) { return this->_first; } \
    static inline className##_iterator className##_end(className *this) { return this->_last; }    \
    static className##_value_type *className##_data(className *this);                              \
    static className##_iterator className##_at(className *this, size_t pos);                       \
    static className##_iterator className##_insert_array(                                          \
        className *this, className##_iterator pos, className##_value_type *, size_t _size);        \
    static inline void className##_reserve(className *this, size_t new_cap);                       \
    static className *className##_create() {                                                       \
        className *this = (className *)MALLOC(sizeof(className));                                  \
        this->capacity = 0;                                                                        \
        this->_first = this->_last = this->content = NULL;                                         \
        return this;                                                                               \
    }                                                                                              \
    static void className##_push_back(className *this, className##_value_type val) {               \
        if (className##_size(this) == this->capacity)                                              \
            className##_reserve(this, this->capacity ? this->capacity * 2 : 1);                    \
        *this->_last++ = val;                                                                      \
    }                                                                                              \
    static void className##_destroy(className *this) {                                             \
        FREE(this->content);                                                                       \
        FREE(this);                                                                                \
    }                                                                                              \
    static inline size_t className##_size(className *this) { return this->_last - this->_first; }  \
    static void className##_shrink_to_fit(className *this) {                                       \
        if (className##_size(this) < this->capacity) {                                             \
            className##_value_type *newContent = (className##_value_type *)MALLOC(                 \
                sizeof(className##_value_type) * className##_size(this));                          \
            memcpy(newContent, this->content,                                                      \
                   className##_size(this) * sizeof(className##_value_type));                       \
            FREE(this->content);                                                                   \
            this->capacity = className##_size(this);                                               \
            this->content = newContent;                                                            \
            this->_first = this->content;                                                          \
            this->_last = this->_first + this->capacity / 2;                                       \
        }                                                                                          \
    }                                                                                              \
    static bool className##_empty(className *this) { return className##_size(this) == 0; }         \
    static className##_value_type *className##_data(className *this) { return this->content; }     \
    static className##_iterator className##_at(className *this, size_t pos) {                      \
        if (pos > className##_size(this))                                                          \
            return NULL;                                                                           \
        else                                                                                       \
            return this->content + pos;                                                            \
    }                                                                                              \
    static className##_iterator className##_insert_array(                                          \
        className *this, className##_iterator pos, className##_value_type *arr, size_t count) {    \
        if (pos < className##_begin(this) || pos > className##_end(this))                          \
            return pos;                                                                            \
        size_t newSize = className##_size(this) + count;                                           \
        size_t pos_index = pos - className##_begin(this);                                          \
        className##_reserve(this, newSize);                                                        \
        pos = className##_begin(this) + pos_index;                                                 \
        memmove(pos + count, pos, (className##_end(this) - pos) * sizeof(className##_value_type)); \
        memmove(pos, arr, count * sizeof(className##_value_type));                                 \
        this->_last = this->_first + newSize;                                                      \
        return pos;                                                                                \
    }                                                                                              \
    static inline void className##_reserve(className *this, size_t new_cap) {                      \
        if (new_cap > this->capacity) {                                                            \
            className##_value_type *newContent =                                                   \
                (className##_value_type *)MALLOC(sizeof(className##_value_type) * new_cap);        \
            memcpy(newContent, this->content, this->capacity * sizeof(className##_value_type));    \
            FREE(this->content);                                                                   \
            this->content = newContent;                                                            \
            this->_first = this->content;                                                          \
            this->_last = this->_first + this->capacity;                                           \
            this->capacity = new_cap;                                                              \
        }                                                                                          \
    }                                                                                              \
    static inline className##_iterator className##_find(className *this,                           \
                                                        bool (*_Deter)(className##_iterator)) {    \
        className##_iterator iter = className##_begin(this);                                       \
        for (; iter != className##_end(this); iter++)                                              \
            if (_Deter(iter))                                                                      \
                return iter;                                                                       \
        return NULL;                                                                               \
    }
