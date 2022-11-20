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
	
	char string[2048];
	char** words = malloc(1024 * 256);
	int* idents = malloc(sizeof(int) * 1024 * 256);
	char* tokens = malloc(sizeof(1) * 2048);
	int tkn_pos = 0;
	c_map_T* mem_map = c_map_new(PDH_COMPILER_MEM_SIZE);
	int k_m_pos = 0;
	int v_m_pos = 0;
	int word_pos = 0;
	while (fgets(string, 2048, file_ptr) != NULL)
	{
		int success = token_tokenize(string, tokens, &tkn_pos);
		int str_pos = 0;
		while (string[str_pos] != '\0')
		{
			char* word = malloc(256);
			int c_pos = 0;
			int str_lock = 0;
			if (string[str_pos] != ' ' && string[str_pos] != '\t' && string[str_pos] != '\n')
			{
				while (1)
				{
					if (string[str_pos] == '"')
					{
						str_lock = !str_lock;
					}
					word[c_pos] = string[str_pos];
					str_pos++;
					c_pos++;
					if (!str_lock)
					{
						if (string[str_pos] == ' ' || string[str_pos] == '\t' || string[str_pos] == '\n' || string[str_pos] == '\0')
						{
							if (c_pos > 0)
							{
								word[c_pos] = 0;
								words[word_pos] = word;
								word_pos++;
							}
							break;
						}
					}
				}
			}
			else
			{
				while (string[str_pos] == ' ' || string[str_pos] == '\t' || string[str_pos] == '\n')
				{
					str_pos++;
				}
			}
		}
		if (success)
		{
			printf("{!} ERROR: %s\n", errors_reasons(success));
			return 1;
		}
	}
	
	printf("Token Pos: %d\n", tkn_pos);
	token_execute(tokens, words, idents);
	
	FILE* out_ptr = fopen(outpath, "wb");
	
	if (out_ptr == NULL)
	{
		perror("Couldn't load file");
		return 1;
	}
	
	fwrite(tokens, 1, tkn_pos, out_ptr);
	
	free(words);
	free(idents);
	free(mem_map);
	free(tokens);
	fclose(out_ptr);
	fclose(file_ptr);
	
	printf("Finished compiling");
	return 0;
}
int compile_raw_file(char* inpath)
{
	FILE* file_ptr = fopen(inpath, "rb");
	
	if (file_ptr == NULL)
	{
		perror("Couldn't load file");
		return 1;
	}
	
	char* tokens = malloc(sizeof(char) * 2048);
	fread(tokens, 1, 2048, file_ptr);
	
	for (int i = 0; i < sizeof(tokens); i++)
	{
		if (tokens[i] == TOKEN_END_OF_FILE)
			break;
		
		printf("Token: %d\n", tokens[i]);
	}
}

#endif