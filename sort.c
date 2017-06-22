#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

int main(void)
{
	Table table = table_create(0);
	
	FILE *f = fopen("input2", "r");              
	table_load(table, f);

	heap_sort(table);

	fclose(f);
	return 0;
}