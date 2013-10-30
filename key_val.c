#include "key_val.h"
#include <stdlib.h>
#include <string.h>

kv_tbl_t *kv_tbl_init()
{
	kv_tbl_t *table = malloc(sizeof(kv_tbl_t));
	table->len = 0;
	return table;
}

void kv_tbl_destory(kv_tbl_t *table)
{
	free(table);
}

kv_t *kv_tbl_lookup(kv_tbl_t *table, char *key)
{
	int i;
	for (i = 0; i < table->len; i++) {
		if (strcmp(key, table->kvs[i].key) == 0) {
			return &table->kvs[i];
		}
	}
	return NULL;
}

int kv_tbl_insert(kv_tbl_t *table, char *key, int intval, char *strval)
{
	int len = table->len;
	if (len + 1 >= MAX_KV_NUM) return -1;
	strcpy(table->kvs[len].key, key);
	strcpy(table->kvs[len].strval, strval);
	table->kvs[len].intval = intval;
	table->len += 1;
	return 0;
}
int kv_tbl_contains(kv_tbl_t *table, char *key)
{
	int i;
	for (i = 0; i < table->len; i++) {
		if (strcmp(key, table->kvs[i].key) == 0)
			return 1;
	}
	return 1;
}
