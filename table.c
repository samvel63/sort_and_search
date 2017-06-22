#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

typedef struct _table {
	Line *rows;
	int size;
};

Table table_create(int size)
{
	Table table = (Table)malloc(sizeof(*table));
	table->size = size;
	table->rows = (Line *)malloc(sizeof(table->size));
	return table;
}

void table_destroy(Table *table)
{
	free((*table)->rows);
	free(*table);
}

int line_read(Line *line, FILE *f)
{
    fscanf(f, "%6s", line->key);
    fscanf(f, "%63s", line->data);
    return !feof(f);
}

void table_load(Table table, FILE *f)
{
	Line line;
	while (line_read(&line, f)) {
		//if (table->size % 10000 == 0)
		//	printf("%d\n", table->size);
		line_add(table, &line);
	}
}

void table_save(Table table, FILE *f)
{
	for (int i = 0; i < table->size; ++i) {
	    fwrite(table->rows[i].key,  sizeof(char), KEY_SIZE, f);
		fwrite(table->rows[i].data, sizeof(char), DATA_SIZE, f);
	}
}

void line_add(Table table, Line *line)
{
	table->size++;
	table->rows = (Line *)realloc(table->rows, (DATA_SIZE + KEY_SIZE) * table->size);
	strcpy(table->rows[table->size - 1].key, line->key);
	strcpy(table->rows[table->size - 1].data, line->data);
}

void line_copy(Line *to, Line *from)
{
	memcpy(to->key, from->key, KEY_SIZE);
	memcpy(to->data, from->data, DATA_SIZE);
}

void sort(Table res)
{
	int n = res->size;
    Line line;
    for (int i = n - 1; i >= 0; --i) {
    	if (i % 100 == 0)
    		printf("%d\n", i);
        for (int j = 0; j < i; ++j) {
            if (line_cmp(&res->rows[j], &res->rows[j + 1]) == 1) {
                line_copy(&line, &res->rows[j]);
                line_copy(&res->rows[j], &res->rows[j + 1]);
                line_copy(&res->rows[j + 1], &line);
            }
        }
    }
    printf("Таблица отсортирована.\n\n");
}

void table_print(Table table)
{
	for (int i = 0; i < table->size; ++i)
		printf("%s %s\n", table->rows[i].key, table->rows[i].data);
	printf("\n");
}

void test(Table table)
{
	table->size++;
	table->rows = (Line *)realloc(table->rows, (DATA_SIZE + KEY_SIZE) * (table->size + 1));
	table->rows[0].key[0] = 'a';
	table->rows[0].key[1] = '\0';
	printf("%s\n\n", table->rows[0].key);
}

void sift(Table t, int left, int right)
{
    int node_idx = left;
    int bigger_child_idx = 2 * left + 1;
    Line tmp_value;
    Line root_value;
    line_copy(&root_value, &t->rows[left]);

    if (bigger_child_idx < right
        && (line_cmp(&t->rows[bigger_child_idx], &t->rows[bigger_child_idx + 1]) == -1))
        ++bigger_child_idx;

    while (bigger_child_idx <= right
        && (line_cmp(&root_value, &t->rows[bigger_child_idx]) == -1)) {
        line_copy(&tmp_value, &t->rows[node_idx]);
        line_copy(&t->rows[node_idx], &t->rows[bigger_child_idx]);
        line_copy(&t->rows[bigger_child_idx], &tmp_value);
        node_idx = bigger_child_idx;
        bigger_child_idx = bigger_child_idx * 2 + 1;
        if (bigger_child_idx < right
            && (line_cmp(&t->rows[bigger_child_idx], &t->rows[bigger_child_idx + 1]) == -1))
            ++bigger_child_idx;
    }
    return;
}


void heap_sort(Table t)
{
    int n = t->size;

    int left, right;
    Line tmp_value;

    left = n / 2 + 1;
    while (left) {
        --left;
        sift(t, left, n - 1);
    }
    right = n - 1;
    while (right) {
        line_copy(&tmp_value, &t->rows[0]);
        line_copy(&t->rows[0], &t->rows[right]);
        line_copy(&t->rows[right], &tmp_value);
        --right;
        sift(t, 0, right);
    }
    printf("отсортирована\n\n");
}

void table_reverse(Table table)
{
	Line line;
	for (int i = 0; i < table->size / 2; ++i) {
		line_copy(&line, &table->rows[i]);
		line_copy(&table->rows[i], &table->rows[table->size - i - 1]);
		line_copy(&table->rows[table->size - i - 1], &line);
    }
}

int line_cmp(Line *line_1, Line *line_2)
{
	for (int i = 0; i < KEY_SIZE; ++i) {
		if (line_1->key[i] > line_2->key[i])
			return 1;
		if (line_1->key[i] < line_2->key[i])
			return -1;
	}
	return 0;
}

void table_shuffle(Table table)
{
	int n;
	Line line;
    srand(time(NULL));

    for (int i = 0; i < table->size; ++i) {
        n = rand() % table->size;
        line_copy(&line, &table->rows[n]);
        line_copy(&table->rows[n], &table->rows[i]);
        line_copy(&table->rows[i], &line);

    }   
    printf("Таблица помешана\n");
}

int table_is_sorted(Table table)
{
	for (int i = 0; i < table->size - 1; ++i) {
		if (line_cmp(&table->rows[i], &table->rows[i + 1]) == -1)
			return 0;
	}
	return 1;
}

void table_binary_search(Table table, const char *key)
{
	if (!table_is_sorted(table))
		sort(table);

    int start = 0;
    int end = table->size;
    int mid = (start + end) / 2;

    int lg;
    int mid_old = -1;

    while ((lg = strcmp(key, table->rows[mid].key)) != 0) {
        if (lg > 0)
            start = mid;
        else
            end = mid;
        mid_old = mid;
        mid = (start + end) / 2;
        if (mid == mid_old)
            return NULL;
    }
    printf("Значение: %s\n\n", table->rows[mid].data);
}