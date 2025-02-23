#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define AU_STRING_IMPLEMENTATION
#include "au_string.h"

/*
#define LIST(T, name)                                                                                                  \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        T data;                                                                                                        \
        struct Node_##T *next;                                                                                         \
    } Node##name;                                                                                                      \
    typedef struct                                                                                                     \
    {                                                                                                                  \
        Node##name *head;                                                                                              \
        Node##name *tail;                                                                                              \
        size_t length;                                                                                                 \
        size_t capacity;                                                                                               \
    } List##name;
*/

// LIST(int, Int)

typedef struct Node
{
    int data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct List
{
    Node *head;
    Node *tail;
    size_t length;
} List;

List create_list()
{
    List l = {};
    return l;
}

bool push_list(List *l, int data)
{
    if (l == nullptr)
        return false;

    Node *n = malloc(sizeof(Node));
    if (!n)
        return false;

    n->data = data;
    n->next = nullptr;
    n->prev = nullptr;

    if (l->length == 0)
    {
        l->head = n;
        l->tail = n;
    }
    else
    {
        l->tail->next = n;
        n->prev = l->tail;
        l->tail = n;
    }

    l->length++;

    return true;
}

bool pop_list(List *l, int *out)
{
    if (!l || l->length == 0)
        return false;

    auto tail = l->tail;
    if (out)
    {
        *out = tail->data;
    }

    if (l->length == 1)
    {
        l->head = nullptr;
        l->tail = nullptr;
    }
    else
    {
        l->tail = tail->prev;
        l->tail->next = nullptr;
    }

    free(tail);
    l->length--;
    return true;
}

bool push_head_list(List *l, int data)
{
    if (l == nullptr)
        return false;

    Node *n = malloc(sizeof(Node));
    if (!n)
        return false;

    n->data = data;
    n->next = l->head;
    n->prev = nullptr;

    if (l->length == 0)
    {
        l->head = n;
        l->tail = n;
    }
    else
    {
        l->head->prev = n;
        l->head = n;
    }

    l->length++;

    return true;
}

bool pop_head_list(List *l, int *out)
{
    if (!l || l->length == 0)
        return false;

    auto head = l->head;
    if (out)
    {
        *out = head->data;
    }

    if (l->length == 1)
    {
        l->head = nullptr;
        l->tail = nullptr;
    }
    else
    {
        l->head = head->next;
        l->head->prev = nullptr;
    }

    free(head);
    l->length--;
    return true;
}

Node *get_at_list(List *l, size_t index)
{
    if (!l || index >= l->length)
        return nullptr;

    if (index < l->length / 2)
    {
        Node *current = l->head;
        for (size_t i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current;
    }
    else
    {
        Node *current = l->tail;
        for (size_t i = l->length - 1; i > index; i--)
        {
            current = current->prev;
        }
        return current;
    }
}

bool pop_at_list(List *l, int *out, size_t index)
{
    if (!l || l->length == 0 || index >= l->length)
        return false;
    if (index == (l->length - 1))
        return pop_list(l, out);
    if (index == 0)
        return pop_head_list(l, out);

    Node *n = get_at_list(l, index);

    if (!n)
        return false;

    if (out)
        *out = n->data;

    auto prev = n->prev;
    auto next = n->next;

    prev->next = next;
    next->prev = prev;

    free(n);

    l->length--;

    return true;
}

bool push_at_list(List *l, int data, size_t index)
{
    if (!l || index > l->length)
        return false;

    if (index == 0)
        return push_head_list(l, data);

    if (index == l->length)
        return push_list(l, data);

    Node *current = get_at_list(l, index);

    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
        return false;

    new_node->data = data;
    new_node->next = current;
    new_node->prev = current->prev;

    current->prev->next = new_node;
    current->prev = new_node;

    l->length++;
    return true;
}

int main()
{
    return 0;
}
