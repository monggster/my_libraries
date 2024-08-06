#include "list.h"
#include <stdio.h>

static void *print_char(void *data)
{
    printf("%c ", *((char *)data));
    return data;
}

static void print_list(List_T *list)
{
    list_iter(list, print_char);
    printf("\n");
}

static void fill_list(List_T *list, void *src, size_t size, size_t length) {
    size_t i;

    if (!list || !src || size == 0 || length == 0) {
        return;
    }
    for (i = 0; i < length; i++) {
        list_push_back(list, src + (i * size));
    }
}

int main(void)
{
    List_T *list;
    char chars[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    char ints[] = {'1', '2', '3', '4', '5'};

    list = list_create();
    fill_list(list, chars, sizeof(*chars), sizeof(chars)/sizeof(*chars));
    print_list(list);
    list_insert(list, 5, ints);
    print_list(list);
    list_destroy(list, NULL);
    return 0;
}
