#include "unity.h"
#include "list.h"
#include <stdlib.h>


static void fill_list(List_T *list, void *src, size_t size, size_t length) {
    size_t i;

    if (!list || !src || length == 0) {
        return;
    }
    for (i = 0; i < length; i++) {
        list_push_back(list, src + (i * size));
    }
}

static void iter_func(void *data)
{
    (*((int *)data)) *= 2;
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_list_get_length(void)
{
    List_T *list;
    int data;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL(0, list_get_length(list));

    data = 42;
    list_push_back(list, &data);
    TEST_ASSERT_EQUAL(1, list_get_length(list));

    list_push_front(list, &data);
    TEST_ASSERT_EQUAL(2, list_get_length(list));
    list_remove_back(list, NULL);
    list_pop(list);
    list_pop(list);
    TEST_ASSERT_EQUAL(0, list_get_length(list));
    list_destroy(list, NULL);
}

void test_list_create(void)
{
    List_T *list;
    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL(0, list_get_length(list));
    list_destroy(list, NULL);
}

void test_list_destroy_null(void) {
    ListStatus_E status;
    status = list_destroy(NULL, NULL);
    TEST_ASSERT_EQUAL(LIST_STATUS_ERROR, status);
}

void test_list_destroy_empty(void) {
    List_T *list;
    ListStatus_E status;
    int arr[] = {1, 2, 3, 4};

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);

    fill_list(list, arr, sizeof(*arr), sizeof(arr)/sizeof(*arr));
    TEST_ASSERT_EQUAL(4, list_get_length(list));
    status = list_destroy(list, NULL);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, status);
}

void test_list_destroy_non_empty_no_callback(void) {
    List_T *list;
    ListStatus_E status;
    int *data1, *data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);

    data1 = malloc(sizeof(int));
    data2 = malloc(sizeof(int));
    *data1 = 1;
    *data2 = 2;
    list_push_back(list, data1);
    list_push_back(list, data2);

    status = list_destroy(list, NULL);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, status);

    free(data1);
    free(data2);
}

void test_list_destroy_non_empty_with_callback(void) {
    List_T *list;
    ListStatus_E status;
    int *data1, *data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);

    data1 = malloc(sizeof(int));
    data2 = malloc(sizeof(int));
    *data1 = 1;
    *data2 = 2;
    list_push_back(list, data1);
    list_push_back(list, data2);

    status = list_destroy(list, free);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, status);
}

void test_list_push_back_empty(void)
{
    ListStatus_E status;
    status = list_push_back(NULL, NULL);
    TEST_ASSERT_EQUAL(LIST_STATUS_ERROR, status);
}

void test_list_push_back_non_empty(void)
{
    List_T *list;
    ListStatus_E status;
    char arr[] = {'a', 'b', 'c', 'd', 'e'};
    size_t i;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    for (i = 0; i < sizeof(arr)/sizeof(*arr); i++) {
        status = list_push_back(list, arr + (i * sizeof(*arr)));
        TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, status);
    }
    list_destroy(list, NULL);
}

void test_list_push_front_empty(void)
{
    ListStatus_E status;
    status = list_push_front(NULL, NULL);
    TEST_ASSERT_EQUAL(LIST_STATUS_ERROR, status);
}

void test_list_push_front_non_empty(void)
{
    List_T *list;
    ListStatus_E status;
    char arr[] = {'a', 'b', 'c', 'd', 'e'};
    size_t i;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    for (i = 0; i < sizeof(arr)/sizeof(*arr); i++) {
        status = list_push_back(list, arr + (i * sizeof(*arr)));
        TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, status);
    }
    list_destroy(list, NULL);
}

void test_list_insert_empty_list(void) {
    List_T *list;
    int data;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data = 42;
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_insert(list, 0, &data));
    TEST_ASSERT_EQUAL(1, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data, list_get(list, 0));

    list_destroy(list, NULL);
}

