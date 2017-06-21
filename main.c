#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

void help()
{
	printf("Помощь:\n");
    printf("1) Печать таблицы\n");
    printf("2) Проверка на сортированную таблицу\n");
    printf("3) Поиск значения по ключу\n");
    printf("4) Сортировка таблицы\n");
    printf("5) Помешать таблицу\n");
    printf("6) Перевернуть таблицу\n");
    printf("7) Помощь \n");
    printf("0) Выход\n");
    printf("\nВыберите действие:\n");  	
    
}

int main(int arc, char **argv)
{
	Table table = table_create(0);
	Line line;
	
	FILE *f = fopen(argv[1], "r");              
	table_load(table, f);
	fclose(f);

	help();

	char s[2];
	while (scanf("%9s", s) == 1) {
		if (!strcmp(s, "0")) {
			break;
		} else if (!strcmp(s, "1")) {
			table_print(table);
		} else if (!strcmp(s, "2")) {
			if (table_is_sorted(table))
				printf("Таблица упорядочена\n\n");
			else
				printf("Таблица неупорядочена\n\n");
		} else if (!strcmp(s, "3")) {
			char key[KEY_SIZE];
			scanf("%6s", key);
			table_binary_search(table, key);
		} else if (!strcmp(s, "4")) {
			sort(table);
		} else if (!strcmp(s, "5")) {
 			table_shuffle(table);
		} else if (!strcmp(s, "6")) {
			table_reverse(table);
		} else if (!strcmp(s, "7")) {
			help();
		}
		memset(s, '\0', 2);
	}
	table_destroy(&table);
	return 0;
}