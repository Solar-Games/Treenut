#include <stdlib.h>
#include <string.h>

#ifndef _TREENUT_Map_H
#define _TREENUT_Map_H

typedef struct {
	int size;
	char** k;
	void** v;
} c_map_T;

/*
	When making a map, you have to allocate the memory yourself.
*/
c_map_T* c_map_new(int size)
{
	c_map_T* map = malloc(sizeof(c_map_T));
	map->k = malloc(size);
	map->k = malloc(size);
	map->v = malloc(size);
	map->size = size;
	return map;
}
int c_map_has_key(c_map_T* c_m, char* k)
{
	for (int i = 0; i < c_m->size; i++)
	{
		if (strcmp(c_m->k[i], k))
		{
			return 1;
		}
	}
	return 0;
}

#endif