void test_list_insert_beginning(void) {
    List_T *list;
    int data1, data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 1;
    data2 = 2;
    list_push_back(list, &data1);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_insert(list, 0, &data2));
    TEST_ASSERT_EQUAL(2, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data2, list_get(list, 0));
    TEST_ASSERT_EQUAL_PTR(&data1, list_get(list, 1));

    list_destroy(list, NULL);
}

void test_list_insert_end(void) {
    List_T *list;
    int data1, data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 1;
    data2 = 2;
    list_push_back(list, &data1);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_insert(list, 1, &data2));
    TEST_ASSERT_EQUAL(2, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data1, list_get(list, 0));
    TEST_ASSERT_EQUAL_PTR(&data2, list_get(list, 1));

    list_destroy(list, NULL);
}

void test_list_insert_middle(void) {
    List_T *list;
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    int data1, data2, data3;
    size_t size;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 1;
    data2 = 2;
    data3 = 2;
    size = sizeof(arr)/sizeof(*arr);
    fill_list(list, arr, sizeof(*arr), size);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_insert(list, 1, &data1));
    TEST_ASSERT_EQUAL(size + 1, list_get_length(list));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_insert(list, 4, &data2));
    TEST_ASSERT_EQUAL(size + 2, list_get_length(list));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_insert(list, 7, &data3));
    TEST_ASSERT_EQUAL(size + 3, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data1, list_get(list, 1));
    TEST_ASSERT_EQUAL_PTR(&data2, list_get(list, 4));
    TEST_ASSERT_EQUAL_PTR(&data3, list_get(list, 7));
    TEST_ASSERT_EQUAL(data1, *((int *)list_get(list, 1)));
    TEST_ASSERT_EQUAL(data2, *((int *)list_get(list, 4)));
    TEST_ASSERT_EQUAL(data3, *((int *)list_get(list, 7)));

    list_destroy(list, NULL);
}

void test_list_insert_invalid_index(void) {
    List_T *list;
    int data1, data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 1;
    data2 = 2;
    list_push_front(list, &data1);
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_insert(list, 2, &data2));
    TEST_ASSERT_EQUAL(1, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data1, list_get(list, 0));

    list_destroy(list, NULL);
}

void test_list_insert_null_list(void) {
    int data;
    
    data = 1;
    TEST_ASSERT_EQUAL(LIST_STATUS_ERROR, list_insert(NULL, 0, &data));
}


void test_list_remove_back_empty_list(void) {
    List_T *list;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_remove_back(list, NULL));
    list_destroy(list, NULL);
}

void test_list_remove_back_single_element(void) {
    List_T *list;
    int data;

    list = list_create();
    data = 42;
    TEST_ASSERT_NOT_NULL(list);
    list_push_back(list, &data);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_remove_back(list, NULL));
    TEST_ASSERT_EQUAL(0, list_get_length(list));
    list_destroy(list, NULL);
}

void test_list_remove_back_multiple_elements(void) {
    List_T *list;
    int data1, data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_remove_back(list, NULL));
    TEST_ASSERT_EQUAL(1, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data1, list_get_back(list));
    list_destroy(list, NULL);
}

void test_list_remove_front_empty_list(void) {
    List_T *list;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_remove_front(list, NULL));
    list_destroy(list, NULL);
}

void test_list_remove_front_single_element(void) {
    List_T *list;
    int data;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data = 42;
    list_push_front(list, &data);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_remove_front(list, NULL));
    TEST_ASSERT_EQUAL(0, list_get_length(list));
    list_destroy(list, NULL);
}

void test_list_remove_front_multiple_elements(void) {
    List_T *list;
    int data1, data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    list_push_front(list, &data1);
    list_push_front(list, &data2);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_remove_front(list, NULL));
    TEST_ASSERT_EQUAL(1, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data1, list_get_front(list));
    list_destroy(list, NULL);
}

void test_list_remove_empty_list(void) {
    List_T *list;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_remove(list, 0, NULL));
    list_destroy(list, NULL);
}

