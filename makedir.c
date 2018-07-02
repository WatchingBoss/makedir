/*
 * Reimplementation of UNIX mkdir utiliry
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define CURRENT_VERSION 0.01

void my_error(char *);
void print_help();
void print_version();
int create_dir(char *, char *);

int main(int argc, char *argv[])
{
	char c;

	if(argc == 2 && (!strcmp(argv[1], "-help") || !strcmp(argv[1], "--help")) )
		print_help();
	if(argc == 2 && (!strcmp(argv[1], "-version") || !strcmp(argv[1], "--version")) )
		print_version();

	while( (c = getopt(argc, argv, "vp:")) > 0)
	{
		switch(c)
		{
			case 'v':
				printf("Use option: %c\n", c);
				break;
			case 'p':
				printf("Use option: %c\n", c);
				break;
			case '?':
				my_error("Wrong argument, error");
				break;
			default:
				break;
		}
	}

	if(argc % 2)
		create_dir(argv[argc - 2], argv[argc - 1]);
	else
		create_dir("pwd", argv[argc - 1]);

	exit(EXIT_SUCCESS);
}

int create_dir(char *source_dir, char *name_dir)
{
	char directory[256], total_path[256];
	strncpy(directory, source_dir, sizeof(directory));

	if(!strcmp(source_dir, "pwd"))
		if(!getcwd(directory, sizeof(directory)))
			my_error("getcwd error");

	strncat(directory, "/", sizeof(directory) - strlen(directory));
	strncat(directory, name_dir, sizeof(directory) - strlen(directory));
	memset(total_path, 0, sizeof(total_path));
	strncat(total_path, directory, sizeof(total_path));

	printf("file: %s\n", total_path);

	if( (mkdir(total_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) < 0)
		my_error("mkdir error");

	exit(EXIT_SUCCESS);
}

void print_help()
{
	printf("Usage: 1) makedir [OPTION]... [DIR]... FILE - create file in DIR or current directory\n"
		   "Create the DIR(s), if it/they do(es) not already exist\n");
	printf("\n");
	printf("  -p         no error if existing, make parent directory as needed\n"
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

void my_error(char *e)
{
	perror(e);
	exit(EXIT_FAILURE);
}
