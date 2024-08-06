#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hashmap.h"


static size_t inner_pearson_hash(const char *key) 
{
    size_t i, j, sum, len;
    unsigned char h;

    static const unsigned char T[256] = {
    98,  6, 85,150, 36, 23,112,164,135,207,169,  5, 26, 64,165,219,  /* 1*/
    61, 20, 68, 89,130, 63, 52,102, 24,229,132,245, 80,216,195,115,  /* 2*/
    90,168,156,203,177,120,  2,190,188,  7,100,185,174,243,162, 10,  /* 3*/
    123,251, 67,250,161,  0,107, 97,241,111,181, 82,249, 33, 69, 55, /* 4*/
    237, 18,253,225,  8,208,172,244,255,126,101, 79,145,235,228,121, /* 5*/
    59,153, 29,  9,213,167, 84, 93, 30, 46, 94, 75,151,114, 73,222,  /* 6*/
    197, 96,210, 45, 16,227,248,202, 51,152,252,125, 81,206,215,186, /* 7*/
    39,158,178,187,131,136,  1, 49, 50, 17,141, 91, 47,129, 60, 99,  /* 8*/
    154, 35, 86,171,105, 34, 38,200,147, 58, 77,118,173,246, 76,254, /* 9*/
    133,232,196,144,198,124, 53,  4,108, 74,223,234,134,230,157,139, /*10*/
    189,205,199,128,176, 19,211,236,127,192,231, 70,233, 88,146, 44, /*11*/
    183,201, 22, 83, 13,214,116,109,159, 32, 95,226,140,220, 57, 12, /*12*/
    221, 31,209,182,143, 92,149,184,148, 62,113, 65, 37, 27,106,166, /*13*/
    3, 14,204, 72, 21, 41, 56, 66, 28,193, 40,217, 25, 54,179,117,   /*14*/
    238, 87,240,155,180,170,242,212,191,163, 78,218,137,194,175,110, /*15*/
    43,119,224, 71,122,142, 42,160,104, 48,247,103, 15, 11,138,239,  /*16*/
    };

    if (!key) {
        return 0;
    }
    len = strlen(key);
    for (sum = 0, j = 0; j < 8; ++j) {
        h = T[(key[0] + j) % 256];
        for (i = 1; i < len; ++i) {
            h = T[h ^ key[i]];
        }
        sum += h << (7 - j);
    }
    return sum;
}

static size_t inner_hashmap_get_index(HashMap_T *hashmap, const char *key)
{
    size_t hash;
    
    if (!key) {
        return 0;
    }
    hash = hashmap->hash_func(key, strlen(key));
    return  hash % list_get_length(hashmap->table);
}

static HashMapPair_T *inner_hashmap_pair_create(const char *key, void *value)
{
    HashMapPair_T *hashmap_pair;
    size_t length;

    if (!key) {
        return NULL;
    }
    hashmap_pair = malloc(sizeof(*hashmap_pair));
    if (!hashmap_pair) {
        return NULL;
    }
    length = strlen(key) + 1;
    hashmap_pair->key = malloc(sizeof(char) * length);
    if (!hashmap_pair->key) {
        free(hashmap_pair);
        return NULL;
    }
    memcpy(hashmap_pair->key, key, length)
    hashmap_pair->value = value;
    return hashmap_pair;
}

static HashMapStatus_E inner_hashmap_resize(HashMap_T *hashmap)
{
    double coefficient;
    List_T *old_table, *new_table, *tmp;
    HashMapPair_T *a;
    size_t i, j;

    coefficient = (double)associations_length(as) / (double)(list_get_length(hashmap->table));
    if(coefficient > ASSOCIATIONS_RESIZE_COEF) {

        new_table = list_new();
        for(i = 0; i < list_get_length(hashmap->table) * 2; i++) {
            tmp = list_new();
            list_push_back(new_table, tmp);
        }
        
        old_table = hashmap->table;
        hashmap->table = new_table;
        for(i = 0; i < list_get_length(old_table); i++) {
            tmp = (List_T *)list_get(old_table, i);
            for(j = 0; j < list_get_length(tmp); j++) {
                a = (HashMapPair_T *)list_get(tmp, j);
                associations_set(as, a->key, a->val);
                ////////
                free(a->key);
                //free(a); //первое освобождение 
                ///////
            }
            list_free(tmp, free); //второе освобождение
        }
        list_free(old_table, nodes_pass);
    }
}

size_t hashmap_get_length(HashMap_T *hashmap)
{
    return hashmap ? hashmap->length : 0;
}

HashMap_T *hashmap_create(size_t size, size_t (*hash_func)(const char *))
{
    HashMap_T *hashmap;
    List_T *table, *row;;
    size_t i;

    table = list_create();
    if (!table) {
        return NULL;
    }
    size = size < HASHMAP_MINIMAL_SIZE ? HASHMAP_MINIMAL_SIZE : size;
    for(i = 0; i < size; i++) {
        row = list_create();
        if (!row) {
            list_destroy(table, free);
            return NULL;
        }
        list_push_back(table, row);
    }
    hashmap = malloc(sizeof(*hashmap));
    if (!hashmap) {
        list_destroy(table, free);
        return NULL;
    }
    hashmap->table = table;
    hashmap->hash_func = hash_func ? hash_func : inner_pearson_hash;
    return hashmap;
}