void test_list_remove_single_element(void) {
    List_T *list;
    int data;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data = 42;
    list_push_back(list, &data);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_remove(list, 0, NULL));
    TEST_ASSERT_EQUAL(0, list_get_length(list));
    list_destroy(list, NULL);
}

void test_list_remove_first_element_multiple_elements(void) {
    List_T *list;
    int data1, data2, data3;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    data3 = 66;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    list_push_back(list, &data3);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_remove(list, 0, NULL));
    TEST_ASSERT_EQUAL(2, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data2, list_get(list, 0));
    list_destroy(list, NULL);
}

void test_list_remove_last_element_multiple_elements(void) {
    List_T *list;
    int data1, data2, data3;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    data3 = 66;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    list_push_back(list, &data3);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_remove(list, 2, NULL));
    TEST_ASSERT_EQUAL(2, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data2, list_get(list, 1));
    list_destroy(list, NULL);
}

void test_list_remove_middle_element(void) {
    List_T *list;
    int data1, data2, data3;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    data3 = 66;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    list_push_back(list, &data3);
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_remove(list, 1, NULL));
    TEST_ASSERT_EQUAL(2, list_get_length(list));
    TEST_ASSERT_EQUAL_PTR(&data1, list_get(list, 0));
    TEST_ASSERT_EQUAL_PTR(&data3, list_get(list, 1));
    list_destroy(list, NULL);
}

void test_list_remove_invalid_index(void) {
    List_T *list;
    int data1, data2, data3;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    data3 = 66;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    list_push_back(list, &data3);
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_remove(list, 3, NULL));
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_remove(list, -1, NULL));
    list_destroy(list, NULL);
}

void test_list_get_back_empty_list(void) {
    List_T *list;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NULL(list_get_back(list));
    list_destroy(list, NULL);
}

void test_list_get_back_single_element(void) {
    List_T *list;
    int data;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data = 42;
    list_push_back(list, &data);
    TEST_ASSERT_EQUAL_PTR(&data, list_get_back(list));
    list_destroy(list, NULL);
}

void test_list_get_back_multiple_elements(void) {
    List_T *list;
    int data1, data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    TEST_ASSERT_EQUAL_PTR(&data2, list_get_back(list));

    list_destroy(list, NULL);
}

void test_list_get_front_empty_list(void) {
    List_T *list;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NULL(list_get_front(list));
    list_destroy(list, NULL);
}

void test_list_get_front_single_element(void) {
    List_T *list;
    int data;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data = 42;
    list_push_front(list, &data);
    TEST_ASSERT_EQUAL_PTR(&data, list_get_front(list));
    list_destroy(list, NULL);
}

void test_list_get_front_multiple_elements(void) {
    List_T *list;
    int data1, data2;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    list_push_front(list, &data1);
    list_push_front(list, &data2);
    TEST_ASSERT_EQUAL_PTR(&data2, list_get_front(list));
    list_destroy(list, NULL);
}

void test_list_get_empty_list(void) {
    List_T *list;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NULL(list_get(list, 0));
    list_destroy(list, NULL);
}

void test_list_get_single_element(void) {
    List_T *list;
    int data;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data = 42;
    list_push_back(list, &data);
    TEST_ASSERT_EQUAL_PTR(&data, list_get(list, 0));
    list_destroy(list, NULL);
}

void test_list_get_first_element_multiple_elements(void) {
    List_T *list;
    int data1, data2, data3;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    data3 = 66;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    list_push_back(list, &data3);
    TEST_ASSERT_EQUAL_PTR(&data1, list_get(list, 0));
    list_destroy(list, NULL);
}

void test_list_get_last_element_multiple_elements(void) {
    List_T *list;
    int data1, data2, data3;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    data3 = 66;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    list_push_back(list, &data3);
    TEST_ASSERT_EQUAL_PTR(&data3, list_get(list, 2));
    list_destroy(list, NULL);
}

void test_list_get_middle_element(void) {
    List_T *list;
    int data1, data2, data3;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    data3 = 66;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    list_push_back(list, &data3);
    TEST_ASSERT_EQUAL_PTR(&data2, list_get(list, 1));
    list_destroy(list, NULL);
}

