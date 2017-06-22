#ifndef _TABLE_H_
#define _TABLE_H_

#include <stdio.h>
#include <string.h>

#define KEY_SIZE 7
#define DATA_SIZE 64


typedef struct {
	char key[KEY_SIZE];
	char data[DATA_SIZE];
} Line;

typedef struct _table *Table;

Table table_create(int size);
void table_destroy(Table *table);
void table_load(Table table, FILE *f);
int line_read(Line *line, FILE *f);
void line_add(Table table, Line *line);
void table_print(Table table);
void line_copy(Line *to, Line *from);
void sort(Table res);
void table_reverse(Table table);
int line_cmp(Line *line_1, Line *line_2);
void table_shuffle(Table table);
void table_binary_search(Table table , const char *key);
void table_save(Table table, FILE *f);
void sift(Table t, int left, int right);
void heap_sort(Table t);

int table_is_sorted(Table table);
#endif // _TABLE_H_