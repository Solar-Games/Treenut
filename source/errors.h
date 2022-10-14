#ifndef _TREENUT_Errors_H
#define _TREENUT_Errors_H

#define ERRORS_UNRECOGNIZABLE	1

char* errors_reasons(int selected)
{
	char* errors[] = {
		"Could not recognize keyword or token"
		"Unending string"
	};
	return errors[selected-1];
}

#endif