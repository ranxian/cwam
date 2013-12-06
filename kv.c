#include "kv.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

kv_tbl_t *kv_tbl_init()
{
	kv_tbl_t *table = malloc(sizeof(kv_tbl_t));
	table->len = 0;
	return table;
}

void kv_tbl_destroy(kv_tbl_t *table)
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

kv_t *kv_tbl_lookup_whtpx(kv_tbl_t *table, char *key)
{
	int i;
	int len = strlen(key);
	for (i = 0; i < table->len; i++) {
		kv_t *kv = &table->kvs[i];

		int yes = 1, j;
		for (j = 0; j < len; j++) {
			if (kv->key[j] != key[j]) {
				yes = 0;
				break;
			}
		}
		if (yes && (kv->key[len] == '~' || kv->key[len] == 0))
			return kv;
	}
	return NULL;
}

int kv_tbl_contains_whtpx(kv_tbl_t *table, char *key)
{
	int i;
	int len = strlen(key);
	for (i = 0; i < table->len; i++) {
		kv_t *kv = &table->kvs[i];

		int yes = 1, j;
		for (j = 0; j < len; j++) {
			if (kv->key[j] != key[j]) {
				yes = 0;
				break;
			}
		}
		if (yes && (kv->key[len] == '~' || kv->key[len] == 0))
			return 1;
	}
	return 0;
}

kv_t *kv_tbl_insert(kv_tbl_t *table, char *key, int intval, char *strval)
{
	int len = table->len;
	if (len + 1 >= MAX_KV_NUM) return NULL;
	strcpy(table->kvs[len].key, key);
	if (strval != NULL)
		strcpy(table->kvs[len].strval, strval);
	table->kvs[len].intval = intval;
	table->len += 1;

	return &table->kvs[len];
}

int kv_tbl_contains(kv_tbl_t *table, char *key)
{
	int i;
	for (i = 0; i < table->len; i++) {
			printf("%s\n", table->kvs[i].key);

		if (strcmp(key, table->kvs[i].key) == 0)
			return 1;
	}
	return 0;
}

int kv_tbl_remove(kv_tbl_t *table, char *key)
{
	int len = table->len;
	int i;
	for (i = 0; i < len; i++) {
		if (strcmp(table->kvs[i].key, key) == 0) {
			int j;
			for (j = i; j < len - 1; j++) {
				table->kvs[j] = table->kvs[j+1]; 
			}
			table->len -= 1;
			return 1;
		}
	}

	return 0;

}
