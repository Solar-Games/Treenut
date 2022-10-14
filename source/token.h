#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

#ifndef _TREENUT_Token_H
#define _TREENUT_Token_H

/* File characters */
#define TOKEN_UNRECOGNIZED 	-2
#define TOKEN_END_OF_FILE 	-1
#define TOKEN_NEWLINE 		0

/* Types */
#define TOKEN_NUMBER 		1
#define TOKEN_DOT 			2
#define TOKEN_STRING 		3

/* Keywords */
#define TOKEN_DEFINE 		10
#define TOKEN_INCLUDE 		11
#define TOKEN_IF			12
#define TOKEN_WHILE			13

/* Syntax Characters */
#define TOKEN_SBR_LEFT		100
#define TOKEN_SBR_RIGHT		101
#define TOKEN_PARA_LEFT		102
#define TOKEN_PARA_RIGHT	103
#define TOKEN_BR_LEFT		104
#define TOKEN_BR_RIGHT		105
#define TOKEN_ARROW_LEFT	106
#define TOKEN_ARROW_RIGHT	107
#define TOKEN_SEMI_COLON	108

/* Arithmetic Characters */
#define TOKEN_PLUS			109
#define TOKEN_MINUS			110
#define TOKEN_TIMES			111
#define TOKEN_DIVIDE		112
#define TOKEN_POWER			113

/* Special Characters */
#define TOKEN_COMMENT		108

int token_stop(short* tkns, char* str, int position, int tkn_pos)
{
	return str[position] == '\0' || tkns[tkn_pos] == TOKEN_COMMENT || tkns[tkn_pos] == TOKEN_UNRECOGNIZED;
}
int token_recieve_errors(short* tkns, int position, int tkn_pos)
{
	if (tkns[position] == TOKEN_UNRECOGNIZED)
	{
		return ERRORS_UNRECOGNIZABLE;
	}
	return 0;
}
int token_skip(char* str, int position)
{
	return str[position] == ' ' || str[position] == '\t' || str[position] == '\r';
}
int token_equals_word(char* str, char* wrd, int position)
{
	int success = 1;
	int length = strlen(wrd);
	for (int i = 0; i < length; i++)
	{
		if (str[position + i] != wrd[i])
		{
			success = 0;
			break;
		}
	}
	return success;
}
int token_is_string(char* str, char* mem, int position)
{
	int success = str[position] == '"';
	int read = str[position] == '"';
	int length = 0;

	while (read)
	{
		read = str[position + 1 + length] != '"';
		if (str[position + 1 + length] == '\0')
		{
			success = 0;
			break;
		}
		if (read)
		{
			mem[length] = str[length + 1 + length];
			length++;
		}
	}
	mem[length + 1] = '\0';
	
	return success;
}
int token_tokenize(void** mem, short* tkns, char* str)
{
	int str_pos = 0;
	int tkn_pos = 0;
	int mem_pos = 0;
	int success = 0;
	while (1)
	{	
		if (token_skip(str, str_pos))
		{
			str_pos++;
			continue;
		}			
		
		switch (str[str_pos])
		{
				case '\0':
					tkns[tkn_pos] = TOKEN_END_OF_FILE;
					break;
					
				case '\n':
					tkns[tkn_pos] = TOKEN_NEWLINE;
					break;
					
				case '0'...'9':
					tkns[tkn_pos] = TOKEN_NUMBER;
					mem[mem_pos] = (str[str_pos] - '0');
					printf("%d\n", mem[mem_pos]);
					break;
					
				case '.':
					tkns[tkn_pos] = TOKEN_DOT;
					break;
			
				case '{':
					tkns[tkn_pos] = TOKEN_SBR_LEFT;
					break;
					
				case '}':
					tkns[tkn_pos] = TOKEN_SBR_RIGHT;
					break;
					
				case '(':
					tkns[tkn_pos] = TOKEN_PARA_LEFT;
					break;
					
				case ')':
					tkns[tkn_pos] = TOKEN_PARA_RIGHT;
					break;
					
				case '[':
					tkns[tkn_pos] = TOKEN_BR_LEFT;
					break;
					
				case ']':
					tkns[tkn_pos] = TOKEN_BR_RIGHT;
					break;
					
				case '<':
					tkns[tkn_pos] = TOKEN_ARROW_LEFT;
					break;
					
				case '>':
					tkns[tkn_pos] = TOKEN_ARROW_RIGHT;
					break;
					
				case '~':
					if (str[str_pos + 1] == '/')
					{
						tkns[tkn_pos] = TOKEN_COMMENT;
					}
					str_pos++;	
					break;
		
				default:
					tkns[tkn_pos] = TOKEN_UNRECOGNIZED;
		}
		
		int str_length = 0;
		char* str_mem = malloc(256);
		if (token_is_string(str, str_mem, str_pos))
		{
			tkns[tkn_pos] = TOKEN_STRING;
			mem[mem_pos] = str_mem;
			
			printf("%s\n", mem[mem_pos]);
			printf("%d\n", str_length);
			
			str_pos += str_length;
		}

		if (token_equals_word(str, "define", str_pos))
		{
			tkns[tkn_pos] = TOKEN_DEFINE;
			str_pos += 5;
		}
		
		if (token_equals_word(str, "include", str_pos))
		{
			tkns[tkn_pos] = TOKEN_INCLUDE;
			str_pos += 6;
		}
		if (token_equals_word(str, "if", str_pos))
		{
			tkns[tkn_pos] = TOKEN_IF;
			str_pos += 1;
		}
		
		if (token_equals_word(str, "while", str_pos))
		{
			tkns[tkn_pos] = TOKEN_WHILE;
			str_pos += 4;
		}
		
		printf("TOKEN: %d\n", tkns[tkn_pos]);
		
		if (token_stop(tkns, str, str_pos, tkn_pos))
		{
			success = token_recieve_errors(tkns, str_pos, tkn_pos);
			printf("SUCCESSFUL: %d\n", success);
			break;
		}
		
		free(str_mem);
		str_pos++;
		tkn_pos++;
	}
	return success;
}
	
#endif