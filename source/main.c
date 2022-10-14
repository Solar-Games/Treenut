#include "compiler.h"

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		compile_file(argv[i]);
	}
	scanf("%s");
}