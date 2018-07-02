#include "include/inc.h"

void print_help()
{
	printf("Usage: 1) makedir [OPTION]... [DIR]... FILE - create file in DIR or current directory\n"
		   "Create the DIR(s), if it/they do(es) not already exist\n");
	printf("\n");
	printf("  -p (not provided)        make parent directory as needed\n"
		   "  -v         print a message for each created directory\n"
		   "  --help     display this help and exit\n"
		   "  --version  output version info and exit\n");
	printf("\n");

	exit(EXIT_SUCCESS);
}

void print_version()
{
	printf("makedir %3.2f", CURRENT_VERSION);
	printf("\n");

	exit(EXIT_SUCCESS);
}