void test_list_get_invalid_index(void) {
    List_T *list;
    int data1, data2, data3;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data1 = 42;
    data2 = 24;
    data3 = 66;
    list_push_back(list, &data1);
    list_push_back(list, &data2);
    list_push_back(list, &data3);
    TEST_ASSERT_NULL(list_get(list, 3));
    TEST_ASSERT_NULL(list_get(list, -1));
    list_destroy(list, NULL);
}

void test_list_pop(void) 
{
    List_T *list;
    int data;

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    data = 42;
    TEST_ASSERT_NULL(list_pop(NULL));
    list_push_back(list, &data);
    TEST_ASSERT_EQUAL_PTR(&data, list_pop(list));
    list_destroy(list, NULL);
}

void test_list_set_back(void)
{
    List_T *list;
    char ch;
    int arr[] = {1, 2, 3, 4};

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    ch = 'a';
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_set_back(list, &ch));
    fill_list(list, arr, sizeof(*arr), sizeof(arr)/sizeof(*arr));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_set_back(list, &ch));
    TEST_ASSERT_EQUAL(&ch, list_get_back(list));
    list_destroy(list, NULL);
}

void test_list_set_front(void)
{
    List_T *list;
    char ch;
    int arr[] = {1, 2, 3, 4};

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    ch = 'a';
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_set_front(list, &ch));
    fill_list(list, arr, sizeof(*arr), sizeof(arr)/sizeof(*arr));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_set_front(list, &ch));
    TEST_ASSERT_EQUAL(&ch, list_get_front(list));
    list_destroy(list, NULL);
}

void test_list_set(void)
{
    List_T *list;
    char ch1, ch2;
    int arr[] = {1, 2, 3, 4};

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    ch1 = 'a';
    ch2 = 'b';
    TEST_ASSERT_EQUAL(LIST_STATUS_ERROR, list_set(NULL, 0, &ch1));
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_set(list, 0, &ch1));
    fill_list(list, arr, sizeof(*arr), sizeof(arr)/sizeof(*arr));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_set(list, 0, NULL));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_set(list, 1, &ch1));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_set(list, 3, &ch2));
    TEST_ASSERT_EQUAL(LIST_STATUS_INDEX_ERROR, list_set(list, -2, NULL));
    TEST_ASSERT_EQUAL_PTR(NULL, list_get(list, 0));
    TEST_ASSERT_EQUAL_PTR(&ch1, list_get(list, 1));
    TEST_ASSERT_EQUAL_PTR(&ch2, list_get(list, 3));
    list_destroy(list, NULL);
}

void test_list_iter(void)
{
    List_T *list;
    int arr[] = {1, 2, 3, 4};

    list = list_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL(LIST_STATUS_ERROR, list_iter(NULL, iter_func));
    TEST_ASSERT_EQUAL(LIST_STATUS_ERROR, list_iter(list, NULL));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_iter(list, iter_func));
    TEST_ASSERT_EQUAL(0, list_get_length(list));
    fill_list(list, arr, sizeof(*arr), sizeof(arr)/sizeof(*arr));
    TEST_ASSERT_EQUAL(LIST_STATUS_ERROR, list_iter(list, NULL));
    TEST_ASSERT_EQUAL(1, *((int *)list_get(list, 0)));
    TEST_ASSERT_EQUAL(2, *((int *)list_get(list, 1)));
    TEST_ASSERT_EQUAL(3, *((int *)list_get(list, 2)));
    TEST_ASSERT_EQUAL(4, *((int *)list_get(list, 3)));
    TEST_ASSERT_EQUAL(LIST_STATUS_SUCCESS, list_iter(list, iter_func));
    TEST_ASSERT_EQUAL(2, *((int *)list_get(list, 0)));
    TEST_ASSERT_EQUAL(4, *((int *)list_get(list, 1)));
    TEST_ASSERT_EQUAL(6, *((int *)list_get(list, 2)));
    TEST_ASSERT_EQUAL(8, *((int *)list_get(list, 3)));
    list_destroy(list, NULL);
}

