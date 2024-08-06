#include <stdlib.h>
#include "list.h"


static ListNode_T *inner_list_node_create(void *data) 
{
    ListNode_T *new_node;

    new_node = malloc(sizeof(*new_node));
    if (!new_node) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

size_t list_get_length(List_T *list)
{
    return list ? list->length : 0;
}

List_T *list_create()
{
    List_T* new_list;

    new_list = malloc(sizeof(*new_list));
    if (!new_list) {
        return NULL;
    }
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->length = 0;
    return new_list;
}

ListStatus_E list_push_back(List_T *list, void *data)
{
    ListNode_T *new_node;

    if (!list) {
        return LIST_STATUS_ERROR;
    }
    new_node = inner_list_node_create(data);
    if (!new_node) {
        return LIST_STATUS_MEMORY_ERROR;
    }
    if (list->length == 0) {
        list->head = new_node;
        list->tail = new_node;
    }
    else{
        list->tail->next = new_node;
        new_node->prev = list->tail;
        list->tail = new_node;
    }
    list->length++;
    return LIST_STATUS_SUCCESS;
}

ListStatus_E list_push_front(List_T *list, void *data)
{
    ListNode_T *new_node;

    if (!list) {
        return LIST_STATUS_ERROR;
    }
    new_node = inner_list_node_create(data);
    if (!new_node) {
        return LIST_STATUS_MEMORY_ERROR;
    }
    if (list->length == 0) {
        list->head = new_node;
        list->tail = new_node;
    }
    else{
        list->head->prev = new_node;
        new_node->next = list->head;
        list->head = new_node;
    }
    list->length++;
    return LIST_STATUS_SUCCESS;
}

ListStatus_E list_insert(List_T *list, size_t index, void *data)
{
    ListNode_T *new_node, *tmp_node;

    if (!list) {
        return LIST_STATUS_ERROR;
    }
    if (index > list->length) {
        return LIST_STATUS_INDEX_ERROR;
    }
    if (index == 0) {
        return list_push_front(list, data);
    }
    if (index == list->length) {
        return list_push_back(list, data);
    }
    new_node = inner_list_node_create(data);
    if (!new_node) {
        return LIST_STATUS_MEMORY_ERROR;
    }
    if (index <= list->length / 2) {
        tmp_node = list->head;
        while (index > 0) {
            tmp_node = tmp_node->next;
            index--;
        }
    }
    else {
        tmp_node = list->tail;
        index = list->length - index - 1;
        while (index > 0) {
            tmp_node = tmp_node->prev;
            index--;
        }
    }
    new_node->next = tmp_node;
    new_node->prev = tmp_node->prev;
    tmp_node->prev->next = new_node;
    tmp_node->prev = new_node;
    list->length++;
    return LIST_STATUS_SUCCESS;
}

ListStatus_E list_remove_back(List_T *list, void (*func)(void *))
{
    ListNode_T *tmp_node;

    if (!list) {
        return LIST_STATUS_ERROR;
    }
    if (list->length == 0) {
        return LIST_STATUS_INDEX_ERROR;
    }
    tmp_node = list->tail;
    if (list->length == 1) {
        list->tail = NULL;
        list->head = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    if (func) {
        func(tmp_node->data);
    }
    free(tmp_node);
    list->length--;
    return LIST_STATUS_SUCCESS;
}

ListStatus_E list_remove_front(List_T *list, void (*func)(void *))
{
    ListNode_T *tmp_node;

    if (!list) {
        return LIST_STATUS_ERROR;
    }
    if (list->length == 0) {
        return LIST_STATUS_INDEX_ERROR;
    }
    tmp_node = list->head;
    if (list->length == 1) {
        list->tail = NULL;
        list->head = NULL;
    }
    else{
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    if (func) {
        func(tmp_node->data);
    }
    free(tmp_node);
    list->length--;
    return LIST_STATUS_SUCCESS;
}

ListStatus_E list_remove(List_T *list, size_t index, void (*func)(void *))
{
    ListNode_T *tmp_node;

    if (!list) {
        return LIST_STATUS_ERROR;
    }
    if (index >= list->length) {
        return LIST_STATUS_INDEX_ERROR;
    }
    if (index == 0) {
        return list_remove_front(list, func);
    }
    else if (index == list->length - 1) {
        return list_remove_back(list, func);
    }
    if (index <= list->length / 2) {
        tmp_node = list->head;
        while (index > 0) {
            tmp_node = tmp_node->next;
            index--;
        }
    }
    else {
        tmp_node = list->tail;
        index = list->length - index - 1;
        while (index > 0) {
            tmp_node = tmp_node->prev;
            index--;
        }
    }
    tmp_node->prev->next = tmp_node->next;
    tmp_node->next->prev = tmp_node->prev;
    if (func) {
        func(tmp_node->data);
    }
    free(tmp_node);
    list->length--;
    return LIST_STATUS_SUCCESS;
}

void *list_get_back(List_T *list)
{
    return (!list || !list->tail) ? NULL : list->tail->data;
}

void *list_get_front(List_T *list)
{
    return (!list || !list->head) ? NULL : list->head->data;
}

void *list_get(List_T *list, size_t index)
{
    ListNode_T *tmp_node;

    if (!list || index >= list->length) {
        return NULL;
    }
    if (index == 0) {
        return list_get_front(list);
    }
    else if (index == list->length - 1) {
        return list_get_back(list);
    }
    if (index <= list->length / 2) {
        tmp_node = list->head;
        while (index > 0) {
            tmp_node = tmp_node->next;
            index--;
        }
    }
    else {
        tmp_node = list->tail;
        index = list->length - index - 1;
        while (index > 0) {
            tmp_node = tmp_node->prev;
            index--;
        }
    }

    return tmp_node->data;
}

void *list_pop(List_T *list)
{
    void *data;
    if (!list || list->length == 0) {
        return NULL;
    }
    data = list_get_back(list);
    list_remove_back(list, NULL);
    return data;
}

ListStatus_E list_set_back(List_T *list, void *data)
{
    if (!list) {
        return LIST_STATUS_ERROR;
    }
    if (list->length == 0) {
        return LIST_STATUS_INDEX_ERROR;
    }
    list->tail->data = data;
    return LIST_STATUS_SUCCESS;
}

ListStatus_E list_set_front(List_T *list, void *data)
{
    if (!list) {
        return LIST_STATUS_ERROR;
    }
    if (list->length == 0) {
        return LIST_STATUS_INDEX_ERROR;
    }
    list->head->data = data;
    return LIST_STATUS_SUCCESS;
}

ListStatus_E list_set(List_T *list, size_t index, void *data)
{
    ListNode_T *tmp_node;

    if (!list) {
        return LIST_STATUS_ERROR;
    }
    if (index >= list->length) {
        return LIST_STATUS_INDEX_ERROR;
    }
    if (index == 0) {
        return list_set_front(list, data);
    }
    else if (index == list->length - 1) {
        return list_set_back(list, data);
    }
    tmp_node = list->head;
    while (index > 0) {
        tmp_node = tmp_node->next;
        index--;
    }
    tmp_node->data = data;
    return LIST_STATUS_SUCCESS;
}

ListStatus_E list_iter(List_T *list, void (*func)(void *))
{
    ListNode_T *tmp_node;

    if (!list || !func) {
        return LIST_STATUS_ERROR;
    }
    tmp_node = list->head;
    while (tmp_node) {
        func(tmp_node->data);
        tmp_node = tmp_node->next;
    }
    return LIST_STATUS_SUCCESS;
}


ListStatus_E list_destroy(List_T *list, void (*func)(void *))
{
    ListNode_T *tmp_node;

    if (!list) {
        return LIST_STATUS_ERROR;
    }
    while (list->head) {
        tmp_node = list->head;
		if (func) {
			func(tmp_node->data);
		}
        list->head = list->head->next;
        free(tmp_node);
    }
    free(list);
    return LIST_STATUS_SUCCESS;
}

List_T *list_slice(List_T *list, size_t start, size_t stop)
{
    List_T *new_list;
    void *data;
    size_t i;

    if (!list) {
        return NULL;
    }
    if (!(start < stop && stop < list->length)) {
        return NULL;
    }
    new_list = list_create(); 
    if (!new_list) {
        return NULL;
    }
    for (i = start; i < stop; i++) {
        data = list_get(list, i);
        if (list_push_back(new_list, data) != LIST_STATUS_SUCCESS) {
            list_destroy(new_list, NULL);
            return NULL;
        }
    }
    return new_list;
}



