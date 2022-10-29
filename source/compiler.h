#include "token.h"
#include "map.h"

#ifndef _TREENUT_Compiler_H
#define _TREENUT_Compiler_H

int compile_file_to_raw(char* inpath, char* outpath)
{
	FILE* file_ptr = fopen(inpath, "r");
	
	if (file_ptr == NULL)
	{
		perror("Couldn't load file");
		return 1;
	}
	
	char string[256];
	short tokens[256];
	c_map_T* mem_map = c_map_new(PDH_COMPILER_MEM_SIZE);
	int k_m_pos = 0;
	int v_m_pos = 0;
	while (fgets(string, 256, file_ptr) != NULL)
	{
		int success = token_tokenize(mem_map->k, mem_map->v, &k_m_pos, &v_m_pos, tokens, string);
		if (success)
		{
			printf("{!} ERROR: %s\n", errors_reasons(success));
			return 1;
		}
	}
	
	FILE* out_ptr = fopen(outpath, "wb");
	
	if (out_ptr == NULL)
	{
		perror("Couldn't load file");
		return 1;
	}
	
	fwrite(tokens, sizeof(tokens), sizeof(short), out_ptr);
	
	free(mem_map);
	fclose(file_ptr);
	fclose(out_ptr);
	return 0;
}
int compile_raw_file(char* inpath)
{
	FILE* file_ptr = fopen(inpath, "r");
	
	if (file_ptr == NULL)
	{
		perror("Couldn't load file");
		return 1;
	}
	
	short tokens[256];
	c_map_T* mem_map = c_map_new(PDH_COMPILER_MEM_SIZE);
	int k_m_pos = 0;
	int v_m_pos = 0;
	token_read(mem_map->k, mem_map->v, &k_m_pos, &v_m_pos, tokens);
		
	fread(tokens, sizeof(tokens), sizeof(short), file_ptr);
}

#endif