void test_list_slice(void)
{
    List_T *list1, *list2;
    size_t size;
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};

    list1 = list_create();
    TEST_ASSERT_NOT_NULL(list1);
    size = sizeof(arr)/sizeof(*arr);
    fill_list(list1, arr, sizeof(*arr), size);
    TEST_ASSERT_EQUAL(size, list_get_length(list1));
    TEST_ASSERT_EQUAL_PTR(NULL, list_slice(NULL, 4, 9));
    TEST_ASSERT_EQUAL_PTR(NULL, list_slice(list1, -1, 2));
    TEST_ASSERT_EQUAL_PTR(NULL, list_slice(list1, 5, 3));
    list2 = list_slice(list1, 1, 4);
    TEST_ASSERT_EQUAL(3, list_get_length(list2));
    TEST_ASSERT_EQUAL_PTR(list_get(list1, 1), list_get(list2, 0));
    TEST_ASSERT_EQUAL_PTR(list_get(list1, 2), list_get(list2, 1));
    TEST_ASSERT_EQUAL_PTR(list_get(list1, 3), list_get(list2, 2));
    TEST_ASSERT_EQUAL_PTR(NULL, list_get(list2, 3));
    list_destroy(list1, NULL);
    list_destroy(list2, NULL);
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_list_get_length);
    RUN_TEST(test_list_create);
    RUN_TEST(test_list_destroy_null);
    RUN_TEST(test_list_destroy_empty);
    RUN_TEST(test_list_destroy_non_empty_no_callback);
    RUN_TEST(test_list_destroy_non_empty_with_callback);
    RUN_TEST(test_list_push_back_empty);
    RUN_TEST(test_list_push_back_non_empty);
    RUN_TEST(test_list_push_front_empty);
    RUN_TEST(test_list_push_front_non_empty);
    RUN_TEST(test_list_insert_empty_list);
    RUN_TEST(test_list_insert_beginning);
    RUN_TEST(test_list_insert_end);
    RUN_TEST(test_list_insert_middle);
    RUN_TEST(test_list_insert_invalid_index);
    RUN_TEST(test_list_insert_null_list);
    RUN_TEST(test_list_remove_back_empty_list);
    RUN_TEST(test_list_remove_back_single_element);
    RUN_TEST(test_list_remove_back_multiple_elements);
    RUN_TEST(test_list_remove_front_empty_list);
    RUN_TEST(test_list_remove_front_single_element);
    RUN_TEST(test_list_remove_front_multiple_elements);
    RUN_TEST(test_list_remove_empty_list);
    RUN_TEST(test_list_remove_single_element);
    RUN_TEST(test_list_remove_first_element_multiple_elements);
    RUN_TEST(test_list_remove_last_element_multiple_elements);
    RUN_TEST(test_list_remove_middle_element);
    RUN_TEST(test_list_remove_invalid_index);
    RUN_TEST(test_list_get_back_empty_list);
    RUN_TEST(test_list_get_back_single_element);
    RUN_TEST(test_list_get_back_multiple_elements);
    RUN_TEST(test_list_get_front_empty_list);
    RUN_TEST(test_list_get_front_single_element);
    RUN_TEST(test_list_get_front_multiple_elements);
    RUN_TEST(test_list_get_empty_list);
    RUN_TEST(test_list_get_single_element);
    RUN_TEST(test_list_get_first_element_multiple_elements);
    RUN_TEST(test_list_get_last_element_multiple_elements);
    RUN_TEST(test_list_get_middle_element);
    RUN_TEST(test_list_get_invalid_index);
    RUN_TEST(test_list_pop);
    RUN_TEST(test_list_set_back);
    RUN_TEST(test_list_set_front);
    RUN_TEST(test_list_set);
    RUN_TEST(test_list_iter);
    RUN_TEST(test_list_slice);

    return UNITY_END();
}

