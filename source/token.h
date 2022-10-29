#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "pdh.h"

#ifndef _TREENUT_Token_H
#define _TREENUT_Token_H

/* File characters */
#define TOKEN_UNRECOGNIZED 	-2
#define TOKEN_END_OF_FILE 	-1
#define TOKEN_NEWLINE 		0

/* Types */
#define TOKEN_NAME			1
#define TOKEN_NUMBER 		2
#define TOKEN_DOT 			3
#define TOKEN_STRING 		4

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
#define TOKEN_SEMICOLON		108

/* Special Characters */
#define TOKEN_EQUALS		109
#define TOKEN_PLUS			110
#define TOKEN_DASH			111
#define TOKEN_ASTERISK		112
#define TOKEN_SLASH			113
#define TOKEN_CARET			114

/* Compiler Indicators */
#define TOKEN_COMMENT		115
#define TOKEN_PRINT			116
#define TOKEN_ERROR			117

int token_stop(short* tkns, char* str, int position, int tkn_pos, int success)
{
	return str[position] == '\0'
			|| str[position] == '\n'
			|| tkns[tkn_pos] == TOKEN_COMMENT
			|| tkns[tkn_pos] == TOKEN_UNRECOGNIZED
			|| success;
}
int token_is_defined(short* tkns, int tkn_pos)
{
	int type = tkns[tkn_pos] == TOKEN_NUMBER || tkns[tkn_pos] == TOKEN_STRING;
	return tkns[tkn_pos - 2] == TOKEN_NAME && tkns[tkn_pos - 1] == TOKEN_EQUALS && type;
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
int* token_is_string(char* str, char* mem, int str_pos)
{
	int success = str[str_pos] == '"';
	int error = str[str_pos] != '"' && str[str_pos] != '+';
	int read = str[str_pos] == '"';
	int length = 0;
	while (read)
	{
		read = str[str_pos + 1 + length] != '"';
		if (str[str_pos + 1 + length] == '\0')
		{
			success = 0;
			break;
		}
		if (read)
		{
			mem[length] = str[str_pos + 1 + length];
			length++;
		}
	}
	mem[length] = 0;
	
	int* a = malloc(sizeof(int) * 3);
	a[0] = success;
	a[1] = length;
	a[2] = error;
	return a;
}
int token_has_name(char** k_mem, int k_m_pos, char* name)
{
	for (int j = 0; j < k_m_pos; j++)
	{
		if (strcmp(name, k_mem[j]) == 0)
		{
			return 1;
		}
	}
	return 0;
}
int token_tokenize(char** k_mem, void** v_mem, int *k_m_pos, int *v_m_pos, short* tkns, char* str)
{
	int str_pos = 0;
	int tkn_pos = 0;
	
	int success = 0;
	
	int prd_defined = 0;
	char* printdata;
	while (1)
	{	
		if (token_skip(str, str_pos))
		{
			str_pos++;
			continue;
		}
		
		char* str_val = malloc(256);
		switch (str[str_pos])
		{
			case '\0':
				tkns[tkn_pos] = TOKEN_END_OF_FILE;
				break;
				
			case '\n':
				tkns[tkn_pos] = TOKEN_NEWLINE;
				break;
				
			case '0'...'9':
				int i = 0;
				int value = 0;
				while (str[str_pos + i] >= '0' && str[str_pos + i] <= '9')
				{
					value *= 10;
					value += (int) (str[str_pos + i] - '0');
					str_val[i] = str[str_pos + i];
					i++;
				}
				str_pos += i;
				tkns[tkn_pos] = TOKEN_NUMBER;
				str_val[i] = 0;
				if (token_is_defined(tkns, tkn_pos))
				{
					v_mem[v_m_pos[0]] = malloc(sizeof(int));
					memcpy(v_mem[v_m_pos[0]], &value, sizeof(int));
					v_m_pos[0]++;
				}
				if (tkns[tkn_pos - 1] == TOKEN_ERROR || tkns[tkn_pos - 1] == TOKEN_PRINT)
				{
					strcat(printdata, str_val);
				}				
				if (str[str_pos] == '+')
				{
					tkn_pos++;
					tkns[tkn_pos] = TOKEN_PLUS;
				}
				if (tkns[tkn_pos - 1] == TOKEN_NUMBER || tkns[tkn_pos - 1] == TOKEN_STRING || str[str_pos] == '"')
				{
					success = ERRORS_INCORRECT_SYNTAX;
				}
				if ((tkns[tkn_pos - 2] == TOKEN_NUMBER || tkns[tkn_pos - 2] == TOKEN_STRING) && tkns[tkn_pos - 1] == TOKEN_PLUS)
				{
					strcat(printdata, str_val);
				}
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
				
			case ';':
				tkns[tkn_pos] = TOKEN_SEMICOLON;
				break;
				
			case '=':
				tkns[tkn_pos] = TOKEN_EQUALS;
				break;
			
			case '+':
				tkns[tkn_pos] = TOKEN_PLUS;
				break;
			
			case '-':
				tkns[tkn_pos] = TOKEN_DASH;
				break;
		
			case '*':
				tkns[tkn_pos] = TOKEN_ASTERISK;
				break;
			
			case '/':
				tkns[tkn_pos] = TOKEN_SLASH;
				break;
				
			case '^':
				tkns[tkn_pos] = TOKEN_CARET;
				break;
				
			case '~':
				if (str[str_pos + 1] == '/')
				{
					tkns[tkn_pos] = TOKEN_COMMENT;
				}
				if (str[str_pos + 1] == '|')
				{
					printdata = malloc(2048);
					prd_defined = 1;
					tkns[tkn_pos] = TOKEN_PRINT;
				}
				if (str[str_pos + 1] == '!')
				{
					printdata = malloc(2048);
					prd_defined = 1;
					tkns[tkn_pos] = TOKEN_ERROR;
				}
				str_pos++;	
				break;
	
			default:
				tkns[tkn_pos] = TOKEN_UNRECOGNIZED;
		}
		
		char* str_mem = malloc(2048);
		int* str_args = token_is_string(str, str_mem, str_pos);
		if (str_args[0])
		{
			tkns[tkn_pos] = TOKEN_STRING;
			str_pos += str_args[1] + 1;
			if (token_is_defined(tkns, tkn_pos))
			{
				v_mem[v_m_pos[0]] = malloc(2048);
				strcpy(v_mem[v_m_pos[0]], str_mem);
				v_m_pos[0]++;
			}
			if (tkns[tkn_pos - 1] == TOKEN_ERROR || tkns[tkn_pos - 1] == TOKEN_PRINT)
			{
				strcat(printdata, str_mem);
			}
			if ((tkns[tkn_pos - 2] == TOKEN_NUMBER || tkns[tkn_pos - 2] == TOKEN_STRING) && tkns[tkn_pos - 1] == TOKEN_PLUS)
			{
				strcat(printdata, str_mem);
			}
			if (tkns[tkn_pos - 1] == TOKEN_NUMBER || tkns[tkn_pos - 1] == TOKEN_STRING)
			{
				success = ERRORS_INCORRECT_SYNTAX;
			}
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
		
		if (tkns[tkn_pos - 1] == TOKEN_DEFINE)
		{
			int i = 0;
			char* name = malloc(256);
			while (!token_skip(str, str_pos + i))
			{
				name[i] = str[str_pos + i];
				i++;
			}
			str_pos += i;
			name[i] = 0;
			int has = token_has_name(k_mem, k_m_pos[0], name);
			if (!has)
			{
				k_mem[k_m_pos[0]] = malloc(256);
				strcpy(k_mem[k_m_pos[0]], name);
				k_m_pos[0]++;
			}
			else
			{
				success = ERRORS_ALREADY_EXISTS;
			}
			tkns[tkn_pos] = TOKEN_NAME;
			free(name);
		}
		
		free(str_val);
		free(str_mem);
		free(str_args);
		if (token_stop(tkns, str, str_pos, tkn_pos, success))
		{
			int i = 0;
			while (1)
			{
				i++;
				printf("TOKEN: %d\n", tkns[tkn_pos - i]);
				if (tkns[tkn_pos - i] == TOKEN_NEWLINE)
					break;
				
				int end = 0;
				if (success == 0)
					switch (tkns[tkn_pos - i])
					{
						case TOKEN_ERROR:
							printf("{!}: %s\n", printdata);
							success = ERRORS_RAISED_BY_USER;
							end = 1;
							break;

						case TOKEN_PRINT:
							printf("{*}: %s\n", printdata);
							end = 1;
							break;

						default:
							break;
					}
					
				if (end)
					break;
			}
			if (success == 0)
				success = token_recieve_errors(tkns, str_pos, tkn_pos);
			
			break;
		}
		str_pos++;
		tkn_pos++;
	}
	free(printdata);
	return success;
}
int token_read(char** k_mem, void** v_mem, int *k_m_pos, int *v_m_pos, short* tkns)
{
	int tkn_pos = 0;
	
	int success = 0;
	while (1)
	{
		printf("TOKEN: %d\n", tkns[tkn_pos]);
		
		tkn_pos = 0;
		if (tkns[tkn_pos] == TOKEN_END_OF_FILE)
		{
			break;
		}
	}
	return success;
}
	
#endif
