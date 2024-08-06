#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

#define HASHMAP_MINIMAL_SIZE 20
#define HASHMAP_INFLATE_TRIGGER_COEFICIENT 0.75
#define HASHMAP_DEFLATE_TRIGGER_COEFICIENT 0.25
#define HASHMAP_RESIZE_COEFICIENT 2


enum hashmap_status_e {
    HASHMAP_STATUS_SUCCESS = 0,
    HASHMAP_STATUS_ERROR = -1,
    HASHMAP_STATUS_MEMORY_ERROR = -2,
    HASHMAP_STATUS_KEY_ERROR = -3
};

struct hash_map_pair_t {
    char *key;
    void *value;
};

struct hash_map_t
{
    List_T *table;
    size_t length;
    size_t *(hash_func)(const char *);
};

typedef hashmap_status_e HashMapStatus_E;
typedef hashmap_pair_t HashMapPair_T;
typedef hashmap_t HashMap_T;

size_t hashmap_get_length(HashMap_T *hashmap);

HashMap_T *hashmap_create(size_t size, size_t (*hash_func)(const char *));

HashMapStatus_E hashmap_destroy(HashMap_T *hashmap, void (*func)(void *));

HashMapStatus_E hashmap_set(HashMap_T *hashmap, const char *key, void *value);

void *hashmap_get(HashMap_T *hashmap, const char *key);

HashMapStatus_E hashmap_remove(HashMap_T *hashmap, const char *key, void (*func)(void *));
 
HashMapStatus_E hashmap_iter(HashMap_T *hashmap, void (*func)(void *));

List_T *hashmap_get_keys(HashMap_T *hashmap);

#endif
