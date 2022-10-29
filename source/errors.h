#ifndef _TREENUT_Errors_H
#define _TREENUT_Errors_H

#define ERRORS_UNRECOGNIZABLE	1
#define ERRORS_ALREADY_EXISTS	2
#define ERRORS_RAISED_BY_USER	3
#define ERRORS_INCORRECT_SYNTAX	4

char* errors_reasons(int selected)
{
	char* errors[] = {
		"Could not recognize keyword or token",
		"Definition already exists",
		"This error was raised by user",
		"Syntax isn't correct",
	};
	return errors[selected-1];
}

#endif