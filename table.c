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
	while (line_read(&line, f))
		line_add(table, &line);
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

void tournament_sort(Table res)
{
    int n = res->size;
    int size;
    int idx;
    for (int i = 1; ; i *= 2) {
        if (i >= n) {
            size = i;
            break;
        }
    }

    Table tree = table_create(2 * size - 1);

    for (int i = 0; i < n; ++i)
        line_copy(&tree->rows[i + size - 1], &res->rows[i]);

    for (int i = size - 2; i >= 0; --i) {
        if (strcmp(tree->rows[2 * i + 1].key, tree->rows[2 * i + 2].key) == -1)
            line_copy(&tree->rows[i], &tree->rows[2 * i + 1]);
        else
            line_copy(&tree->rows[i], &tree->rows[2 * i + 2]);
    }

    for (int i = 0; i < n; ++i) {
        idx = 0;
        line_copy(&res->rows[i], &tree->rows[idx]);
        memset(tree->rows[idx].key, '\0', 6);

        while ( idx < size - 1) {
            if (!strcmp(tree->rows[idx * 2 + 1].key, res->rows[i].key)) {
                memset(tree->rows[idx * 2 + 1].key, '\0', 6);
                idx = idx * 2 + 1;
            } else {
                memset(tree->rows[idx * 2 + 2].key, '\0', 6);
                idx = idx * 2 + 2;
            }
        }

        while (idx) {
            idx -= 1;
            idx /= 2;
            if (strcmp(tree->rows[2 * idx + 1].key, tree->rows[2 * idx + 2].key) == -1)
                line_copy(&tree->rows[idx], &tree->rows[2 * idx + 1]);
            else
                line_copy(&tree->rows[idx], &tree->rows[2 * idx + 2]);
        }
    }

    table_destroy(&tree);
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