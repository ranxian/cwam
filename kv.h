#ifndef KEY_VAL_H
#define KEY_VAL_H
#include "defs.h"
typedef struct {
	char key[256];
	int intval;
	char strval[256];
} kv_t;

typedef struct {
	kv_t kvs[MAX_KV_NUM];
	int len;
} kv_tbl_t;

kv_tbl_t *kv_tbl_init();
void kv_tbl_destroy();
kv_t *kv_tbl_lookup(kv_tbl_t *table, char *key);
kv_t *kv_tbl_insert(kv_tbl_t *table, char *key, int intval, char *strval);
int kv_tbl_remove(kv_tbl_t *table, char *key);
int kv_tbl_contains(kv_tbl_t *table, char *key);
int kv_tbl_contains_whtpx(kv_tbl_t *table, char *key);
kv_t *kv_tbl_lookup_whtpx(kv_tbl_t *table, char *key);
#endif