HashMapStatus_E hashmap_set(HashMap_T *hashmap, const char *key, void *value)
{
    HashMapPair_T *hashmap_pair;
    List_T *row;
    size_t i, index;

    if (!hashmap || !key) {
        return HASHMAP_STATUS_ERROR;
    }
    index = inner_hashmap_get_index(hashmap, key);
    row = (List_T *)list_get(hashmap->table, index);
    for(i = 0; i < list_get_length(row); i++) {
        hashmap_pair = (HashMapPair_T *)list_get(row, i);
        if(!strcmp(hashmap_pair->key, key)) {
            hashmap_pair->value = value;
            return HASHMAP_STATUS_SUCCESS;
        }
    }
    hashmap_pair = inner_hashmap_pair_create(key, value);
    list_push_back(row, hashmap_pair);
    hashmap->length++;
    inner_hashmap_resize(hashmap);
    return HASHMAP_STATUS_SUCCESS;
}

void *hashmap_get(HashMap_T *hashmap, const char *key)
{
    HashMapPair_T *hashmap_pair;
    List_T *row;
    size_t i, index;

    if (!hashmap || !key) {
        return NULL;
    }
    index = inner_hashmap_get_index(hashmap, key);
    row = (List_T *)list_get(hashmap->table, index);
    for(i = 0; i < list_get_length(row); i++) {
        hashmap_pair = (HashMapPair_T *)list_get(row, i);
        if(!strcmp(hashmap_pair->key, key)) {
            return hashmap_pair->value;
        }
    }
    return NULL;
}

HashMapStatus_E hashmap_remove(HashMap_T *hashmap, const char *key, void (*func)(void *))
{
    HashMapPair_T *hashmap_pair;
    List_T *row;
    size_t i, index;

    if (!hashmap || !key) {
        return HASHMAP_STATUS_ERROR;
    }
    index = inner_hashmap_get_index(hashmap, key);
    row = (List_T *)list_get(hashmap->table, index);
    for(i = 0; i < list_length(row); i++) {
        hashmap_pair = (HashMapPair_T *)list_get(row, i);
        if(!strcmp(hashmap_pair->key, key)) {
            if (func) {
                func(hashmap_pair->value);
            }
            free(hashmap_pair->key);
            list_remove(row, i, free);
            hashmap->length--;
            inner_hashmap_resize(hashmap);
            return HASHMAP_STATUS_SUCCESS;
        }
    }
    return HASHMAP_STATUS_KEY_ERROR;
}

HashMapStatus_E hashmap_iter(HashMap_T *hashmap, void (*func)(void *))
{
    List_T *row;
    HashMapPair_T *hashmap_pair;
    size_t i, j;

    if (!hashmap || !func) {
        return HASHMAP_STATUS_ERROR;
    }
    for(i = 0; i < list_get_length(hashmap->table); i++) {
        row = (List_T *)list_get(hashmap->table, i);
        for(j = 0; j < list_get_length(row); j++) {
            hashmap_pair = (HashMapPair_T *)list_get(row, j);
            func(hashmap_pair->value);
        }
    }
    return HASHMAP_STATUS_SUCCESS;
}

List_T *hashmap_get_keys(HashMap_T *hashmap)
{
    List_T *row, *keys;
    HashMapPair_T *hashmap_pair;
    char *key;
    size_t i, j, length;

    if (!hashmap) {
        return NULL;
    }
    keys = list_create();
    for(i = 0; i < list_get_length(hashmap->table); i++) {
        row = (List_T *)list_get(hashmap->table, i);
        for(j = 0; j < list_get_length(row); j++) {
            hashmap_pair = (HashMapPair_T *)list_get(row, j);
            length = strlen(hashmap_pair->key) + 1;
            key = malloc(sizeof(char) * length);
            if (!key) {
                list_destroy(keys, free);
                return NULL;
            }
            memcpy(key, hashmap_pair->key, length);
            list_push_back(keys, key_copy);
        }
    }
    return keys;
}

HashMapStatus_E hashmap_destroy(HashMap_T hashmap, void (*func)(void *)) {
    List_T *row;
    HashMapPair_T *hashmap_pair;
    size_t i, j;

    for(i = 0; i < list_get_length(hashmap->table); i++) {
        row = (List_T *)list_get(hashmap->table, i);
        for(j = 0; j < list_get_length(row); j++) {
            hashmap_pair = (HashMapPair_T *)list_get(row, j);
            if (func) {
                func(hashmap_pair->val);
            }
            free(hashmap_pair->key);
            free(hashmap_pair);
        }
        list_destroy(row, NULL);
    }
    list_destroy(hashmap->table, NULL);
    free(hashmap);
}
