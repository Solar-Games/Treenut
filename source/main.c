#include "compiler.h"

int main(int argc, char** argv)
{
	if (strcmp(argv[argc - 2], "-r"))
	{
		compile_file_to_raw(argv[argc - 2], argv[argc - 1]);
	}
	else
	{
		compile_raw_file(argv[argc - 1]);
	}
}