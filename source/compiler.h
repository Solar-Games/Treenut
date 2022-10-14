#include "token.h"

#ifndef _TREENUT_Compiler_H
#define _TREENUT_Compiler_H

int compile_file(char* path)
{
	FILE* file_ptr = fopen(path, "r");
	
	if (file_ptr == NULL)
	{
		perror("Couldn't load file");
		return 1;
	}
	
	char string[256];
	short tokens[256];
	void* memory[2048];
	while (fgets(string, 256, file_ptr) != NULL)
	{
		int success = token_tokenize(memory, tokens, string);
		if (success)
		{
			printf("ERROR: %s\n", errors_reasons(success));
			break;
		}
	}
	fclose(file_ptr);
	return 0;
}

#endif