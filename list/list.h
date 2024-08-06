#ifndef NODES_H
#define NODES_H

#include <stddef.h>

enum list_status_e {
    LIST_STATUS_SUCCESS = 0,
    LIST_STATUS_ERROR = -1,
    LIST_STATUS_MEMORY_ERROR = -2,
    LIST_STATUS_INDEX_ERROR = -3
};

struct list_node_t {
    void *data;
    struct list_node_t *next;
    struct list_node_t *prev;
};

struct list_t {
    size_t length;
    struct list_node_t *head;
    struct list_node_t *tail;
};

typedef enum list_status_e ListStatus_E;
typedef struct list_node_t ListNode_T;
typedef struct list_t List_T;


size_t list_get_length(List_T *list);

List_T *list_create();

ListStatus_E list_destroy(List_T *list, void (*func)(void *));

ListStatus_E list_push_back(List_T *list, void *data);

ListStatus_E list_push_front(List_T *list, void *data);

ListStatus_E list_insert(List_T *list, size_t index, void *data);

ListStatus_E list_remove_back(List_T *list, void (*func)(void *));

ListStatus_E list_remove_front(List_T *list, void (*func)(void *));

ListStatus_E list_remove(List_T *list, size_t index, void (*func)(void *));

void *list_get_back(List_T *list);

void *list_get_front(List_T *list);

void *list_get(List_T *list, size_t index);

void *list_pop(List_T *list);

ListStatus_E list_set_back(List_T *list, void *data);

ListStatus_E list_set_front(List_T *list, void *data);

ListStatus_E list_set(List_T *list, size_t index, void *data);

ListStatus_E list_iter(List_T *list, void (*func)(void *));

List_T *list_slice(List_T *list, size_t start, size_t end);

#endif
