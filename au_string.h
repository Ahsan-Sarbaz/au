/*

Author: Ahsan Ullah
Date: 23-Feb-2025

Notice: This is to be used with C 23. Make sure to have -Wall -Wextra -Wpedantic -Werror
        since we assume that the inputs types are always correct.

TODO:
    - memmem is only POSIX, need to find/implement a cross-platform alternative


*/

#ifndef AU_STRING_H
#define AU_STRING_H

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    char* data;
    size_t length;
} string;

string create_string(char* data, size_t length);
void free_string(string* s);
bool insert_string(string* dst, size_t index, const string src);
bool append_string(string* dst, const string src);
bool prepend_string(string* dst, const string src);
bool remove_string(string* dst, size_t index, size_t length);
bool compare_string(string a, string b);

#endif

#ifdef AU_STRING_IMPLEMENTATION
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

[[nodiscard]] string create_string(char* data, size_t length)
{
    string s = {};
    if (data)
    {
        s.length = length <= 0 ? strlen(data) : length;
        s.data = malloc(s.length + 1);
        if (s.data)
        {
            memcpy(s.data, data, s.length);
            s.data[s.length] = '\0';
        }
    }
    return s;
}

void free_string(string* s)
{
    if (s && s->data)
    {
        free(s->data);
        s->data = NULL;
        s->length = 0;
    }
}

[[nodiscard]] bool compare_string(string a, string b)
{
    if (a.length != b.length) return false;
    if (a.data == b.data) return true;
    return memcmp(a.data, b.data, a.length) == 0;
}

[[nodiscard]] bool has_substring(string haystack, string needle)
{
    if (haystack.length < needle.length) return false;
    if (haystack.length == needle.length) return memcmp(haystack.data, needle.data, needle.length) == 0;

    return memmem(haystack.data, haystack.length, needle.data, needle.length) != NULL;
}

[[nodiscard]] bool has_prefix(string str, string prefix)
{
    if (str.length < prefix.length) return false;
    return memcmp(str.data, prefix.data, prefix.length) == 0;
}

[[nodiscard]] bool has_suffix(string str, string suffix)
{
    if (str.length < suffix.length) return false;
    return memcmp(str.data + str.length - suffix.length, suffix.data, suffix.length) == 0;
}

[[nodiscard]] bool trim_prefix(string* str, string prefix)
{
    if (!has_prefix(*str, prefix)) return false;

    memmove(str->data, str->data + prefix.length, str->length - prefix.length);
    char* new_data = realloc(str->data, (str->length - prefix.length) + 1);
    if (!new_data) return false;
    str->data = new_data;
    str->length -= prefix.length;
    str->data[str->length] = '\0';
    return true;
}

[[nodiscard]] bool trim_suffix(string* str, string suffix)
{
    if (!has_suffix(*str, suffix)) return false;

    char* new_data = realloc(str->data, (str->length - suffix.length) + 1);
    if (!new_data) return false;
    str->data = new_data;
    str->length -= suffix.length;
    str->data[str->length] = '\0';
    return true;
}

[[nodiscard]] bool insert_string(string* dst, size_t index, const string src)
{
    if (index > dst->length) return false;

    bool overlap = (src.data >= dst->data) && (src.data < dst->data + dst->length);
    char* temp_src = NULL;
    if (overlap)
    {
        temp_src = malloc(src.length);
        if (!temp_src) return false;
        memcpy(temp_src, src.data, src.length);
    }

    char* new_data = realloc(dst->data, dst->length + src.length + 1);
    if (!new_data)
    {
        if (temp_src) free(temp_src);
        return false;
    }

    dst->data = new_data;

    const char* src_source = temp_src ? temp_src : src.data;

    if (index == 0)
        memmove(new_data + src.length, new_data, dst->length);
    else if (index != dst->length)
        memmove(new_data + index + src.length, new_data + index, dst->length - index);

    memcpy(new_data + index, src_source, src.length);
    dst->length += src.length;
    new_data[dst->length] = '\0';

    if (temp_src) free(temp_src);
    return true;
}

[[nodiscard]] bool append_string(string* dst, const string src)
{
    if (!dst || !src.data) return false;

    if (!dst->data)
    {
        *dst = create_string(src.data, src.length);
        return (dst->data != NULL);
    }

    return insert_string(dst, dst->length, src);
}

[[nodiscard]] bool prepend_string(string* dst, const string src)
{
    if (!dst || !src.data) return false;

    if (!dst->data)
    {
        *dst = create_string(src.data, src.length);
        return (dst->data != NULL);
    }

    return insert_string(dst, 0, src);
}

[[nodiscard]] bool remove_string(string* dst, size_t index, size_t length)
{
    if (!dst || !dst->data || dst->length == 0 || index > dst->length || length > dst->length - index) return false;

    const int new_length = dst->length - length;

    if (new_length == 0)
    {
        free_string(dst);
        return true;
    }

    if (index + length < dst->length)
    {
        char* src = dst->data + index + length;
        char* _dst = dst->data + index;
        memmove(_dst, src, dst->length - (index + length));
    }

    dst->length = new_length;
    dst->data[new_length] = '\0';

    char* new_data = realloc(dst->data, new_length + 1);
    if (new_data) dst->data = new_data;

    return true;
}

#endif // AU_STRING_IMPLEMENTATION
