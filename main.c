#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define AU_STRING_IMPLEMENTATION
#include "au_string.h"
#include <stdio.h>
#include <time.h>

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


typedef struct timespec timespec;

timespec get_time() {
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t;
}

double get_elapsed_time(timespec start, timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
}

// Performance test functions
void test_push_head(size_t n) {
    List list = create_list();
    timespec start = get_time();
    for (size_t i = 0; i < n; ++i) {
        push_head_list(&list, (int)i);
    }
    timespec end = get_time();
    double elapsed = get_elapsed_time(start, end);
    printf("Push head %8zu elements: %8.3f ms (%6.3f ns/op)\n",
           n, elapsed * 1000, elapsed * 1e9 / n);

    int dummy;
    while (pop_head_list(&list, &dummy)) {}
}

void test_push_tail(size_t n) {
    List list = create_list();
    timespec start = get_time();
    for (size_t i = 0; i < n; ++i) {
        push_list(&list, (int)i);
    }
    timespec end = get_time();
    double elapsed = get_elapsed_time(start, end);
    printf("Push tail %8zu elements: %8.3f ms (%6.3f ns/op)\n",
           n, elapsed * 1000, elapsed * 1e9 / n);

    int dummy;
    while (pop_list(&list, &dummy)) {}
}

void test_pop_head(size_t n) {
    List list = create_list();
    for (size_t i = 0; i < n; ++i) {
        push_head_list(&list, (int)i);
    }

    timespec start = get_time();
    int dummy;
    for (size_t i = 0; i < n; ++i) {
        pop_head_list(&list, &dummy);
    }
    timespec end = get_time();
    double elapsed = get_elapsed_time(start, end);
    printf("Pop head  %8zu elements: %8.3f ms (%6.3f ns/op)\n",
           n, elapsed * 1000, elapsed * 1e9 / n);
}

void test_pop_tail(size_t n) {
    List list = create_list();
    for (size_t i = 0; i < n; ++i) {
        push_list(&list, (int)i);
    }

    timespec start = get_time();
    int dummy;
    for (size_t i = 0; i < n; ++i) {
        pop_list(&list, &dummy);
    }
    timespec end = get_time();
    double elapsed = get_elapsed_time(start, end);
    printf("Pop tail  %8zu elements: %8.3f ms (%6.3f ns/op)\n",
           n, elapsed * 1000, elapsed * 1e9 / n);
}

void test_insert_middle(size_t n) {
    List list = create_list();
    timespec start = get_time();
    for (size_t i = 0; i < n; ++i) {
        size_t index = list.length / 2;
        push_at_list(&list, (int)i, index);
    }
    timespec end = get_time();
    double elapsed = get_elapsed_time(start, end);
    printf("Insert mid%8zu elements: %8.3f ms (%6.3f ns/op)\n",
           n, elapsed * 1000, elapsed * 1e9 / n);

    int dummy;
    while (pop_head_list(&list, &dummy)) {}
}

void test_pop_middle(size_t n) {
    List list = create_list();
    for (size_t i = 0; i < n; ++i) {
        push_list(&list, (int)i);
    }

    timespec start = get_time();
    int dummy;
    for (size_t i = 0; i < n; ++i) {
        size_t index = list.length / 2;
        pop_at_list(&list, &dummy, index);
    }
    timespec end = get_time();
    double elapsed = get_elapsed_time(start, end);
    printf("Pop mid   %8zu elements: %8.3f ms (%6.3f ns/op)\n",
           n, elapsed * 1000, elapsed * 1e9 / n);
}

void test_access_pattern(size_t n, size_t accesses, const char* desc) {
    List list = create_list();
    for (size_t i = 0; i < n; ++i) {
        push_list(&list, (int)i);
    }

    timespec start = get_time();
    for (size_t a = 0; a < accesses; ++a) {
        // Access pattern: 0, n-1, n/2, n/4, 3n/4
        Node* n1 = get_at_list(&list, 0);
        Node* n2 = get_at_list(&list, n-1);
        Node* n3 = get_at_list(&list, n/2);
        Node* n4 = get_at_list(&list, n/4);
        Node* n5 = get_at_list(&list, 3*n/4);
        
        // Prevent optimization
        volatile int d1 = n1->data;
        volatile int d2 = n2->data;
        volatile int d3 = n3->data;
        volatile int d4 = n4->data;
        volatile int d5 = n5->data;

        (void)d1;
        (void)d2;
        (void)d3;
        (void)d4;
        (void)d5;
    }
    timespec end = get_time();
    double elapsed = get_elapsed_time(start, end);
    printf("Access %-14s (size %zu): %6.3f ns/access\n",
           desc, n, elapsed * 1e9 / (accesses * 5));

    int dummy;
    while (pop_list(&list, &dummy)) {}
}

int main() {
    printf("=== Push/Pop Performance ===\n");
    test_push_head(1000000);
    test_push_tail(1000000);
    test_pop_head(1000000);
    test_pop_tail(1000000);

    printf("\n=== Middle Operations (Quadratic Complexity) ===\n");
    test_insert_middle(10000);
    test_pop_middle(10000);

    printf("\n=== Random Access Performance ===\n");
    test_access_pattern(100000, 10000, "mixed");
    test_access_pattern(1000000, 10000, "mixed");

    return 0;
}
